#include"pageJeu.h"
#include "utils.h"

static GtkWidget *fenetre = NULL;
static GtkBuilder *builder = NULL;
static GtkLabel *labMsgServer = NULL;
static GtkLabel *labPhase = NULL;
static GtkLabel *labMeilleurProp = NULL;
static GtkButton *buttonSoumission = NULL;
static GError *error = NULL;
static int isButtonXclicked=1;
static int isClosed = 0;
static GtkWidget *pTable;
static GtkLabel *pLabel[17][17];
static char* murLabel[17][17];
static pthread_t temps;
static char* name;
static int phaseReflexion=0; // Pase actuelle
static int phaseResolution=0;
static int phaseEnchere=0;
static int xR,xB,xJ,xV,xC; // coord x des robots et cibles
static int yR,yB,yJ,yV,yC; // coord y des robots cibles
static char cC[2];  // couleur cible
static int myCoup;

int proposition(GtkWidget * p_wid, gpointer p_data){
	GtkEntry* entry = (GtkEntry*)p_data;
	char* coups = gtk_entry_get_text(entry);

	if(strcmp(coups, "")){
		char messageEnvoye[70];

		if(phaseReflexion==1 && isdigit(*coups)){
			myCoup=atoi(coups);
			sprintf(messageEnvoye, "TROUVE/%s/%s/\n",name,coups);
			sendToServer(messageEnvoye);
		} else if(phaseEnchere==1 && isdigit(*coups)){
			myCoup=atoi(coups);
			sprintf(messageEnvoye, "ENCHERE/%s/%s/\n",name,coups);
			sendToServer(messageEnvoye);
		} else if(phaseResolution==1){
			sprintf(messageEnvoye, "SOLUTION/%s/%s/\n",name,coups);
			sendToServer(messageEnvoye);
		}

		gtk_entry_set_text(entry, "");
	}
	printf("end proposition\n");
	return 0;
}

void meilleureProposition(char* username, char* nbCoups){
	char message[60];
	if(nbCoups==NULL) sprintf(message, "Meilleure proposition : %s = %d", username, myCoup);
	else sprintf(message, "Meilleure proposition : %s = %d", username, atoi(nbCoups));

	gdk_threads_enter();
	gtk_label_set_text(labMeilleurProp, message);
	gdk_threads_leave();
}

void setPhase(char *phase){

	if(strstr(phase,"ENCHERE") && phaseReflexion==1){
		gdk_threads_enter();
		gtk_label_set_text(labPhase, "Phase d'enchere");
		gtk_button_set_label(buttonSoumission, "Votre enchère");
		gdk_threads_leave();
		phaseReflexion=0;
		phaseResolution=0;
		phaseEnchere=1;
		pthread_create(&temps, NULL, threadChrono, 40);
	}
	if(strstr(phase,"RESOLUTION") && phaseEnchere==1){
		gdk_threads_enter();
		gtk_label_set_text(labPhase, "Phase de resolution");
		gtk_button_set_label(buttonSoumission, "Votre solution");
		gdk_threads_leave();
		phaseEnchere=0;
		phaseReflexion=0;
		phaseResolution=1;
		pthread_create(&temps, NULL, threadChrono, 60);
	}
	if(strstr(phase,"REFLEXION")){
		gdk_threads_enter();
		gtk_label_set_text(labPhase, "Phase de reflexion");
		gtk_button_set_label(buttonSoumission, "Votre nombre de coups");
		gdk_threads_leave();
		phaseResolution=0;
		phaseReflexion=1;
		phaseEnchere=0;
		pthread_create(&temps, NULL, threadChrono, 300);

	}
}


int chat(GtkWidget * p_wid, gpointer p_data){

	GtkEntry* entry = (GtkEntry*)p_data;

	char messageEnvoye[200];
	char message[140];

	sprintf(message, "%s",  gtk_entry_get_text(entry));
	if(strcmp(name, "") && strcmp(message, "")){
		sprintf(messageEnvoye, "CHAT/%s/%s\n",name,message);
		sendToServer(messageEnvoye);
	}

	return 0;


}

void addMurTableau(int x, int y, char *mur){
	if (strstr(murLabel[x][y], mur) == NULL) {
		if(!strcmp(mur,"H")){
			strcat(murLabel[x][y+1],"B");
		}else {
			strcat(murLabel[x][y],mur);
		}
	}
}

void addMurTableauBase(){
	int i;
	int j;
	for(i=0; i<16;i++){
		for(j=15;j>=0;j--){
			if(i==0){
				addMurTableau(i,j,"G");
			} else if(i==15){
				addMurTableau(i,j,"D");
			}
			if(j==0){
				addMurTableau(i,j,"B");
			} else if(j==15){
				addMurTableau(i,j,"H");
			}
		}
	}
	addMurTableau(7,8,"G");
	addMurTableau(7,9,"B");
	addMurTableau(8,7,"D");
	addMurTableau(8,7,"B");
	addMurTableau(8,9,"B");
	addMurTableau(8,8,"D");
}


char* getCouleurCible(char* couleur){
	char coul[8];
	if(!strcmp(couleur, "cR")){
		sprintf(coul,"FF0000");
	}else if(!strcmp(couleur, "cA")){
		sprintf(coul, "0000FF");
	}else if(!strcmp(couleur, "cJ")){
		sprintf(coul, "EFD807");
	}else { // cV
		sprintf(coul, "00FF00");
	}
	return coul;
}

void displayRobot(){
	char couleur[8] = "";
	int x, y;
	int i;
	for(i=0; i<5; i++){
		if(i==0){
			x=xR;
			y=yR;
			sprintf(couleur, "FF0000");
		}else if(i==1){
			x=xB;
			y=yB;
			sprintf(couleur, "0000FF");
		}else if(i==2){
			x=xJ;
			y=yJ;
			sprintf(couleur, "EFD807");
		}else if(i==3){
			x=xV;
			y=yV;
			sprintf(couleur, "00FF00");
		} else {
			x=xC;
			y=yC;
			sprintf(couleur, getCouleurCible(cC));
		}
		printf("couleur %s\n", couleur);

		char res[100] = "";
		if(strstr(murLabel[x][y], "G")){
			if((x!=0)&&(y!=16)&& (strstr(murLabel[x-1][y],"D") &&  ((strstr(murLabel[x-1][y+1],"B")) || (strstr(murLabel[x-1][y],"B"))))
					&& 	!strstr(murLabel[x][y], "B") && !strstr(murLabel[x][y+1], "B") ){
				strcat(res, " ");
			}else{
				strcat(res, "|");
			}
		}else {
			strcat(res, "  ");
		}

		char lettre = 'R';
		if(i==4) lettre='C';

		if(strstr(murLabel[x][y], "B")){
			sprintf(res, "%s<span foreground=\"#%s\" face=\"Sans\"><u><small>%c</small></u></span>",res, couleur,lettre);
		}else{
			sprintf(res, "%s<span foreground=\"#%s\" face=\"Sans\"><small>%c</small></span>",res,couleur,lettre);
		}

		if(strstr(murLabel[x][y], "D")){
			if( (x!=15) && (y!=16) && (strstr(murLabel[x+1][y],"G")
					&&  ((strstr(murLabel[x+1][y+1],"B")) || (strstr(murLabel[x+1][y],"B"))) )){
				strcat(res, " ");
			}else{
				strcat(res, "|");
			}
		}else {
			strcat(res, "  ");
		}

		gdk_threads_enter();
		gtk_label_set_use_markup(pLabel[x][y], TRUE);
		gtk_label_set_markup(pLabel[x][y], res);
		gdk_threads_leave();
	}
	printf("Fin for display robot\n");
}


void displayPlateau(){
	printf("start display\n");
	int i;
	int j;
	for(i=0; i<16; i++){
		for(j=16; j>=0; j--){
			displayCase(i, j);
		}
	}
	printf("après for display\n");

	char* xSouligne = "|<span face=\"Sans\"><small><small><small>    </small></small></small></span>";
	char* secSouligne = "|<span face=\"Sans\"><u><small><small>sec</small></small></u></span>";

	gdk_threads_enter();	
	gtk_label_set_use_markup(pLabel[7][7], TRUE);
	gtk_label_set_markup(pLabel[7][7], secSouligne);
	gtk_label_set_use_markup(pLabel[7][8], TRUE);
	gtk_label_set_markup(pLabel[7][8], xSouligne);
	gdk_threads_leave();

	printf("fin display\n");
}


void resetRobot(){

	printf("Entre dans reset Robot\n");
	int x, y;
	int i;

	for(i=0;i<5;i++){
		if(i==0){
			x=xR;
			y=yR;
		}else if(i==1){
			x=xB;
			y=yB;
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
		displayCase(x, y);

	}
}

void displayCase(int x, int y){
	char res[5] = "";
	if(strstr(murLabel[x][y], "G")){
		if((x!=0)&&(y!=16)&& (strstr(murLabel[x-1][y],"D") &&  ((strstr(murLabel[x-1][y+1],"B")) || (strstr(murLabel[x-1][y],"B"))))
				&& 	!strstr(murLabel[x][y], "B") && !strstr(murLabel[x][y+1], "B") ){
			strcat(res, " ");
		}else{
			strcat(res, "|");
		}
	}else {
		strcat(res, "  ");
	}

	if(strstr(murLabel[x][y], "B")) strcat(res, "_");
	else strcat(res, " ");


	if(strstr(murLabel[x][y], "D")){
		if( (x!=15) && (y!=16) && (strstr(murLabel[x+1][y],"G")
				&&  ((strstr(murLabel[x+1][y+1],"B")) || (strstr(murLabel[x+1][y],"B"))) )){
			strcat(res, " ");
		} else {
			strcat(res, "|");
		}
	}else {
		strcat(res, "  ");
	}


	gdk_threads_enter();
	gtk_label_set_text( pLabel[x][y], res);
	gdk_threads_leave();
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
	printf("enigme avant addRobot == %s\n",enigme);

	char *p = enigme; // suppr 1ier et dernier char (les parentheses)
	p++;
	p[strlen(p)-1] = 0;

	printf("enigme après addRobot == %s\n",p);

	char** splitVirgule = splitWithChar(p, ',');

	xR=atoi(splitVirgule[0]);
	yR=atoi(splitVirgule[1]);
	xB=atoi(splitVirgule[2]);
	yB=atoi(splitVirgule[3]);
	xJ=atoi(splitVirgule[4]);
	yJ=atoi(splitVirgule[5]);
	xV=atoi(splitVirgule[6]);
	yV=atoi(splitVirgule[7]);
	xC=atoi(splitVirgule[8]);
	yC=atoi(splitVirgule[9]);

	strcat(cC, "c");
	if(strstr(splitVirgule[10],"B")){
		strcat(cC, "A");
	}else{
		strcat(cC, splitVirgule[10]);
	}

}

void threadChrono(int chrono){
	char temps[100];
	gdk_threads_enter();
	gtk_label_set_use_markup(pLabel[7][7], TRUE);
	gdk_threads_leave();
	int phaseResol=phaseResolution;
	int phaseEnch=phaseEnchere;
	int phaseReflex=phaseReflexion;

	while((chrono>=0) && (phaseReflex==phaseReflexion) && (phaseEnch==phaseEnchere)
			&& (phaseResol==phaseResolution)){ //On teste si on change pas de phase
		sprintf(temps, "|<span face=\"Sans\"><u><small><small><small>%d</small></small></small></u></span>", chrono);
		gdk_threads_enter();
		gtk_label_set_markup(pLabel[7][8], temps);
		gdk_threads_leave();
		sleep(1);
		chrono--;
	}
}

void affichageBilan(char *bilan){
	printf("on entre dans affichage bilan\n");
	printf("bilan ==== %s\n",bilan);

	gdk_threads_enter();
	GtkLabel *lab = GTK_WIDGET(gtk_builder_get_object (builder, "recapPartie"));
	gtk_label_set_text(lab, bilan);
	gdk_threads_leave();
}

void startReflexion(char* enigme, char* bilan){
	resetRobot();

	char* bilan_copy = strdup(bilan);
	char* enigme_copy = strdup(enigme);

	affichageBilan(bilan);
	addRobotCible(enigme);
	setPhase("REFLEXION");
	displayRobot();
}

int startPageJeu(char* plateau, char* pseudo){

	int i=0;
	int j=0;
	int x=0;
	int y=0;
	printf("on entre dans StartPageJEu\n");
	for(x=0;x<17;x++){
		for(y=0;y<17;y++){
			murLabel[x][y] = malloc(4 * sizeof(char));
			murLabel[x][y] = strdup("");
			pLabel[x][y] = gtk_label_new("   ");
		}
	}

	printf("apresStartPageJEu\n");
	name = pseudo;
	printf("apres name\n");
	if( !g_thread_supported()) g_thread_init( NULL );
	gdk_threads_init();

	addMurTableauBase();
	addMurTableauPlateau(plateau);
	displayPlateau();
	printf("after display !\n");

	isClosed = 0;


	printf("Before init \n");
	gdk_threads_enter();

	builder = gtk_builder_new();
	gchar* filename =  g_build_filename ("glade_files/pageJeu.glade", NULL);

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
	labMeilleurProp = GTK_WIDGET(gtk_builder_get_object (builder, "meilleurProposition"));
	buttonSoumission = GTK_BUTTON(gtk_builder_get_object(builder, "soumission"));

	gtk_misc_set_alignment(labMsgServer,0,0);

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


	g_signal_connect (gtk_builder_get_object (builder, "soumission"), "clicked", G_CALLBACK (proposition),(gpointer)(gtk_builder_get_object(builder, "proposition")));
	g_signal_connect (gtk_builder_get_object (builder, "bouttonChat"), "clicked", G_CALLBACK (chat),(gpointer)(gtk_builder_get_object(builder, "messageChat")));

	gtk_widget_show_all (fenetre);

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
	printf("Ajouter message server\n");

	char* toDisplay = gtk_label_get_text(labMsgServer);
	if(toDisplay==NULL || !strcmp(toDisplay, "") ){
		gdk_threads_enter();
		gtk_label_set_text(labMsgServer, message);
		gdk_threads_leave();
	} else {
		char res[strlen(toDisplay) + strlen(message) + 10];
		sprintf(res, "%s\n%s", toDisplay, message);
		gdk_threads_enter();
		gtk_label_set_text(labMsgServer, res);
		gdk_threads_leave();
	}
}
