/*
    C ECHO client example using sockets
*/
#include"pagePrincipal.h"

//#define FALSE 0 
//#define TRUE 1 
 
int sock; 
char user[255];
int connecte;


GtkWidget *fenetre_principale = NULL;
GtkBuilder *builder = NULL;
GError *error = NULL;
gchar *filename = NULL;
 
void OnDestroy(GtkWidget *pWidget, gpointer pData);
 
int connexion(GtkWidget * p_wid, gpointer p_data){

	GtkEntry* entry = (GtkEntry*)p_data;
	//puts(gtk_entry_get_text(entry)); //Recuperation du texte de l'area text
	
	//Initialisation des valeur
	char buffer[1024];
	char messageEnvoye[255];
	char connexionReussi[255];
    char fin[2];
    strcpy(fin,"/");
    strcpy(messageEnvoye,"CONNEX/");    
    
    //User prend la valeur entre par l'utilisateur
    strcpy(user, gtk_entry_get_text(entry));
    
    strcpy(connexionReussi,"BIENVENUE/");
	strcat(connexionReussi,user);
	strcat(connexionReussi,fin);

    puts("juste avant le cat du messageEnvoye");
    strcat(messageEnvoye, gtk_entry_get_text(entry));
    strcat(messageEnvoye, fin);
    puts("juste avant le send sock");
    send(sock, messageEnvoye, strlen(messageEnvoye), 0);
    
	int n = 0;
    if((n = recv(sock, buffer, sizeof buffer - 1, 0)) < 0)
    {
        perror("recv()");
    }

	//puts(strcmp(connexionReussi,buffer));
    if(strcmp(connexionReussi,buffer)==0){
    	puts("entre dans le if");
	    affichagePrincipale(user);
    }

    puts(buffer);    
    //OnDestroy(p_wid, p_data);
    puts("Connected\n");
} 


void OnDestroy(GtkWidget *pWidget, gpointer pData)
{
    /* Arrêt de la boucle événementielle */
    gtk_main_quit();
}
 
// gcc $(pkg-config --cflags --libs gtk+-2.0) client.c -o client
int main(int argc , char *argv[])
{
    
    sock = socket(AF_INET , SOCK_STREAM , 0);

    if (sock == -1){
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    //puts(server.sin_addr.s_addr);
    
	if (bind(sock, (struct sockaddr *)&server, sizeof (server))){
		perror("bind");
		/* Handle this error somehow */
    }
    
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){
        perror("connect failed. Error");
        return 1;
    }


	//instanciation de connecte a 0 pour dire qu'il est pas encore connecter
	connecte = 0;	
	
    /* Initialisation de la librairie Gtk. */
    gtk_init(&argc, &argv);

    /* Ouverture du fichier Glade de la fenêtre principale */
    builder = gtk_builder_new();

    /* Création du chemin complet pour accéder au fichier test.glade. */
    /* g_build_filename(); construit le chemin complet en fonction du système */
    /* d'exploitation. ( / pour Linux et \ pour Windows) */
    filename =  g_build_filename ("pageConnexion.glade", NULL);

      /* Chargement du fichier test.glade. */
    gtk_builder_add_from_file (builder, filename, &error);
    g_free (filename);
    if (error){
        gint code = error->code;
        g_printerr("%s\n", error->message);
        g_error_free (error);
        return code;
    }

    /* Récupération du pointeur de la fenêtre principale */
    fenetre_principale = GTK_WIDGET(gtk_builder_get_object (builder, "window1"));

    g_signal_connect (gtk_builder_get_object (builder, "connexion"), "clicked", G_CALLBACK (connexion),(gpointer)(gtk_builder_get_object(builder, "entry1")));

    //g_signal_connect (gtk_builder_get_object (builder, "connexion"), "destroy", G_CALLBACK (OnDestroy),NULL);


    // printf("lll %s",gtk_entry_get_text(GTK_ENTRY (gtk_builder_get_object(builder, "entry1"))));

    //gtk_button_set_visibile(GTK_BUTTON(gtk_builder_get_object(builder, "Deconnexion")), "False");

    /* Affichage de la fenêtre principale. */
    gtk_widget_show_all (fenetre_principale);

    gtk_main();

    return 0;
}
