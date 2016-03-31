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
GtkWidget *pTable;
GtkLabel *pLabel[17][17];
char* murLabel[17][17];
pthread_t temps;


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
	printf("mur = %s\n",mur);
	if(strstr(mur,"B")){
		printf("if = %s\n",mur);
		sprintf(tmp,"cA");
		addMurTableau(x,y,tmp);
	}else{
		printf("else = %s\n",mur);
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
							pLabel[i][j]= gtk_label_new(" ");
					}else{
						pLabel[i][j]= gtk_label_new("|");
					}
			}else {
				pLabel[i][j]= gtk_label_new("  ");
			}
			tmp=g_strdup_printf("%s",gtk_label_get_text (pLabel[i][j]));
			if(strstr(murLabel[i][j],"r")||strstr(murLabel[i][j],"c")){
				setCouleurLabel(i,j);
			}else{
				if(strstr(murLabel[i][j], "B")){
						tmp=g_strdup_printf("%s_",tmp);
						gtk_label_set_text (pLabel[i][j],tmp);
				}else {
					tmp=g_strdup_printf("%s ",tmp);
					gtk_label_set_text (pLabel[i][j],tmp);
				}
			}
			tmp=g_strdup_printf("%s",gtk_label_get_label (pLabel[i][j]));
			if(strstr(murLabel[i][j], "D")){
				if((i!=15)&&(j!=16)&&(strstr(murLabel[i+1][j],"G") &&  ((strstr(murLabel[i+1][j+1],"B")) || (strstr(murLabel[i+1][j],"B"))))){
						tmp=g_strdup_printf("%s ",tmp);
						gtk_label_set_markup (pLabel[i][j],tmp);
					}else{
						tmp=strdup(gtk_label_get_text (pLabel[i][j]));
						tmp=g_strdup_printf("%s|",tmp);
						gtk_label_set_markup (pLabel[i][j],tmp);
					}
			}else {
				tmp=g_strdup_printf("%s ",tmp);
				gtk_label_set_markup (pLabel[i][j],tmp);
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
		printf("chrono = %d",chrono);
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
		printf("chrono = %d",chrono);
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
		printf("chrono = %d",chrono);
		temps=g_strdup_printf("|<span face=\"Sans\"><u><small><small><small>%d</small></small></small></u></span>",chrono);
		gtk_label_set_markup(pLabel[7][7], temps);
		sleep(1);
		chrono--;
	}
}

int startPageJeu(char* usr)
{

	isClosed = 0;
	//	strcpy(user,usr);
	if( !g_thread_supported()) g_thread_init( NULL );
	gdk_threads_init();

	gtk_init(NULL,NULL);
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

   gtk_container_add(GTK_CONTAINER(gtk_builder_get_object (builder, "vpaned13")), GTK_WIDGET(pTable));

   int i=0;
   int j=0;
   int x=0;
   int y=0;

    for(x=0;x<17;x++){
		for(y=0;y<17;y++){
			murLabel[x][y]=strdup("");
		}
	}

    addMurTableauBase();

    char str[] = "(0,2,H)(0,3,B)(0,9,H)(0,10,B)(0,12,D)(1,6,D)(1,12,G)(1,12,H)(1,13,B)(2,5,H)(2,5,D)(2,6,G)(2,6,B)(2,9,H)(2,9,D)(2,10,B)(3,5,G)(3,8,H)(3,9,G)(3,9,B)(5,0,H)(5,1,B)(5,1,D)(5,15,D)(6,0,D)(6,1,G)(6,4,D)(6,10,H)(6,11,B)(6,11,D)(6,15,G)(7,0,G)(7,4,G)(7,4,H)(7,5,B)(7,10,D)(7,11,G)(8,2,D)(8,9,H)(8,10,G)(8,10,B)(8,15,D)(9,2,G)(9,2,H)(9,3,B)(9,14,D)(9,15,G)(10,0,D)(10,5,H)(10,6,B)(10,6,D)(10,13,H)(10,14,G)(10,14,B)(11,0,G)(11,6,G)(11,12,D)(12,4,H)(12,4,D)(12,5,B)(12,12,G)(12,12,H)(12,13,B)(12,13,D)(13,3,H)(13,4,G)(13,4,B)(13,9,H)(13,9,D)(13,10,B)(13,13,G)(14,9,G)(15,2,H)(15,3,B)(15,10,H)(15,11,B)";

    addMurTableauPlateau(str);

    addRobotCible("(14,7,4,11,2,1,8,4,10,14,B)");

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
