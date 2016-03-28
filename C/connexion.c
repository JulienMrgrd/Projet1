#include "pagePrincipal.h"

typedef int SOCKET;
SOCKET sock;
char user[255];

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

    puts("Connected\n");
} 
