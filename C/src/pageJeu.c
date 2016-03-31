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

int deconnex(){

    char mess[255];
    char fin[2];
    strcpy(fin,"/");
    strcpy(mess,"SORT/");

    strcat(mess, user);
    strcat(mess, fin);

    send(sock, mess, strlen(mess), 0);

    close(sock);
}


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

void addMurLabel(){
	int i=0;
	int j=0;
	char *tmp;
	tmp=strdup("");
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
			if(strstr(murLabel[i][j], "B")){
				tmp=strdup(gtk_label_get_text (pLabel[i][j]));
				strcat(tmp,"_");
				gtk_label_set_text (pLabel[i][j],tmp);
			}else {
				tmp=strdup(gtk_label_get_text (pLabel[i][j]));
				strcat(tmp,"  ");
				gtk_label_set_text (pLabel[i][j],tmp);
			}
			if(strstr(murLabel[i][j], "D")){
				if((i!=15)&&(j!=16)&&(strstr(murLabel[i+1][j],"G") &&  ((strstr(murLabel[i+1][j+1],"B")) || (strstr(murLabel[i+1][j],"B"))))
					 ){
						tmp=strdup(gtk_label_get_text (pLabel[i][j]));
						strcat(tmp,"  ");
						gtk_label_set_text (pLabel[i][j],tmp);
					}else{
						tmp=strdup(gtk_label_get_text (pLabel[i][j]));
						strcat(tmp,"|");
						gtk_label_set_text (pLabel[i][j],tmp);
					}
			}else {
				tmp=strdup(gtk_label_get_text (pLabel[i][j]));
				strcat(tmp," ");
				gtk_label_set_text (pLabel[i][j],tmp);
			}
		}
	}
	gchar* secSouligne;
	gchar* xSouligne;
	gchar* labTime;
	secSouligne="<span face=\"Sans\"><u><small>S</small></u></span> |";
	xSouligne="| <span face=\"Sans\"><u><small><small>360</small></small></u></span>";
	labTime="| <span face=\"Sans\"><u><small><small><small>TIME</small></small></small></u></span>";
	gtk_label_set_use_markup(pLabel[7][7], TRUE);
	gtk_label_set_use_markup(pLabel[8][7], TRUE);
	gtk_label_set_markup(pLabel[7][8], labTime);
	gtk_label_set_markup(pLabel[7][7], xSouligne);
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

int startPageJeu(char* usr)
{

	isClosed = 0;
	//	strcpy(user,usr);
	//if( !g_thread_supported()) g_thread_init( NULL );
	//gdk_threads_init();

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
   printf("avant le for");
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
