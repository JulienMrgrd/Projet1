#include "pageAttente.h"

static GtkWidget *fenetre = NULL;
static GtkBuilder *builder = NULL;
static GError *error = NULL;
static gchar *filename = NULL;
static int isButtonXclicked = 1;

/* callback */
static void destroy( GtkWidget *widget, gpointer data ){
    gtk_main_quit ();
}
 
int startPageAttente(){
    gtk_init(NULL,NULL);
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

    gtk_widget_show_all (fenetre);
    gtk_main();

    printf("Fin de la fonction startPageAttente\n");
    return isButtonXclicked;
}

/* Appelé par le programme pour fermer la fenêtre */
void destroyPageAttente(){
	printf("Destruction de la fenetre d'attente\n");
	isButtonXclicked=0;
	gtk_widget_destroy(fenetre);
	printf("Destroy d'attente reussi\n");
}
