#include"pageJeu.h"

int sock; 
char user[255];

static GtkWidget *fenetre = NULL;
static GtkBuilder *builder = NULL;
static GError *error = NULL;
static gchar *filename = NULL;
static int isButtonXclicked=1;
static int isClosed = 0;

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


void* ecouteThread(){


	while(1){
		char buffer[255];
		int n=0;

		//PARTIE RECEPTION DES MESSAGE DU SERVEUR
		if((n = recv(sock, buffer, sizeof buffer - 1, 0)) < 0){
			perror("recv()");
		}

		GtkTextView *textview;
		textview = GTK_TEXT_VIEW(gtk_builder_get_object (builder, "textview2"));
		gtk_builder_get_object (builder, "textview2");
		gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview)),buffer, -1);
	}

}

int startPageJeu(char* plateau){
	isClosed = 0;
//	strcpy(user,usr);
	if( !g_thread_supported()) g_thread_init( NULL );
	gdk_threads_init();

	builder = gtk_builder_new();
	filename =  g_build_filename ("glade_files/pageJeu.glade", NULL);

	gtk_builder_add_from_file (builder, filename, &error);
	g_free (filename);
	if (error) {
		gint code = error->code;
		g_printerr("%s\n", error->message);
		g_error_free (error);
		return code;
	}
	fenetre = GTK_WIDGET(gtk_builder_get_object (builder, "window1"));

	GtkWidget *pTable;
	GtkWidget *pLabel[16][16];
	pTable=gtk_table_new(20,20,TRUE);
	gtk_container_add(GTK_CONTAINER(gtk_builder_get_object (builder, "vpaned13")), GTK_WIDGET(pTable));
	printf("avant le for");
	int i=0;
	int j=0;
	for(i=0;i<16;i++){
		for(j=0;j<16;j++){
			printf("i = %d j=%d\n",i,j);
			if(j==0||j==15){
				pLabel[i][j]= gtk_label_new("_");
				gtk_table_attach(GTK_TABLE(pTable), pLabel[i][j],
						i,i+1, j, j+1,
						GTK_EXPAND,  GTK_EXPAND,
						0, 0);
			}
			if((i==0||i==15) && j!=0 && j!=15){
				pLabel[i][j]= gtk_label_new("|");
				gtk_table_attach(GTK_TABLE(pTable), pLabel[i][j],
						i,i+1, j, j+1,
						GTK_EXPAND,  GTK_EXPAND,
						0, 0);
			}

		}

	}

	//ASSIGNATION DES TACHES POUR CHAQUE BOUTON
	// g_signal_connect (gtk_builder_get_object (builder, "Solution"), "clicked", G_CALLBACK (enchere),(gpointer)(gtk_builder_get_object(builder, "entry1")));

	//  g_signal_connect (gtk_builder_get_object (builder, "Deconnexion"), "clicked", G_CALLBACK (deconnex),NULL);

	//pthread_t thread1;

	//pthread_create(&thread1, NULL, ecouteThread, NULL);

	// printf("lll %s",gtk_entry_get_text(GTK_ENTRY (gtk_builder_get_object(builder, "entry1"))));

	//gtk_button_set_visibile(GTK_BUTTON(gtk_builder_get_object(builder, "Deconnexion")), "False");

	/* Affichage de la fenêtre jeu. */
	gtk_widget_show_all (fenetre);

	gtk_main();
	isClosed = 1;
	return isButtonXclicked;
}

/* Appelé par le programme pour fermer la fenêtre */
void destroyPageJeu(){
	printf("Destruction de la fenetre de jeu\n");
	isButtonXclicked=0;
	gdk_threads_enter();
	gtk_widget_destroy(fenetre);
	gdk_threads_leave();
	printf("Destroy de jeu reussi\n");
}

