#include "pageAttente.h"

static GtkWidget *fenetre = NULL;
static GtkBuilder *builder = NULL;

static GtkWidget *text_view;
static GtkTextBuffer *buffer;
static GtkWidget *scrolled_window;
static GtkTextIter iter;

static GError *error = NULL;
static gchar *filename = NULL;
static int isButtonXclicked = 1;
static int isClosed = 0;
static char* username;

/* callback */
static void destroy( GtkWidget *widget, gpointer data ){
	gtk_main_quit ();
}


/**
 * Lancement de l'affichage de la page d'attente, plus récupération du pointeur sur le label "messageAttente"
 */
int startPageAttente(char* pseudo){
	printf("Startpage Attente\n");
	isClosed = 0;

	if( !g_thread_supported()) g_thread_init( NULL );
	gdk_threads_init();
	gdk_threads_enter();

	builder = gtk_builder_new();
	filename =  g_build_filename ("glade_files/pageAttente.glade", NULL);

	gtk_builder_add_from_file (builder, filename, &error);
	g_free (filename);
	if (error){
		gint code = error->code;
		g_printerr("%s\n", error->message);
		g_error_free (error);
		return code;
	}

	fenetre = GTK_WIDGET(gtk_builder_get_object (builder, "window1"));
	g_signal_connect (fenetre, "destroy", G_CALLBACK (destroy), NULL);

	buffer = gtk_text_buffer_new(NULL);
    text_view = GTK_WIDGET(gtk_builder_get_object (builder, "textview"));
	gtk_text_view_set_buffer (text_view, buffer);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text_view), GTK_WRAP_WORD);
    scrolled_window = GTK_WIDGET(gtk_builder_get_object (builder, "scrolledwindow"));
    gtk_text_buffer_get_start_iter(buffer, &iter);

//	gtk_misc_set_alignment(scrolled_window,0,0);

	if(username==NULL){
		printf("Avant bienvenue\n");
		char bienvenue[60] = "Bienvenue dans la partie ";
		strcat(bienvenue, pseudo);
		gtk_text_buffer_insert_with_tags_by_name(buffer, &iter, bienvenue, -1, NULL, NULL, NULL, NULL,NULL);
		gtk_text_buffer_get_end_iter(buffer,&iter);
		printf("Après bienvenue\n");
	}
	username = pseudo;

	gtk_widget_show_all (fenetre);

	gtk_main();
	gdk_threads_leave();

	isClosed=1;
	printf("Fin de la fonction startPageAttente\n");
	return isButtonXclicked;
}

/* Appelé par le programme pour fermer la fenêtre */
void destroyPageAttente(){
	printf("Destruction de la fenetre d'attente\n");
	isButtonXclicked=0;
	gdk_threads_enter();
	gtk_widget_destroy(fenetre);
	gdk_threads_leave();
	printf("Destroy d'attente reussi\n");
}

/**
 * Ajout et affichage du message reçu par le serveur dans le label "messageAttente"
 * @param message : message a ajouter au label pour qu'il soit afficher
 */
void addMessageServerPageAttente(char* message){
	if(isClosed==1) return; // la fenêtre a été fermée

	printf("Avant ajouter message attente\n");
	char newMessage[strlen(message)+1];
	strcpy(newMessage, "\n");
	strcat(newMessage, message);

	gtk_text_buffer_insert(buffer, &iter, newMessage, -1);
	gtk_text_buffer_get_end_iter(buffer,&iter);
	printf("Après ajouter message attente\n");

}
