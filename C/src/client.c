#include "client.h"

typedef int SOCKET;
SOCKET sock;

pthread_t threadEcoute;
pthread_t threadFenetreConnexion;
pthread_t threadFenetreAttente;
pthread_t threadFenetreJeu;
char* username;


void fctThreadFenetreConnexion(){
	if(startPageConnexion()==1){
		printf("Bouton \"X\" cliqué !\n");
		shutdown(sock, SHUT_RDWR);
	}
	printf("(thread) Fin affichage connexion\n");
	pthread_exit(NULL);
}

void fctThreadFenetreAttente(){
	if(startPageAttente()==1){
		printf("Bouton \"X\" cliqué !\n");
		char* message;
		strcpy(message,"SORT/");
		strcat(message, username);
		sendToServer(message);
		shutdown(sock, SHUT_RDWR);
	}
	printf("(thread) Fin affichage d'attente\n");
	pthread_exit(NULL);
}

void fctThreadFenetreJeu(char* plateau){
	if(startPageJeu(plateau)==1){
		printf("Bouton \"X\" cliqué !\n");
		char* message;
		strcpy(message,"SORT/");
		strcat(message, username);
		sendToServer(message);
		shutdown(sock, SHUT_RDWR);
	}
	printf("(thread) Fin affichage jeu\n");
	pthread_exit(NULL);
}

void fctThreadEcoute(){
	int i=0;
	int argCheck;
	int sizeMessageServer;
	char* affich[512];
	char* recvBuffer[1000]; // 1000 caractères max sont reçus du serveur

	while(1){
		argCheck = 0;
		sizeMessageServer=0;
		memset(affich, 0, sizeof affich);
		memset(recvBuffer, 0, sizeof recvBuffer);
		if((sizeMessageServer = recv(sock, recvBuffer, sizeof recvBuffer, 0)) <= 0){
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
			if((argCheck=checkArguments(split[1]))==0) goto argError; // Vérifie que les arguments sont !=NULL
			if(username!=NULL){
				printf("J'ai déjà un username\n");
				continue;
			}
			username = split[1];
			sprintf(affich,"Bienvenue %s",username);
			destroyPageConnexion();
			if(pthread_create(&threadFenetreAttente, NULL, fctThreadFenetreAttente, NULL)) {
				perror("pthread_create");
				return EXIT_FAILURE;
			}
			addMessageServerPageAttente(affich);

		} else if(!strcmp(prot,"USERNAME_ALREADY_USED")){
			if((argCheck=checkArguments(split[1]))==0) goto argError;
			sprintf(affich,"%s déjà utilisé !",split[1]);
			changeLabelPageConnexion(affich);

		} else if(!strcmp(prot,"CONNECTE")){
			if((argCheck=checkArguments(split[1]))==0) goto argError;
			sprintf(affich,"[serveur] : %s vient de se connecter",split[1]);
			addMessageServerPageAttente(affich);
			// TODO: addMessageServerPageJeu(affich);

		} else if(!strcmp(prot,"WAIT")){
			sprintf(affich,"[serveur] : Une partie est en cours.\n"
					"Veuillez attendre la fin du tour.");
			addMessageServerPageAttente(affich);

		} else if(!strcmp(prot,"LE_SAVIEZ_VOUS")){
			if((argCheck=checkArguments(split[1]))==0) goto argError;
			sprintf(affich,"[serveur] : Le saviez-vous ?\n%s",split[1]);
			addMessageServerPageAttente(affich);

		} else if(!strcmp(prot,"START_TIME_SESSION")){
			if((argCheck=checkArguments(split[1]))==0) goto argError;
				sprintf(affich,"[serveur] : La partie commence dans %s sec...",split[1]);
				addMessageServerPageAttente(affich);

		} else if(!strcmp(prot,"START_CANCEL_SESSION")){
			if((argCheck=checkArguments(split[1]))==0) goto argError;
				sprintf(affich,"[serveur] : Plus assez de joueurs. Partie annulée.",split[1]);
				addMessageServerPageAttente(affich);

		} else if(!strcmp(prot,"SORTI")){
			if((argCheck=checkArguments(split[1]))==0) goto argError;
				sprintf(affich,"[serveur] : Nous avons un rage quit de la part de %s",split[1]);
				addMessageServerPageAttente(affich);
				// TODO: addMessageServerPageJeu(affich);

		} else if(!strcmp(prot,"SESSION")){
			if((argCheck=checkArguments(split[1]))==0) goto argError;
			sprintf(affich,"[serveur] : La partie commence !");
			addMessageServerPageAttente(affich);
			sleep(1);
			destroyPageAttente();
			if(pthread_create(&threadFenetreJeu, NULL, fctThreadFenetreJeu, split[1])) {
				perror("pthread_create");
				return EXIT_FAILURE;
			}

		} else if(!strcmp(prot,"VAINQUEUR")){
			if((argCheck=checkArguments(split[1]))==0) goto argError;
			sprintf(affich,"Félicitation a %s, vainqueur de cette session !",split[1]);
			// TODO: addMessageServerPageJeu(affich);
			destroyPageJeu();
			if(pthread_create(&threadFenetreAttente, NULL, fctThreadFenetreAttente, NULL)) {
				perror("pthread_create");
				return EXIT_FAILURE;
			}

		} else if(!strcmp(prot,"TOUR")){
			if((argCheck=checkArguments(split[1], split[2]))==0) goto argError;
			//TODO: startReflexion(split[1], split[2]);

		} else if(!strcmp(prot,"TUASTROUVE")){
			sprintf(affich,"Tu es le premier a avoir trouve !");
		} else if(!strcmp(prot,"ILATROUVE")){
			sprintf(affich,"%s a trouve une solution en %s coups",split[1],split[2]); // ???? en X coups
		} else if(!strcmp(prot,"FINREFLEXION")){
			sprintf(affich,"Temps imparti finit, fin de la phase de reflexion !");

			//ENCHERE
		} else if(!strcmp(prot,"TUENCHERE")){
			sprintf(affich,"Enchere accepte !");
		} else if(!strcmp(prot,"ECHECENCHERE")){
			sprintf(affich,"Votre enchere est incoherente a cause de celle de %s qui a encherit en %s",split[1],split[2]);// ???? en X coups
		} else if(!strcmp(prot,"ILENCHERE")){
			sprintf(affich,"%s a effectuer une enchere en %s coups.",split[1],split[2]);// ???? en X coups
		} else if(!strcmp(prot,"FINENCHERE")){
			if(split[1]!=NULL){
				sprintf(affich,"Les encheres sont finit le vainqueur est %s en %s coups.",split[1],split[2]); // ???? en X coups
			} else{
				sprintf(affich,"Les encheres sont finit il n'y a aucun vainqueur !");
			}
			//RESOLUTION
		} else if(!strcmp(prot,"SASOLUTION")){
			sprintf(affich,"La solution de %s est : %s",split[1],split[2]);
		} else if(!strcmp(prot,"BONNE")){
			sprintf(affich,"La solution est bonne, préparez-vous aux prochains tour !");
		} else if(!strcmp(prot,"MAUVAISE")){
			if(split[1]!=NULL){
				sprintf(affich,"La solution est mauvaise, au tour de %s" ,split[1]);
			} else{
				sprintf(affich,"La solution est mauvaise !");
			}
		} else if(!strcmp(prot,"FINRESO")){
			sprintf(affich,"Phase de resolution finit !");
		} else if(!strcmp(prot,"TROPLONG")){
			if(split[1]!=NULL){
				sprintf(affich,"Temps de resolution finit, au tour de %s" ,split[1]);
			} else{
				sprintf(affich,"Temps de resolution finit !");
			}
		} else {
			sprintf(affich,"Commande \"%s\" inconnue.", prot);
		}

		argError:
		if(argCheck==0) printf("Arguments de %s incomplets !!\n", prot);
		puts(affich);
	}

	printf("Fin d'écoute du server\n");
	pthread_exit(NULL);
}

// Retourne 1 si tous les arguments sont !=NULL
int checkArguments(char* arg, ...){
	int i;
	va_list valist;

	/* initialize valist for arg number of arguments */
	va_start(valist, arg);

	/* access all the arguments assigned to valist */
	for (i = 0; i < arg; i++) {
		if(va_arg(valist, char*)==NULL) return 0;
	}

	/* clean memory reserved for valist */
	va_end(valist);

	return 1;
}

void sendToServer(char* message){
	if(send(sock, message, strlen(message), 0) < 0){
		perror("send(pseudo)");
		return;
	}
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
		perror("Erreur de connexion au server");
		printf("Est-il démarré ?\nAvez-vous donné le bon IP en paramètre ?\n");
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

	printf("\nFin du client ... Bye bye ;)\n");

	return 0;
}
