#include"pagePrincipale.h"
 
int sock; 
char user[255];

GtkWidget *fenetre_principale2 = NULL;
GtkBuilder *builder2 = NULL;
GError *error2 = NULL;
gchar *filename2 = NULL;
 
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
     
    gtk_button_set_label(GTK_BUTTON(gtk_builder_get_object(builder2, "Deconnexion")), "Connexion");

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
		textview = GTK_TEXT_VIEW(gtk_builder_get_object (builder2, "textview2"));
		gtk_builder_get_object (builder2, "textview2");
		gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview)),buffer, -1);
	}

}

int startPageJeu(char* usr)
{

          
    strcpy(user,usr);

    /* Initialisation de la librairie Gtk. */
    gtk_init(NULL, NULL);

    /* Ouverture du fichier Glade de la fenêtre principale */
    builder2 = gtk_builder_new();

    /* Création du chemin complet pour accéder au fichier test.glade. */
    /* g_build_filename(); construit le chemin complet en fonction du système */
    /* d'exploitation. ( / pour Linux et \ pour Windows) */
    filename2 =  g_build_filename ("glade_files/pageJeu.glade", NULL);
    printf("avant le for");

    /* Chargement du fichier test.glade. */
    gtk_builder_add_from_file (builder2, filename2, &error2);
    g_free (filename2);
    if (error2)
    {
    gint code = error2->code;
    g_printerr("%s\n", error2->message);
    g_error_free (error2);
    return code;
    }
    /* Récupération du pointeur de la fenêtre principale */
    fenetre_principale2 = GTK_WIDGET(gtk_builder_get_object (builder2, "window1"));

    GtkWidget *pTable;
    GtkWidget *pLabel[16][16];
    pTable=gtk_table_new(20,20,TRUE);
   gtk_container_add(GTK_CONTAINER(gtk_builder_get_object (builder2, "vpaned13")), GTK_WIDGET(pTable));
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
   // g_signal_connect (gtk_builder_get_object (builder2, "Solution"), "clicked", G_CALLBACK (enchere),(gpointer)(gtk_builder_get_object(builder2, "entry1")));

  //  g_signal_connect (gtk_builder_get_object (builder2, "Deconnexion"), "clicked", G_CALLBACK (deconnex),NULL);

    //pthread_t thread1;

    //pthread_create(&thread1, NULL, ecouteThread, NULL);

    // printf("lll %s",gtk_entry_get_text(GTK_ENTRY (gtk_builder_get_object(builder, "entry1"))));

    //gtk_button_set_visibile(GTK_BUTTON(gtk_builder_get_object(builder, "Deconnexion")), "False");

    /* Affichage de la fenêtre principale. */
    gtk_widget_show_all (fenetre_principale2);

    gtk_main();

    return 0;
}

