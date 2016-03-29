#include "connexion.h"

GtkWidget *fenetre_connexion = NULL;
GtkBuilder *builder = NULL;
GError *error = NULL;
gchar *filename = NULL;

/* callback */
static void destroy( GtkWidget *widget, gpointer   data ){
    gtk_main_quit ();
}
 
void startPageConnexion(){
    gtk_init(NULL,NULL);
    builder = gtk_builder_new();
    filename =  g_build_filename ("pageDeCo.glade", NULL);

    gtk_builder_add_from_file (builder, filename, &error);
    g_free (filename);
    if (error){
        gint code = error->code;
        g_printerr("%s\n", error->message);
        g_error_free (error);
        return;
    }

    fenetre_connexion = GTK_WIDGET(gtk_builder_get_object (builder, "window1"));

    g_signal_connect (gtk_builder_get_object (builder, "connexion"), "clicked", G_CALLBACK (connexion),(gpointer)(gtk_builder_get_object(builder, "userText")));
    g_signal_connect (fenetre_connexion, "destroy", G_CALLBACK (destroy), NULL);

    gtk_widget_show_all (fenetre_connexion);
    gtk_main();

    printf("Fin de la fonction startPageConnexion\n");
}

void destroyPageConnexion(){
	printf("Destruction de la fenetre\n");
	gtk_widget_destroy(fenetre_connexion);
	printf("Destroy reussi\n");
}
