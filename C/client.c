#include <pthread.h>
#include "pageConnexion.h"
#include "utils.h"

typedef int SOCKET;
SOCKET sock;

pthread_t threadEcoute;
pthread_t threadFenetre;

void *fctThreadEcoute(void *arg){
	int i=0;
	int sizeMessageServer = 0;
	char* affich[512];
	char* recvBuffer[2000];

	printf("C'est parti pour le while true d'écoute ;)\n");
	while(1){
		if((sizeMessageServer = recv(sock, recvBuffer, 255, 0)) <= 0)
		{
			puts("Serveur déconnecté ...");
			break;
		}
		recvBuffer[sizeMessageServer] = '\0';

		char** res = splitWithChar(recvBuffer, '/');
		for (i=0; res[i] != NULL; i++) {
			printf("Resultat %d = %s\n", i, res[i]);
		}
		res[0];

		if(!strcmp(res[0],"BIENVENUE")){
			sprintf(affich,"Bienvenue %s",res[1]);
		}else if(!strcmp(res[0],"CONNECTE")){
			sprintf(affich,"%s vient de se connecter",res[1]);

			//DECONNEXION
		}else if(!strcmp(res[0],"SORTI")){
			sprintf(affich,"Nous avons un rage quit de la part de : %s",res[1]);

			//SESSION
		}else if(!strcmp(res[0],"SESSION")){
			//TODO:
			//Appel a la methode qui affiche le plateau sur la page
		}else if(!strcmp(res[0],"VAINQUEUR")){
			sprintf(affich,"Félicitation a %s vainqueur de cette session",res[1]);

			//REFLEXION
		}else if(!strcmp(res[0],"TOUR")){
			//TODO:
			//PLACEMENT DE ROBOTS
		}else if(!strcmp(res[0],"TUASTROUVE")){
			sprintf(affich,"Tu es le premier a avoir trouve !");
		}else if(!strcmp(res[0],"ILATROUVE")){
			sprintf(affich,"%s a trouve une solution en %s coups",res[1],res[2]); // ???? en X coups
		}else if(!strcmp(res[0],"FINREFLEXION")){
			sprintf(affich,"Temps imparti finit, fin de la phase de reflexion !");

			//ENCHERE
		}else if(!strcmp(res[0],"TUENCHERE")){
			sprintf(affich,"Enchere accepte !");
		}else if(!strcmp(res[0],"ECHECENCHERE")){
			sprintf(affich,"Votre enchere est incoherente a cause de celle de %s qui a encherit en %s",res[1],res[2]);// ???? en X coups
		}else if(!strcmp(res[0],"ILENCHERE")){
			sprintf(affich,"%s a effectuer une enchere en %s coups.",res[1],res[2]);// ???? en X coups
		}else if(!strcmp(res[0],"FINENCHERE")){
			if(res[1]!=NULL){
				sprintf(affich,"Les encheres sont finit le vainqueur est %s en %s coups.",res[1],res[2]); // ???? en X coups
			}else{
				sprintf(affich,"Les encheres sont finit il n'y a aucun vainqueur !");
			}
			//RESOLUTION
		}else if(!strcmp(res[0],"SASOLUTION")){
			sprintf(affich,"La solution de %s est : %s",res[1],res[2]);
		}else if(!strcmp(res[0],"BONNE")){
			sprintf(affich,"La solution est bonne, préparez-vous aux prochains tour !");
		}else if(!strcmp(res[0],"MAUVAISE")){
			if(res[1]!=NULL){
				sprintf(affich,"La solution est mauvaise, au tour de %s" ,res[1]);
			}else{
				sprintf(affich,"La solution est mauvaise !");
			}
		}else if(!strcmp(res[0],"FINRESO")){
			sprintf(affich,"Phase de resolution finit !");
		}else if(!strcmp(res[0],"TROPLONG")){
			if(res[1]!=NULL){
				sprintf(affich,"Temps de resolution finit, au tour de %s" ,res[1]);
			}else{
				sprintf(affich,"Temps de resolution finit !");
			}
		}

		puts(affich);
	}

	printf("Fin d'écoute du server (hors du while)\n");
	(void) arg; //Pour enlever le warning
	pthread_exit(NULL);
}

void *fctThreadFenetre(void *arg){
	affichageConnexion();
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

	if(pthread_create(&threadFenetre, NULL, fctThreadFenetre, NULL)) {
		perror("pthread_create");
		return EXIT_FAILURE;
	}

	if (pthread_join(threadEcoute, NULL)) {
		perror("pthread_join\n");
		return EXIT_FAILURE;
	} else {
		printf("Le thread d'écoute s'est fini.\n");
	}

	if (pthread_join(threadFenetre, NULL)) {
		perror("pthread_join\n");
		return EXIT_FAILURE;
	} else {
		printf("Le thread de la fenetre s'est fini.\n");
	}
	printf("\nFin du client ... Bye bye ;)");

	return 0;
}
