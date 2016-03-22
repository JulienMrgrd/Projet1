/*
    C ECHO client example using sockets
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<gtk/gtk.h>  //gtk
 
int sock; 
struct sockaddr_in server;
char user[255];


GtkWidget *fenetre_principale = NULL;
GtkBuilder *builder = NULL;
GError *error = NULL;
gchar *filename = NULL;
 
int connex(){

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    printf("Entrer votre nom d'utilisateur : \n ");
    scanf("%s" , &user);

    char mess[255];
    char fin[2];
    strcpy(fin,"/");
    strcpy(mess,"CONNEX/");

    strcat(mess, user);
    strcat(mess, fin);
    
    send(sock, mess, strlen(mess), 0);
     
    puts("Connected\n");
} 
 
 
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


int enchere(){

    char message[50];

    printf("En combien de coup pouverz vous reussir ? \n ");
    scanf("%s" , &message);

    char fin[2];
    char mess[255];
    strcpy(fin,"/");
    strcpy(mess,"ENCHERE/");

    strcat(mess, user);
    strcat(mess, fin);
    strcat(mess, message);
    strcat(mess, fin);
    
    send(sock, mess, strlen(mess), 0);
     
	gtk_button_set_label(GTK_BUTTON(gtk_builder_get_object(builder, "Deconnexion")), "Connexion");

	return 0;

}
 
// gcc $(pkg-config --cflags --libs gtk+-2.0) client.c -o client
int main(int argc , char *argv[])
{
      /* Initialisation de la librairie Gtk. */
      gtk_init(&argc, &argv);

      /* Ouverture du fichier Glade de la fenêtre principale */
      builder = gtk_builder_new();

      /* Création du chemin complet pour accéder au fichier test.glade. */
      /* g_build_filename(); construit le chemin complet en fonction du système */
      /* d'exploitation. ( / pour Linux et \ pour Windows) */
      filename =  g_build_filename ("project1.glade", NULL);

          /* Chargement du fichier test.glade. */
      gtk_builder_add_from_file (builder, filename, &error);
      g_free (filename);
      if (error)
      {
        gint code = error->code;
        g_printerr("%s\n", error->message);
        g_error_free (error);
        return code;
      }

        /* Récupération du pointeur de la fenêtre principale */
      fenetre_principale = GTK_WIDGET(gtk_builder_get_object (builder, "MainWindow"));

      g_signal_connect (gtk_builder_get_object (builder, "Deconnexion"), "clicked", G_CALLBACK (enchere),NULL);



      /* Affichage de la fenêtre principale. */
      gtk_widget_show_all (fenetre_principale);

      gtk_main();

      return 0;
}
