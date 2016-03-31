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
	gchar* coul;
	tmp=g_strdup_printf("%s",gtk_label_get_text (pLabel[i][j]));
	if(strstr(murLabel[i][j], "B")){
		if(strstr(murLabel[i][j], "rR")){
			coul=g_strdup_printf("%s<span foreground=\"#FF0000\" face=\"Sans\"><u><small>R</small></u></span>",tmp);
		}else if(strstr(murLabel[i][j], "rA")){
			printf("%s\n",murLabel[i][j]);
			printf(" i = %d j = %d\n",i,j);
			coul=g_strdup_printf("%s<span foreground=\"#0000FF\" face=\"Sans\"><u><small>R</small></u></span>",tmp);
		}else if(strstr(murLabel[i][j], "rJ")){
			coul=g_strdup_printf("%s<span foreground=\"#FFFF00\" face=\"Sans\"><u><small>R</small></u></span>",tmp);
		}else if(strstr(murLabel[i][j], "rV")){
			coul=g_strdup_printf("%s<span foreground=\"#00FF00\" face=\"Sans\"><u><small>R</small></u></span>",tmp);
		}else if(strstr(murLabel[i][j], "cR")){
			coul=g_strdup_printf("%s<span foreground=\"#FF0000\" face=\"Sans\"><u><small>C</small></u></span>",tmp);
		}else if(strstr(murLabel[i][j], "cA")){
			coul=g_strdup_printf("%s<span foreground=\"#0000FF\" face=\"Sans\"><u><small>C</small></u></span>",tmp);
		}else if(strstr(murLabel[i][j], "cJ")){
			coul=g_strdup_printf("%s<span foreground=\"#E3FF00\" face=\"Sans\"><u><small>C</small></u></span>",tmp);
		}else if(strstr(murLabel[i][j], "cV")){
			coul=g_strdup_printf("%s<span foreground=\"#00FF00\" face=\"Sans\"><u><small>C</small></u></span>",tmp);
		}
	}else{
		if(strstr(murLabel[i][j], "rR")){
			coul=g_strdup_printf("%s<span foreground=\"#FF0000\" face=\"Sans\"><small>R</small></span>",tmp);
		}else if(strstr(murLabel[i][j], "rA")){
			coul=g_strdup_printf("%s<span foreground=\"#0000FF\" face=\"Sans\"><small>R</small></span>",tmp);
		}else if(strstr(murLabel[i][j], "rJ")){
			coul=g_strdup_printf("%s<span foreground=\"#FFFF00\" face=\"Sans\"><small>R</small></span>",tmp);
		}else if(strstr(murLabel[i][j], "rV")){
			coul=g_strdup_printf("%s<span foreground=\"#00FF00\" face=\"Sans\"><small>R</small></span>",tmp);
		}else if(strstr(murLabel[i][j], "cR")){
			coul=g_strdup_printf("%s<span foreground=\"#FF0000\" face=\"Sans\"><small>C</small></span>",tmp);
		}else if(strstr(murLabel[i][j], "cA")){
			coul=g_strdup_printf("%s<span foreground=\"#0000FF\" face=\"Sans\"><small>C</small></span>",tmp);
		}else if(strstr(murLabel[i][j], "cJ")){
			coul=g_strdup_printf("%s<span foreground=\"#E3FF00\" face=\"Sans\"><small>C</small></span>",tmp);
		}else if(strstr(murLabel[i][j], "cV")){
			coul=g_strdup_printf("%s<span foreground=\"#00FF00\" face=\"Sans\"><small>C</small></span>",tmp);
		}
	}
	gtk_label_set_use_markup(pLabel[i][j], TRUE);
	gtk_label_set_markup(pLabel[i][j], coul);
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
						gdk_threads_enter();
						pLabel[i][j]= gtk_label_new(" ");
						gdk_threads_leave();
					}else{
						gdk_threads_enter();
						pLabel[i][j]= gtk_label_new("|");
						gdk_threads_leave();
					}
			}else {
				gdk_threads_enter();
				pLabel[i][j]= gtk_label_new("  ");
				gdk_threads_leave();
			}
			tmp=g_strdup_printf("%s",gtk_label_get_text (pLabel[i][j]));
			if(strstr(murLabel[i][j],"r")||strstr(murLabel[i][j],"c")){
				setCouleurLabel(i,j);
			}else{
				if(strstr(murLabel[i][j], "B")){
						tmp=g_strdup_printf("%s_",tmp);
						gdk_threads_enter();
						gtk_label_set_text (pLabel[i][j],tmp);
						gdk_threads_leave();
				}else {
					tmp=g_strdup_printf("%s ",tmp);
					gdk_threads_enter();
					gtk_label_set_text (pLabel[i][j],tmp);
					gdk_threads_leave();
				}
			}
			tmp=g_strdup_printf("%s",gtk_label_get_label (pLabel[i][j]));
			if(strstr(murLabel[i][j], "D")){
				if((i!=15)&&(j!=16)&&(strstr(murLabel[i+1][j],"G") &&  ((strstr(murLabel[i+1][j+1],"B")) || (strstr(murLabel[i+1][j],"B"))))){
						tmp=g_strdup_printf("%s ",tmp);
						gdk_threads_enter();
						gtk_label_set_markup (pLabel[i][j],tmp);
						gdk_threads_leave();
					}else{
						tmp=strdup(gtk_label_get_text (pLabel[i][j]));
						tmp=g_strdup_printf("%s|",tmp);
						gdk_threads_enter();
						gtk_label_set_markup (pLabel[i][j],tmp);
						gdk_threads_leave();
					}
			}else {
				gdk_threads_enter();
				tmp=g_strdup_printf("%s ",tmp);
				gtk_label_set_markup (pLabel[i][j],tmp);
				gdk_threads_leave();
			}
		}
	}
	gchar* xSouligne;
	xSouligne="|<span face=\"Sans\"><small><small><small>    </small></small></small></span>";
	gtk_label_set_use_markup(pLabel[7][8], TRUE);
	gtk_label_set_markup(pLabel[7][8], xSouligne);

	xSouligne;
	xSouligne="|<span face=\"Sans\"><u><small><small><small>360</small></small></small></u></span>";
	gtk_label_set_use_markup(pLabel[7][7], TRUE);
	gtk_label_set_markup(pLabel[7][7], xSouligne);

	gchar* secSouligne;
	secSouligne="<span face=\"Sans\"><u><small><small>S</small></small></u></span> |";
	gtk_label_set_use_markup(pLabel[8][7], TRUE);
	gtk_label_set_markup(pLabel[8][7], secSouligne);
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

char *replace(char *st, char *orig, char *repl) {
  static char buffer[4096];
  char *ch;
  if (!(ch = strstr(st, orig)))
   return st;
  strncpy(buffer, st, ch-st);
  buffer[ch-st] = 0;
  sprintf(buffer+(ch-st), "%s%s", repl, ch+strlen(orig));
  return buffer;
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

void threadChronoReflexion(){
	int chrono=300;
	gchar* temps;
	gtk_label_set_use_markup(pLabel[7][7], TRUE);
	while(chrono>=0){
		temps=g_strdup_printf("|<span face=\"Sans\"><u><small><small><small>%d</small></small></small></u></span>",chrono);
		gtk_label_set_markup(pLabel[7][7], temps);
		sleep(1);
		chrono--;
	}
}

void threadChronoEnchere(){
	int chrono=40;
	gchar* temps;
	gtk_label_set_use_markup(pLabel[7][7], TRUE);
	while(chrono>=0){
		temps=g_strdup_printf("|<span face=\"Sans\"><u><small><small><small>%d</small></small></small></u></span>",chrono);
		gtk_label_set_markup(pLabel[7][7], temps);
		sleep(1);
		chrono--;
	}
}

void threadChronoResolution(){
	int chrono=60;
	gchar* temps;
	gtk_label_set_use_markup(pLabel[7][7], TRUE);
	while(chrono>=0){
		temps=g_strdup_printf("|<span face=\"Sans\"><u><small><small><small>%d</small></small></small></u></span>",chrono);
		gtk_label_set_markup(pLabel[7][7], temps);
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
	gdk_threads_leave();

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

    pthread_create(&temps, NULL, threadChronoReflexion, NULL);

    gtk_main();

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
