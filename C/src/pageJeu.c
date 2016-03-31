#include"pageJeu.h"
#include "utils.h"

int sock; 
char user[255];

static GtkWidget *fenetre = NULL;
static GtkBuilder *builder = NULL;
static GError *error = NULL;
static gchar *filename = NULL;
static int isButtonXclicked=1;
static int isClosed = 0;
static GtkWidget *pTable;
static GtkLabel *pLabel[17][17];
static char* murLabel[17][17];
static pthread_t temps;


int enchere(GtkWidget * p_wid, gpointer p_data){
	//Ajouter un boolean pour savoir on est dans quel etat
	//Reflexion ou Enchere ou Resolution
	char coup[255];
	GtkEntry* entry = (GtkEntry*)p_data;

	strcpy(coup, gtk_entry_get_text(entry));

	char fin[2];
	char mess[255];
	strcpy(fin,"/");
	strcpy(mess,"TROUVE/");

	strcat(mess, user);
	strcat(mess, fin);
	strcat(mess, coup);
	strcat(mess, fin);

	send(sock, mess, strlen(mess), 0);

	gtk_button_set_label(GTK_BUTTON(gtk_builder_get_object(builder, "Deconnexion")), "Connexion");

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
	if(isUnderline==1) coul = g_strdup_printf("%s<span foreground=\"#%s\" face=\"Sans\"><u><small>%c</small></u></span>",tmp, couleur, lettre);
	else g_strdup_printf("%s<span foreground=\"#%s\" face=\"Sans\"><small>%c</small></span>",tmp, couleur, lettre);

	gdk_threads_enter();
	gtk_label_set_use_markup(pLabel[i][j], TRUE);
	gtk_label_set_markup(pLabel[i][j], coul);
	gdk_threads_leave();
}


void addMurLabel(){
	int i=0;
	int j=0;
	gchar* tmp = NULL;

	for(i=0; i<16;i++){
		for(j=16;j>=0;j--){
			pLabel[i][j]=gtk_label_new("");
			if(strstr(murLabel[i][j], "G")){
				if((i!=0)&&(j!=16)&& (strstr(murLabel[i-1][j],"D") &&  ((strstr(murLabel[i-1][j+1],"B")) || (strstr(murLabel[i-1][j],"B"))))
						&& 	!strstr(murLabel[i][j], "B") && !strstr(murLabel[i][j+1], "B") ){
					pLabel[i][j]= gtk_label_new(" ");
				}else{
					pLabel[i][j]= gtk_label_new("|");
				}
			}else {
				pLabel[i][j]= gtk_label_new("  ");
			}
			gdk_threads_enter();
			tmp=g_strdup_printf("%s",gtk_label_get_text (pLabel[i][j]));
			gdk_threads_leave();
			if(strstr(murLabel[i][j],"r")||strstr(murLabel[i][j],"c")){
				setCouleurLabel(i,j);
			}else{
				if(strstr(murLabel[i][j], "B")) tmp=g_strdup_printf("%s_",tmp);
				else tmp=g_strdup_printf("%s ",tmp);
				gdk_threads_enter();
				gtk_label_set_text (pLabel[i][j],tmp);
				gdk_threads_leave();
			}
			gdk_threads_enter();
			tmp=g_strdup_printf("%s",gtk_label_get_label (pLabel[i][j]));
			gdk_threads_enter();
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
	gdk_threads_enter();

	xSouligne="|<span face=\"Sans\"><u><small><small><small>360</small></small></small></u></span>";
	gdk_threads_enter();
	gtk_label_set_use_markup(pLabel[7][7], TRUE);
	gtk_label_set_markup(pLabel[7][7], xSouligne);
	gdk_threads_enter();

	gchar* secSouligne;
	secSouligne="<span face=\"Sans\"><u><small><small>S</small></small></u></span> |";
	gdk_threads_enter();	
	gtk_label_set_use_markup(pLabel[8][7], TRUE);
	gtk_label_set_markup(pLabel[8][7], secSouligne);
	gdk_threads_enter();
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

	char** splitParentOuvr = splitWithChar(enigme, '(');
	char** splitParentFerm;
	char** splitVirgule;
	gchar* rR;
	int j=0;
	int i=0;
	int x;
	int y;
	int k=0;
	char *s;
	char *tmp;
	tmp=strdup("");
	for (i=0; (splitParentOuvr[i] != NULL) ; i++) {
		if(strlen(splitParentOuvr[i])!=0){
			splitParentFerm = splitWithChar(splitParentOuvr[i], ')');
			for(j=0; (splitParentFerm[j] != NULL) ; j++){
				splitVirgule = splitWithChar(splitParentFerm[j], ',');
				addMurTableau(atoi(splitVirgule[0]),atoi(splitVirgule[1]),"rR");
				addMurTableau(atoi(splitVirgule[2]),atoi(splitVirgule[3]),"rA");
				addMurTableau(atoi(splitVirgule[4]),atoi(splitVirgule[5]),"rJ");
				addMurTableau(atoi(splitVirgule[6]),atoi(splitVirgule[7]),"rV");
				addMurTableauCible(atoi(splitVirgule[8]),atoi(splitVirgule[9]),splitVirgule[10]);
			}
		}
	}
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
	gchar* bil;
	gdk_threads_enter();
	GtkLabel *lab = GTK_WIDGET(gtk_builder_get_object (builder, "recapPartie"));
	bil=g_strdup_printf("%s",bilan);
	gtk_label_set_text(lab, bil);
	gdk_threads_leave();
}

void startReflexion(char* enigme, char *bilan){
	printf("enigme === %s \n",enigme);
	addRobotCible(enigme);
	affichageBilan(bilan);
}

int startPageJeu(char* plateau){

	int i=0;
	int j=0;
	int x=0;
	int y=0;

	for(x=0;x<17;x++){
		for(y=0;y<17;y++){
			murLabel[x][y]=strdup("");
		}
	}

	isClosed = 0;

	if( !g_thread_supported()) g_thread_init( NULL );
	gdk_threads_init();

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

	pTable=gtk_table_new(48,20,FALSE);
	gdk_threads_enter();
	gtk_container_add(GTK_CONTAINER(gtk_builder_get_object (builder, "vpaned13")), GTK_WIDGET(pTable));

	addMurTableauBase();

	addMurTableauPlateau(plateau);

	addMurLabel();

	for(i=0;i<16;i++){
		for(j=16;j>=0;j--){
			gtk_table_attach(GTK_TABLE(pTable), pLabel[i][j],
					i*3,i*3+2, 16-j, 16-j+1,
					GTK_SHRINK ,  GTK_SHRINK,
					0, 0);
		}
	}

	gtk_widget_show_all (fenetre);

	pthread_create(&temps, NULL, threadChrono, 300);

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
