/*
    C ECHO client example using sockets
*/
#include"pagePrincipal.h"

//#define FALSE 0 
//#define TRUE 1 
 
typedef int SOCKET;
SOCKET sock;
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
    char fin[8], name[50];
    // strcpy(fin,"\0\r\n");
    // strcpy(messageEnvoye,"CONNEX/");
    sprintf(name, "%s",  gtk_entry_get_text(entry));
    sprintf(messageEnvoye, "CONNEX/%s/\n", name);

    
    puts("juste avant le send sock");
    if(send(sock, messageEnvoye, strlen(messageEnvoye), 0) < 0)
    {
        perror("send(pseudo)");
        return;
    }
    puts("juste apres le send sock");

    int n = 0;
	if((n = recv(sock, buffer, 255, 0)) < 0)
    {
        perror("recv()");
    }
	buffer[n] = '\0';
	//printf("on passe le recv\n");
   printf("buffer = %s taille %d, n: %d\n",buffer, strlen(buffer), n);

	sprintf(connexionReussi, "BIENVENUE/%s/\n", name);
    printf("connexionReussi = %s \n",connexionReussi);
	//puts(strcmp(connexionReussi,buffer));
    if(strcmp(connexionReussi,buffer)==0){
    	printf("entre dans le if");
	    affichagePrincipale(user);
    }

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

    if (sock == INVALID_SOCKET){
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(2016);
    //puts(server.sin_addr.s_addr);
    
//	if (bind(sock, (struct sockaddr *)&server, sizeof (server))){
//		perror("bind");
//		/* Handle this error somehow */
//    }
    
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR){
    	perror("connect()");
    	return;
    	//exit(errno);
    }

	
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
