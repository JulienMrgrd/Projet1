#include "pageAttente.h"

static GtkWidget *fenetre = NULL;
static GtkBuilder *builder = NULL;
static GError *error = NULL;
static gchar *filename = NULL;
static int isButtonXclicked = 1;
static int isClosed = 0;

/* callback */
static void destroy( GtkWidget *widget, gpointer data ){
	gtk_main_quit ();
}

int startPageAttente(){
	isClosed = 0;
	if( !g_thread_supported()) g_thread_init( NULL );
	gdk_threads_init();

//	gdk_threads_enter();
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

	GtkLabel *lab = GTK_WIDGET(gtk_builder_get_object (builder, "messageAttente"));
	gtk_misc_set_alignment(lab,0,0);

	gtk_widget_show_all (fenetre);
	gtk_main();
//	gdk_threads_leave();

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

void addMessageServerPageAttente(char *message){
	if(isClosed==1) return; // la fenêtre a été fermée
	strcat(message, "\n");

	gdk_threads_enter();
	{
		GtkLabel *lab = GTK_WIDGET(gtk_builder_get_object (builder, "messageAttente"));
		{
			char* toDisplay[strlen(message)+strlen(gtk_label_get_text(lab))];
			strcpy(toDisplay,gtk_label_get_text(lab));
			printf("Label before = %s\n", toDisplay);
			strcat(toDisplay, message);
			gtk_label_set_text(lab, toDisplay);
		}
		printf("Label after = %s\n", gtk_label_get_text(lab));
	}
	gdk_threads_leave();
//	free(toDisplay);
}
