#include <pthread.h>
#include "pageConnexion.h"
#include "pageAttente.h"
#include "utils.h"

typedef int SOCKET;
SOCKET sock;

pthread_t threadEcoute;
pthread_t threadFenetreConnexion;
pthread_t threadFenetreAttente;
pthread_t threadFenetrePrincipale;

void *fctThreadFenetreAttente(void *arg){
	startPageAttente();
	printf("(thread) Fin affichage d'attente");
	(void) arg; //Pour enlever le warning
	pthread_exit(NULL);
}

void *fctThreadFenetreConnexion(void *arg){
	startPageConnexion();
	printf("(thread) Fin affichage connexion");
	(void) arg; //Pour enlever le warning
	pthread_exit(NULL);
}

void *fctThreadEcoute(void *arg){
	int i=0;
	int sizeMessageServer;
	char* affich[512];
	char* recvBuffer[2000];

	printf("C'est parti pour le while true d'écoute ;)\n");
	while(1){
		sizeMessageServer=0;
		memset(affich, 0, sizeof affich);
		memset(recvBuffer, 0, sizeof recvBuffer);
		if((sizeMessageServer = recv(sock, recvBuffer, 255, 0)) <= 0){
			puts("Serveur déconnecté ...\n");
			break;
		} else {
			printf("Message reçu\n");
		}

		printf("size message : %d\n", sizeMessageServer);

		char** split = splitWithChar(recvBuffer, '/');
		if(sizeMessageServer==2){
			printf("Message reçu vide\n");
			continue;
		}

		for (i=0; (split[i] != NULL) ; i++) {
			printf("Resultat %d = %s\n", i, split[i]);
		}

		char* prot = split[0];

		if(!strcmp(prot,"BIENVENUE")){
			sprintf(affich,"Bienvenue %s",split[1]);
			printf("Fermeture de la fenetre de connexion dans client.c\n");
			destroyPageConnexion();
			printf("Fermeture de connexion effectuée dans client.c\n");
			printf("Démarrage de la fenêtre d'attente\n");
			if(pthread_create(&threadFenetreAttente, NULL, fctThreadFenetreAttente, NULL)) {
				perror("pthread_create");
				return EXIT_FAILURE;
			}
		}else if(!strcmp(prot,"CONNECTE")){
			sprintf(affich,"%s vient de se connecter",split[1]);

			//DECONNEXION
		}else if(!strcmp(prot,"SORTI")){
			sprintf(affich,"Nous avons un rage quit de la part de : %s",split[1]);

			//SESSION
		}else if(!strcmp(prot,"SESSION")){
			//TODO:
			//Appel a la methode qui affiche le plateau sur la page
		}else if(!strcmp(prot,"VAINQUEUR")){
			sprintf(affich,"Félicitation a %s vainqueur de cette session",split[1]);

			//REFLEXION
		}else if(!strcmp(prot,"TOUR")){
			//TODO:
			//PLACEMENT DE ROBOTS
		}else if(!strcmp(prot,"TUASTROUVE")){
			sprintf(affich,"Tu es le premier a avoir trouve !");
		}else if(!strcmp(prot,"ILATROUVE")){
			sprintf(affich,"%s a trouve une solution en %s coups",split[1],split[2]); // ???? en X coups
		}else if(!strcmp(prot,"FINREFLEXION")){
			sprintf(affich,"Temps imparti finit, fin de la phase de reflexion !");

			//ENCHERE
		}else if(!strcmp(prot,"TUENCHERE")){
			sprintf(affich,"Enchere accepte !");
		}else if(!strcmp(prot,"ECHECENCHERE")){
			sprintf(affich,"Votre enchere est incoherente a cause de celle de %s qui a encherit en %s",split[1],split[2]);// ???? en X coups
		}else if(!strcmp(prot,"ILENCHERE")){
			sprintf(affich,"%s a effectuer une enchere en %s coups.",split[1],split[2]);// ???? en X coups
		}else if(!strcmp(prot,"FINENCHERE")){
			if(split[1]!=NULL){
				sprintf(affich,"Les enchesplit sont finit le vainqueur est %s en %s coups.",split[1],split[2]); // ???? en X coups
			}else{
				sprintf(affich,"Les enchesplit sont finit il n'y a aucun vainqueur !");
			}
			//RESOLUTION
		}else if(!strcmp(prot,"SASOLUTION")){
			sprintf(affich,"La solution de %s est : %s",split[1],split[2]);
		}else if(!strcmp(prot,"BONNE")){
			sprintf(affich,"La solution est bonne, préparez-vous aux prochains tour !");
		}else if(!strcmp(prot,"MAUVAISE")){
			if(split[1]!=NULL){
				sprintf(affich,"La solution est mauvaise, au tour de %s" ,split[1]);
			}else{
				sprintf(affich,"La solution est mauvaise !");
			}
		}else if(!strcmp(prot,"FINRESO")){
			sprintf(affich,"Phase de splitolution finit !");
		}else if(!strcmp(prot,"TROPLONG")){
			if(split[1]!=NULL){
				sprintf(affich,"Temps de splitolution finit, au tour de %s" ,split[1]);
			}else{
				sprintf(affich,"Temps de splitolution finit !");
			}
		} else {
			sprintf(affich,"Commande \"%s\" inconnue.", prot);
		}

		puts(affich);
	}

	printf("Fin d'écoute du server (hors du while)\n");
	(void) arg; //Pour enlever le warning
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	sock = socket(AF_INET , SOCK_STREAM , 0);

	if (sock == INVALID_SOCKET){
		printf("Socket non crée\n");
	}

	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_family = AF_INET;
	server.sin_port = htons(2016);
	//puts(server.sin_addr.s_addr);

	//Connect to remote server
	if(connect(sock , (struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR) {
		perror("Erreur de connexion au server\nVous avez donné le bon IP en paramètre ?");
		return;
	}
	puts("Connexion au serveur réussie");

	if(pthread_create(&threadEcoute, NULL, fctThreadEcoute, NULL)) {
		perror("pthread_create");
		return EXIT_FAILURE;
	}

	if(pthread_create(&threadFenetreConnexion, NULL, fctThreadFenetreConnexion, NULL)) {
		perror("pthread_create");
		return EXIT_FAILURE;
	}

	if (pthread_join(threadEcoute, NULL)) {
		perror("pthread_join");
		return EXIT_FAILURE;
	}
	printf("After join.\n");

	printf("\nFin du client ... Bye bye ;)\n");

	return 0;
}
