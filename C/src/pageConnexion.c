#include "pageConnexion.h"

static GtkWidget *fenetre = NULL;
static GtkBuilder *builder = NULL;
static GError *error = NULL;
static gchar *filename = NULL;
static int isButtonXclicked = 1;
static int isClosed = 0;

typedef int SOCKET;
SOCKET sock;
char user[50];

int connexion(GtkWidget * p_wid, gpointer p_data){
	GtkEntry* entry = (GtkEntry*)p_data;

	char messageEnvoye[70];
	char name[50];
	
	sprintf(name, "%s",  gtk_entry_get_text(entry));
	if(strcmp(name, "")){
		sprintf(messageEnvoye, "CONNEX/%s/\n", name);
		sendToServer(messageEnvoye);
	}
}

/* callback */
static void destroy( GtkWidget *widget, gpointer   data ){
	gtk_main_quit ();
}

int startPageConnexion(){
	isClosed = 0;
	if( !g_thread_supported()) g_thread_init( NULL );
	gdk_threads_init();

	gtk_init(NULL,NULL);
	builder = gtk_builder_new();
	filename =  g_build_filename ("glade_files/pageConnexion.glade", NULL);

	gtk_builder_add_from_file (builder, filename, &error);
	g_free (filename);
	if (error){
		gint code = error->code;
		g_printerr("%s\n", error->message);
		g_error_free (error);
		return code;
	}

	fenetre = GTK_WIDGET(gtk_builder_get_object (builder, "window1"));

	g_signal_connect (gtk_builder_get_object (builder, "connexion"), "clicked", G_CALLBACK (connexion),(gpointer)(gtk_builder_get_object(builder, "userText")));
	g_signal_connect (fenetre, "destroy", G_CALLBACK (destroy), NULL);

	gtk_widget_show_all (fenetre);
	gtk_main();

	isClosed = 1;
	printf("Fin de la fonction startPageConnexion\n");
	return isButtonXclicked;
}

/* Appelé par le programme pour fermer la fenêtre */
void destroyPageConnexion(){
	printf("Destruction de la fenetre\n");
	isButtonXclicked=0;
	gdk_threads_enter();
	gtk_widget_destroy(fenetre);
	gdk_threads_leave();
	printf("Destroy reussi\n");
}

void changeLabelPageConnexion(char* message){
	if(isClosed==1) return; // la fenêtre a été fermée
	gdk_threads_enter();
	{
		GtkLabel *lab = GTK_WIDGET(gtk_builder_get_object (builder, "user"));
		printf("Label before = %s\n", gtk_label_get_text(lab));
		gtk_label_set_text(lab, message);
		printf("Label after = %s\n", gtk_label_get_text(lab));
	}
	gdk_threads_leave();
}
