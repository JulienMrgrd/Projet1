#include "pageAttente.h"

GtkWidget *fenetre_attente = NULL;
GtkBuilder *builder_attente = NULL;
GError *error_attente = NULL;
gchar *filename_attente = NULL;

/* callback */
static void destroy( GtkWidget *widget, gpointer data ){
    gtk_main_quit ();
}
 
void startPageAttente(){
    gtk_init(NULL,NULL);
    builder_attente = gtk_builder_new();
    filename_attente =  g_build_filename ("glade_files/pageAttente.glade", NULL);

    gtk_builder_add_from_file (builder_attente, filename_attente, &error_attente);
    g_free (filename_attente);
    if (error_attente){
        gint code = error_attente->code;
        g_printerr("%s\n", error_attente->message);
        g_error_free (error_attente);
        return;
    }

    fenetre_attente = GTK_WIDGET(gtk_builder_get_object (builder_attente, "window1"));
    g_signal_connect (fenetre_attente, "destroy", G_CALLBACK (destroy), NULL);

    gtk_widget_show_all (fenetre_attente);
    gtk_main();

    printf("Fin de la fonction startPageAttente\n");
}

void destroyPageAttente(){
	printf("Destruction de la fenetre d'attente\n");
	gtk_widget_destroy(fenetre_attente);
	printf("Destroy d'attente reussi\n");
}
