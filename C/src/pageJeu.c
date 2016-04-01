#include"pageJeu.h"
#include "utils.h"

int sock; 
char user[255];

static GtkWidget *fenetre = NULL;
static GtkBuilder *builder = NULL;
static GtkLabel *labMsgServer = NULL;
static GtkLabel *labPhase = NULL;
static GtkButton *buttonSoumission = NULL;
static GError *error = NULL;
static gchar *filename = NULL;
static int isButtonXclicked=1;
static int isClosed = 0;
static GtkWidget *pTable;
static GtkLabel *pLabel[17][17];
static char* murLabel[17][17];
static pthread_t temps;
static char* name;
static int phaseReflexion=0;
static int phaseResolution=0;
static int phaseEnchere=0;
static int xR,xA,xJ,xV,xC;
static int yR,yA,yJ,yV,yC;
static char* cC;

int solution(GtkWidget * p_wid, gpointer p_data){
	GtkEntry* entry = (GtkEntry*)p_data;
	char* coups;
	char messageEnvoye[70];
	sprintf(coups, "%s",  gtk_entry_get_text(entry));

	if(phaseReflexion==1){
		if(strcmp(coups, "")){
			sprintf(messageEnvoye, "TROUVE/%s/%s/\n",name,coups);
			sendToServer(messageEnvoye);
		}
	}else if(phaseEnchere==1){
		if(strcmp(coups, "")){
			sprintf(messageEnvoye, "ENCHERE/%s/%s/\n",name,coups);
			sendToServer(messageEnvoye);
		}
	}else if(phaseResolution==1){
		if(strcmp(name, "")){
			sprintf(messageEnvoye, "SOLUTION/%s/%s/\n",name,coups);
			sendToServer(messageEnvoye);
		}
	}
	return 0;
}

void setPhase(char *phase){

	if(strstr(phase,"ENCHERE") && phaseReflexion==1){
		gdk_threads_enter();
		gtk_label_set_text(labPhase, "phase Enchere");
		gtk_button_set_label(buttonSoumission, "Encherir");
		gdk_threads_leave();
		phaseReflexion=0;
		phaseResolution=0;
		phaseEnchere=1;
	}
	if(strstr(phase,"RESOLUTION") && phaseEnchere==1){
		gdk_threads_enter();
		gtk_label_set_text(labPhase, "phase Resolution");
		gtk_button_set_label(buttonSoumission, "Proposer solution");
		gdk_threads_leave();
		phaseEnchere=0;
		phaseReflexion=0;
		phaseResolution=1;
	}
	if(strstr(phase,"REFLEXION")){
		gdk_threads_enter();
		gtk_label_set_text(labPhase, "phase Reflexion");
		gtk_button_set_label(buttonSoumission, "Encherir");
		gdk_threads_leave();
		phaseResolution=0;
		phaseReflexion=1;
		phaseEnchere=0;
	}
}


int chat(GtkWidget * p_wid, gpointer p_data){

	GtkEntry* entry = (GtkEntry*)p_data;

	char messageEnvoye[200];
	char message[140];

	sprintf(message, "%s",  gtk_entry_get_text(entry));
	if(strcmp(name, "")){
		sprintf(messageEnvoye, "CHAT/%s/%s\n",name,message);
		sendToServer(messageEnvoye);
	}

	return 0;

}

void addMurTableau(int x, int y, char *mur){
	if (strstr(murLabel[x][y], mur) != NULL) {
	}else{
		if(!strcmp(mur,"H")){
			strcat(murLabel[x][y+1],"B");
		}else {
			strcat(murLabel[x][y],mur);
		}
	}
}

void addMurTableauCible(int x, int y, char *mur){
	char *tmp;
	tmp=strdup("c");
	if(strstr(mur,"B")){
		sprintf(tmp,"cA");
		addMurTableau(x,y,tmp);
	}else{
		sprintf(tmp,"c%s",mur);
		addMurTableau(x,y,tmp);
	}
}

void addMurTableauBase(){
	int i=0;
	int j=0;
	char *tmp;
	tmp=strdup("");
	for(i=0; i<16;i++){
		for(j=15;j>=0;j--){
			if(i==0){
				addMurTableau(i,j,"G");
			}
			if(i==15){
				addMurTableau(i,j,"D");
			}
			if(j==0){
				addMurTableau(i,j,"B");
			}
			if(j==15){
				addMurTableau(i,j,"H");
			}
		}
	}
	addMurTableau(7,8,"G");
	addMurTableau(7,9,"B");
	addMurTableau(8,9,"B");
	addMurTableau(8,8,"D");
}


void setCouleurLabel(int i, int j){
	gchar* tmp = NULL;
	char* mur = murLabel[i][j];
	char lettre = 'R'; // par défaut (économise des lignes ci-dessous)
	char couleur[6] = "FF0000"; // par défaut
	gdk_threads_enter();
	tmp=g_strdup_printf("%s",gtk_label_get_text (pLabel[i][j]));
	gdk_threads_leave();

	int isUnderline = 0;
	if(strstr(mur, "B")) isUnderline = 1;

	if(strstr(mur, "rR")){
		// nada, on laisse les valeurs par défaut
	}else if(strstr(mur, "rA")){
		sprintf(couleur, "0000FF");
	}else if(strstr(mur, "rJ")){
		sprintf(couleur, "FFFF00");
	}else if(strstr(mur, "rV")){
		sprintf(couleur, "00FF00");
	}else if(strstr(mur, "cR")){
		lettre = 'C'; // couleur par defaut
	}else if(strstr(mur, "cA")){
		lettre = 'C';
		sprintf(couleur, "0000FF");
	}else if(strstr(mur, "cJ")){
		lettre = 'C';
		sprintf(couleur, "E3FF00");
	}else if(strstr(mur, "cV")){
		lettre = 'C';
		sprintf(couleur, "00FF00");
	}

	gchar* coul;
	if(isUnderline==1) coul = g_strdup_printf("%s<span foreground=\"#%s\" face=\"Sans\"><u><small>%s</small></u></span>",tmp, couleur, lettre);
	else coul = g_strdup_printf("%s<span foreground=\"#%s\" face=\"Sans\"><small>%s</small></span>",tmp, couleur, lettre);

	gdk_threads_enter();
	gtk_label_set_use_markup(pLabel[i][j], TRUE);
	gtk_label_set_markup(pLabel[i][j], coul);
	gdk_threads_leave();
}

char* getCouleur(char* couleur){
	char* coul;
	if(strstr(couleur, "cR")){
		sprintf(couleur,"FF0000");
		return coul;
	}else if(strstr(couleur, "cA")){
		sprintf(coul, "0000FF");
		return coul;
	}else if(strstr(couleur, "cJ")){
		sprintf(coul, "E3FF00");
		return coul;
	}else if(strstr(couleur, "cV")){
		sprintf(coul, "00FF00");
		return coul;
	}
}

void displayRobot(){
	printf("Entre dans display Robot\n");
	gchar* tmp = NULL;
	gchar* val;
	char* couleur;
	char* lettre="R";
	int x, y;
	int i=0;
	for(i=0;i<5;i++){
		printf("Debut for\n");
		if(i==0){
			x=xR;
			y=yR;
			couleur =strdup("FF0000");
		}else if(i==1){
			x=xA;
			y=yA;
			couleur =strdup("0000FF");
		}else if(i==2){
			x=xJ;
			y=yJ;
			couleur = strdup("E3FF00");
		}else if(i==3){
			x=xV;
			y=yV;
			couleur = strdup("00FF00");
		}else if(i==4){
			x=xC;
			y=yC;
			lettre="C";
			couleur = strdup(getCouleur(cC));
		}
		printf("Apres if i == ??\n");

		if(strstr(murLabel[x][y], "G")){
			if((x!=0)&&(y!=16)&& (strstr(murLabel[x-1][y],"D") &&  ((strstr(murLabel[x-1][y+1],"B")) || (strstr(murLabel[x-1][y],"B"))))
					&& 	!strstr(murLabel[x][y], "B") && !strstr(murLabel[x][y+1], "B") ){
				gdk_threads_enter();
				gtk_label_set_text(pLabel[x][y]," ");
				gdk_threads_leave();
			}else{
				gdk_threads_enter();
				gtk_label_set_text(pLabel[x][y],"|");
				gdk_threads_leave();
			}
		}else {
			gdk_threads_enter();
			gtk_label_set_text(pLabel[x][y],"  ");
			gdk_threads_leave();
		}
		printf("Apres 1er if\n");
		tmp=g_strdup_printf("%s",gtk_label_get_text (pLabel[x][y]));
		if(strstr(murLabel[x][y], "B")){
			val = g_strdup_printf("%s<span foreground=\"#%s\" face=\"Sans\"><u><small>%s</small></u></span>",tmp,couleur,lettre);
		}else{
			val = g_strdup_printf("%s<span foreground=\"#%s\" face=\"Sans\"><small>%s</small></span>",tmp,couleur,lettre);
		}
		gdk_threads_enter();
		gtk_label_set_use_markup(pLabel[x][y], TRUE);
		gtk_label_set_markup(pLabel[x][y], val);
		gdk_threads_leave();

		gdk_threads_enter();
		tmp=g_strdup_printf("%s",gtk_label_get_label (pLabel[x][y]));
		gdk_threads_leave();
		if(strstr(murLabel[x][y], "D")){
			if( (x!=15) && (y!=16) && (strstr(murLabel[x+1][y],"G")
					&&  ((strstr(murLabel[x+1][y+1],"B")) || (strstr(murLabel[x+1][y],"B"))) )){
				tmp=g_strdup_printf("%s ",tmp);
			}else{
				gdk_threads_enter();
				tmp=strdup(gtk_label_get_text (pLabel[x][y]));
				gdk_threads_leave();
				tmp=g_strdup_printf("%s|",tmp);
			}
		}else {
			tmp=g_strdup_printf("%s ",tmp);
		}
		gdk_threads_enter();
		gtk_label_set_markup (pLabel[x][y],tmp);
		gdk_threads_leave();
	}

}


void display(){
	int i=0;
	int j=0;
	gchar* tmp = NULL;

	for(i=0; i<16;i++){
		for(j=16;j>=0;j--){
			gdk_threads_enter();
			gtk_label_set_text(pLabel[i][j], "");
			gdk_threads_leave();

			if(strstr(murLabel[i][j], "G")){
				if((i!=0)&&(j!=16)&& (strstr(murLabel[i-1][j],"D") &&  ((strstr(murLabel[i-1][j+1],"B")) || (strstr(murLabel[i-1][j],"B"))))
						&& 	!strstr(murLabel[i][j], "B") && !strstr(murLabel[i][j+1], "B") ){
					gdk_threads_enter();
					gtk_label_set_text(pLabel[i][j]," ");
					gdk_threads_leave();
				}else{
					gdk_threads_enter();
					gtk_label_set_text(pLabel[i][j],"|");
					gdk_threads_leave();
				}
			}else {
				gdk_threads_enter();
				gtk_label_set_text(pLabel[i][j],"  ");
				gdk_threads_leave();
			}
			gdk_threads_enter();
			tmp=g_strdup_printf("%s",gtk_label_get_text (pLabel[i][j]));
			gdk_threads_leave();
//			if(strstr(murLabel[i][j],"r")||strstr(murLabel[i][j],"c")){
//				setCouleurLabel(i,j);
//			}else{
			if(strstr(murLabel[i][j], "B")) tmp=g_strdup_printf("%s_",tmp);
			else tmp=g_strdup_printf("%s ",tmp);
			gdk_threads_enter();
			gtk_label_set_text (pLabel[i][j],tmp);
			gdk_threads_leave();
		//	}
			gdk_threads_enter();
			tmp=g_strdup_printf("%s",gtk_label_get_label (pLabel[i][j]));
			gdk_threads_leave();
			if(strstr(murLabel[i][j], "D")){
				if( (i!=15) && (j!=16) && (strstr(murLabel[i+1][j],"G")
						&&  ((strstr(murLabel[i+1][j+1],"B")) || (strstr(murLabel[i+1][j],"B"))) )){
					tmp=g_strdup_printf("%s ",tmp);
				}else{
					gdk_threads_enter();
					tmp=strdup(gtk_label_get_text (pLabel[i][j]));
					gdk_threads_leave();
					tmp=g_strdup_printf("%s|",tmp);
				}
			}else {
				tmp=g_strdup_printf("%s ",tmp);
			}
			gdk_threads_enter();
			gtk_label_set_markup (pLabel[i][j],tmp);
			gdk_threads_leave();
		}
	}
	gchar* xSouligne;
	xSouligne="|<span face=\"Sans\"><small><small><small>    </small></small></small></span>";
	gdk_threads_enter();
	gtk_label_set_use_markup(pLabel[7][8], TRUE);
	gtk_label_set_markup(pLabel[7][8], xSouligne);
	gdk_threads_leave();

	xSouligne="|<span face=\"Sans\"><u><small><small><small>360</small></small></small></u></span>";
	gdk_threads_enter();
	gtk_label_set_use_markup(pLabel[7][7], TRUE);
	gtk_label_set_markup(pLabel[7][7], xSouligne);
	gdk_threads_leave();

	gchar* secSouligne;
	secSouligne="<span face=\"Sans\"><u><small><small>S</small></small></u></span> |";
	gdk_threads_enter();	
	gtk_label_set_use_markup(pLabel[8][7], TRUE);
	gtk_label_set_markup(pLabel[8][7], secSouligne);
	gdk_threads_leave();
}


void resetRobot(){

	printf("Entre dans reset Robot\n");
	gchar* tmp = NULL;
	gchar* val;
	char* couleur;
	char* lettre="R";
	int x, y;
	int i=0;
	for(i=0;i<5;i++){
		printf("Debut for\n");
		if(i==0){
			x=xR;
			y=yR;
		}else if(i==1){
			x=xA;
			y=yA;
		}else if(i==2){
			x=xJ;
			y=yJ;
		}else if(i==3){
			x=xV;
			y=yV;
		}else if(i==4){
			x=xC;
			y=yC;
		}
		printf("Apres if i == ??\n");

		if(strstr(murLabel[x][y], "G")){
			if((x!=0)&&(y!=16)&& (strstr(murLabel[x-1][y],"D") &&  ((strstr(murLabel[x-1][y+1],"B")) || (strstr(murLabel[x-1][y],"B"))))
					&& 	!strstr(murLabel[x][y], "B") && !strstr(murLabel[x][y+1], "B") ){
				gdk_threads_enter();
				gtk_label_set_text(pLabel[x][y]," ");
				gdk_threads_leave();
			}else{
				gdk_threads_enter();
				gtk_label_set_text(pLabel[x][y],"|");
				gdk_threads_leave();
			}
		}else {
			gdk_threads_enter();
			gtk_label_set_text(pLabel[x][y],"  ");
			gdk_threads_leave();
		}
		printf("Apres 1er if\n");
		tmp=g_strdup_printf("%s",gtk_label_get_text (pLabel[x][y]));
		if(strstr(murLabel[x][y], "B")) tmp=g_strdup_printf("%s_",tmp);
		else tmp=g_strdup_printf("%s ",tmp);
		gdk_threads_enter();
		gtk_label_set_text (pLabel[x][y],tmp);
		gdk_threads_leave();

		gdk_threads_enter();
		tmp=g_strdup_printf("%s",gtk_label_get_label (pLabel[x][y]));
		gdk_threads_leave();
		if(strstr(murLabel[x][y], "D")){
			if( (x!=15) && (y!=16) && (strstr(murLabel[x+1][y],"G")
					&&  ((strstr(murLabel[x+1][y+1],"B")) || (strstr(murLabel[x+1][y],"B"))) )){
				tmp=g_strdup_printf("%s ",tmp);
			}else{
				gdk_threads_enter();
				tmp=strdup(gtk_label_get_text (pLabel[x][y]));
				gdk_threads_leave();
				tmp=g_strdup_printf("%s|",tmp);
			}
		}else {
			tmp=g_strdup_printf("%s ",tmp);
		}
		gdk_threads_enter();
		gtk_label_set_markup (pLabel[x][y],tmp);
		gdk_threads_leave();
	}


}


void addMurTableauPlateau(char* plateau){

	char** splitParentOuvr = splitWithChar(plateau, '(');
	char** splitParentFerm;
	char** splitVirgule;
	char* tmp;
	int j=0;
	int i=0;
	for (i=0; (splitParentOuvr[i] != NULL) ; i++) {
		if(strlen(splitParentOuvr[i])!=0){
			splitParentFerm = splitWithChar(splitParentOuvr[i], ')');
			for(j=0; (splitParentFerm[j] != NULL) ; j++){
				splitVirgule = splitWithChar(splitParentFerm[j], ',');
				addMurTableau(atoi(splitVirgule[0]),atoi(splitVirgule[1]),(splitVirgule[2]));
			}
		}
	}
}

void addRobotCible(char* enigme){
	printf("On entre dans addRobotCible \n");
	printf("enigme == %s\n",enigme);
	char** splitParentOuvr = splitWithChar(enigme, '(');
	char** splitParentFerm;
	char** splitVirgule;

	splitParentFerm = splitWithChar(splitParentOuvr[1], ')');
	splitVirgule = splitWithChar(splitParentFerm[0], ',');
	xR=atoi(splitVirgule[0]);
	yR=atoi(splitVirgule[1]);
	xA=atoi(splitVirgule[2]);
	yA=atoi(splitVirgule[3]);
	xJ=atoi(splitVirgule[4]);
	yJ=atoi(splitVirgule[5]);
	xV=atoi(splitVirgule[6]);
	yV=atoi(splitVirgule[7]);
	xC=atoi(splitVirgule[8]);
	yC=atoi(splitVirgule[9]);
	printf("Before cC\n");
	cC=strdup("c");
	printf("After cC\n");
	if(strstr(splitVirgule[10],"B")){
		printf("then\n");
		strcat(cC, "A");
	}else{
		printf("else\n");
		strcat(cC, splitVirgule[10]);
	}
	printf("AFter splitVirgule 10\n");
//
//	addMurTableau(xR,yR,"rR");
//	addMurTableau(xA,yA,"rA");
//	addMurTableau(xJ,yJ,"rJ");
//	addMurTableau(xV,yV,"rV");
//	addMurTableau(xC,yC,cC);

}

void threadChrono(int chrono){
	gchar* temps;
	gdk_threads_enter();
	gtk_label_set_use_markup(pLabel[7][7], TRUE);
	gdk_threads_leave();
	while(chrono>=0){
		temps=g_strdup_printf("|<span face=\"Sans\"><u><small><small><small>%d</small></small></small></u></span>",chrono);
		gdk_threads_enter();
		gtk_label_set_markup(pLabel[7][7], temps);
		gdk_threads_leave();
		sleep(1);
		chrono--;
	}
}

void affichageBilan(char *bilan){
	printf("on entre dans affichage bilan\n");
	printf("bilan ==== %s\n",bilan);
	gchar* bil;
	gdk_threads_enter();
	GtkLabel *lab = GTK_WIDGET(gtk_builder_get_object (builder, "recapPartie"));
	bil=g_strdup_printf("%s",bilan);
	gtk_label_set_text(lab, bil);
	gdk_threads_leave();
}

void startReflexion(char* enigme, char *bilan){
	resetRobot();
	printf("enigme === %s \n",enigme);
	affichageBilan(bilan);
	addRobotCible(enigme);
	printf("Before display robot\n");
	displayRobot();
	printf("fin startreflex\n");
}

int startPageJeu(char* plateau, char* pseudo){

	int i=0;
	int j=0;
	int x=0;
	int y=0;
	printf("on entre dans StartPageJEu\n");
	for(x=0;x<17;x++){
		for(y=0;y<17;y++){
			murLabel[x][y]=strdup("");
			pLabel[x][y]=gtk_label_new("   ");
		}
	}

	printf("apreesStartPageJEu\n");
	name = strdup("");
	sprintf(name,"%s",pseudo);
	printf("apres name\n");
	if( !g_thread_supported()) g_thread_init( NULL );
	gdk_threads_init();

	addMurTableauBase();
	addMurTableauPlateau(plateau);
	display();

	isClosed = 0;


	printf("Before init \n");
	gdk_threads_enter();

	builder = gtk_builder_new();
	filename =  g_build_filename ("glade_files/pageJeu.glade", NULL);

	gtk_builder_add_from_file (builder, filename, &error);
	g_free (filename);
	if (error){
		gint code = error->code;
		g_printerr("%s\n", error->message);
		g_error_free (error);
		return code;
	}
	fenetre = GTK_WIDGET(gtk_builder_get_object (builder, "window1"));
	labMsgServer = GTK_WIDGET(gtk_builder_get_object (builder, "messageServeur"));
	labPhase = GTK_WIDGET(gtk_builder_get_object (builder, "phase"));
	buttonSoumission = GTK_BUTTON(gtk_builder_get_object(builder, "soumission"));

	pTable=gtk_table_new(48,20,FALSE);
	gtk_container_add(GTK_CONTAINER(gtk_builder_get_object (builder, "vpaned13")), GTK_WIDGET(pTable));

	for(i=0;i<16;i++){
		for(j=16;j>=0;j--){
			gtk_table_attach(GTK_TABLE(pTable), pLabel[i][j],
					i*3,i*3+2, 16-j, 16-j+1,
					GTK_SHRINK ,  GTK_SHRINK,
					0, 0);
		}
	}

	setPhase("Reflexion");

	g_signal_connect (gtk_builder_get_object (builder, "soumission"), "clicked", G_CALLBACK (solution),(gpointer)(gtk_builder_get_object(builder, "proposition")));
	g_signal_connect (gtk_builder_get_object (builder, "bouttonChat"), "clicked", G_CALLBACK (chat),(gpointer)(gtk_builder_get_object(builder, "messageChat")));
	
	gtk_widget_show_all (fenetre);

	pthread_create(&temps, NULL, threadChrono, 300);

	printf("Before main \n");
	gtk_main();
	gdk_threads_leave();
	
	isClosed = 1;
	return isButtonXclicked;
}

void destroyPageJeu(){
	printf("Destruction de la fenetre de jeu\n");
	isButtonXclicked=0;
	gdk_threads_enter();
	gtk_widget_destroy(fenetre);
	gdk_threads_leave();
	printf("Destroy de jeu reussi\n");
}


void addMessageServerPageJeu(char* message){
	if(isClosed==1) return; // la fenêtre a été fermée

	printf("Avant ajouter message server\n");
	gdk_threads_enter();
	char* toDisplay = gtk_label_get_text(labMsgServer);
	gdk_threads_leave();
	printf("Avant setText\n");
	if(toDisplay==NULL || !strcmp(toDisplay, "") ){
		printf("getText vide\n");
		gdk_threads_enter();
		gtk_label_set_text(labMsgServer, message);
		gdk_threads_leave();
	} else {
		printf("getText non vide\n");
		char* res[strlen(toDisplay) + strlen(message) + 2];
		sprintf(res, "%s\n%s", toDisplay, message);
		printf("Après cat toDisplay\n");
		gdk_threads_enter();
		gtk_label_set_text(labMsgServer, res);
		gdk_threads_leave();
	}
	printf("Après ajouter message server\n");
}
