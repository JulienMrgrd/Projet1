#include "pageAttente.h"

static GtkWidget *fenetre = NULL;
static GtkBuilder *builder = NULL;
static GError *error = NULL;
static gchar *filename = NULL;
static int isButtonXclicked = 1;
static int isClosed = 0;
static char* username;

/* callback */
static void destroy( GtkWidget *widget, gpointer data ){
	gtk_main_quit ();
}

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

	GtkLabel *lab = GTK_WIDGET(gtk_builder_get_object (builder, "messageAttente"));
	gtk_misc_set_alignment(lab,0,0);

	gtk_widget_show_all (fenetre);
	/*
	if(username==NULL){
		printf("Avant bienvenue\n");
		char toDisplay[] = "Bienvenue ";
		strcat(toDisplay, pseudo);
		printf("Après init toDisplay\n");
		gtk_label_set_text(GTK_WIDGET(gtk_builder_get_object (builder, "messageAttente")), toDisplay);
		printf("Après bienvenue\n");
	}
	username = *pseudo;
	 */
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

void addMessageServerPageAttente(char* message){
	if(isClosed==1) return; // la fenêtre a été fermée



	printf("Avant ajouter message attente\n");
	gdk_threads_enter();
	GtkLabel *lab = GTK_WIDGET(gtk_builder_get_object (builder, "messageAttente"));
	char* toDisplay = gtk_label_get_text(lab);
	gdk_threads_leave();
	printf("Avant setText\n");
	if(toDisplay==NULL || !strcmp(toDisplay, " ") ){
		printf("getText vide\n");
		gdk_threads_enter();
		gtk_label_set_text(lab, message);
		gdk_threads_leave();
	} else {
		printf("getText non vide\n");
		char* res[strlen(toDisplay) + strlen(message) + 2];
		printf("Après instanc toDisplay\n");
		printf("Label before = %s\n", toDisplay);
		sprintf(res, "%s\n%s", toDisplay, message);
		printf("Après cat toDisplay\n");
		gdk_threads_enter();
		gtk_label_set_text(lab, res);
		gdk_threads_leave();
//		toDisplay=NULL;
//		res=NULL;
	}
	printf("Après setText\n");
	gdk_threads_enter();
	printf("Label after = %s\n", gtk_label_get_text(lab));
	gdk_threads_leave();
	printf("Après ajouter message attente\n");

	printf("Après leave le thread message attente\n");
//	sleep(1);
}
