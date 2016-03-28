#include "connexion.h"

GtkWidget *fenetre_connexion = NULL;
GtkBuilder *builder = NULL;
GError *error = NULL;
gchar *filename = NULL;
 
int affichageConnexion(){

	/* Initialisation de la librairie Gtk. */
    gtk_init(NULL,NULL);

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
    fenetre_connexion = GTK_WIDGET(gtk_builder_get_object (builder, "window1"));

    g_signal_connect (gtk_builder_get_object (builder, "connexion"), "clicked", G_CALLBACK (connexion),(gpointer)(gtk_builder_get_object(builder, "entry1")));

    /* Affichage de la fenêtre principale. */
    gtk_widget_show_all (fenetre_connexion);

    gtk_main();

    return 0;
}
