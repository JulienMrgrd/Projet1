#include "client.h"

typedef int SOCKET;
SOCKET sock;

pthread_t threadEcoute;
pthread_t threadFenetreConnexion;
pthread_t threadFenetreAttente;
pthread_t threadFenetreJeu;
char* username;


void fctThreadFenetreConnexion(){
	printf("fctThreadFenetreConnexion\n");
	if(startPageConnexion()==1){
		printf("Bouton \"X\" cliqué !\n");
		quit();
		shutdown(sock, SHUT_RDWR);
	}
	printf("(thread) Fin affichage connexion\n");
	pthread_exit(NULL);
}

void fctThreadFenetreAttente(){
	printf("fctThreadFenetreAttente\n");
	sleep(1);
	if(startPageAttente(username)==1){
		printf("Bouton \"X\" cliqué !\n");
		quit();
		shutdown(sock, SHUT_RDWR);
	}
	printf("(thread) Fin affichage d'attente\n");
	pthread_exit(NULL);
}

void fctThreadFenetreJeu(char* plateau){
	printf("fctThreadFenetreConnexion\n");
	if(startPageJeu(plateau)==1){
		printf("Bouton \"X\" cliqué !\n");
		quit();
		shutdown(sock, SHUT_RDWR);
	}
	printf("(thread) Fin affichage jeu\n");
	pthread_exit(NULL);
}

void fctThreadEcoute(){
	int isProtInconnu = 0;
	int i=0;
	int argCheck;
	int sizeMessageServer;
	char* affich[512];
	char* recvBuffer[1000]; // 1000 caractères max sont reçus du serveur

	while(1){
		isProtInconnu = 0;
		argCheck = 0;
		sizeMessageServer=0;
		memset(affich, 0, sizeof affich);
		memset(recvBuffer, 0, sizeof recvBuffer);
		if((sizeMessageServer = recv(sock, recvBuffer, sizeof recvBuffer, 0)) <= 0){
			puts("Serveur déconnecté ...\n");
			break;
		} else {
			printf("Message reçu : %s\n", recvBuffer);
		}

		char** split = splitWithChar(recvBuffer, '/');
		if(sizeMessageServer==2){
			printf("Message reçu vide\n");
			continue;
		}

		for (i=0; (split[i] != NULL && isNonPrintable(split[i])==0) ; i++) {
			printf("Resultat %d = %s\n", i, split[i]);
		}

		char* prot = split[0];
		char* argOne = split[1];

		if(!strcmp(prot,"BIENVENUE")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError; // Vérifie que les arguments sont !=NULL
			if(username!=NULL){
				printf("J'ai déjà un username\n");
				continue;
			}
			username = strdup(split[1]);
			printf("Bienvenue %s",username);
			sprintf(affich,"Bienvenue %s",username);
			destroyPageConnexion();
			if(pthread_create(&threadFenetreAttente, NULL, fctThreadFenetreAttente, NULL)) {
				perror("pthread_create");
				return EXIT_FAILURE;
			}

		} else if(!strcmp(prot,"USERNAME_ALREADY_USED")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;
			sprintf(affich,"%s déjà utilisé !",argOne);
			changeLabelPageConnexion(affich);

		} else if(!strcmp(prot,"CONNECTE")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;
			sprintf(affich,"[serveur] : %s vient de se connecter",argOne);
			addMessageServerPageAttente(affich);
			// TODO: addMessageServerPageJeu(affich);

		} else if(!strcmp(prot,"WAIT")){
			sprintf(affich,"[serveur] : Une partie est en cours.\n"
					"Veuillez attendre la fin du tour.");
			addMessageServerPageAttente(affich);

		} else if(!strcmp(prot,"LE_SAVIEZ_VOUS")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;
			sprintf(affich,"[serveur] : Le saviez-vous ?\n%s", argOne);
			addMessageServerPageAttente(affich);

		} else if(!strcmp(prot,"START_TIME_SESSION")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;
			sprintf(affich,"[serveur] : La partie commence dans %s sec...",argOne);
			addMessageServerPageAttente(affich);

		} else if(!strcmp(prot,"START_CANCEL_SESSION")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;
			sprintf(affich,"[serveur] : Plus assez de joueurs. Partie annulée.",argOne);
			addMessageServerPageAttente(affich);

		} else if(!strcmp(prot,"SORTI")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;
			sprintf(affich,"[serveur] : Nous avons un rage quit de la part de %s",argOne);
			addMessageServerPageAttente(affich);
			// TODO: addMessageServerPageJeu(affich);

		} else if(!strcmp(prot,"SESSION")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;
			sprintf(affich,"[serveur] : La partie commence !");
			addMessageServerPageAttente(affich);
			sleep(1);
			destroyPageAttente();
			if(pthread_create(&threadFenetreJeu, NULL, fctThreadFenetreJeu, argOne)) {
				perror("pthread_create");
				return EXIT_FAILURE;
			}

		} else if(!strcmp(prot,"VAINQUEUR")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;
			sprintf(affich,"[serveur] : Félicitation a %s, vainqueur de cette session !",argOne);
			// TODO: addMessageServerPageJeu(affich);
			destroyPageJeu();
			if(pthread_create(&threadFenetreAttente, NULL, fctThreadFenetreAttente, NULL)) {
				perror("pthread_create");
				return EXIT_FAILURE;
			}

		} else if(!strcmp(prot,"TOUR")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;  // enigme
			if((argCheck=checkOneArgument(split[2]))==0) goto argError; // bilan
			//TODO: startReflexion(argOne, split[2]);

		} else if(!strcmp(prot,"TUASTROUVE")){
			sprintf(affich,"[serveur] : Tu es le premier à avoir trouvé !");
			//addMessageServerPageJeu(affich);
		} else if(!strcmp(prot,"ILATROUVE")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;
			sprintf(affich,"[serveur] : %s a trouve une solution en %s coups",argOne,split[2]); // ???? en X coups
			//addMessageServerPageJeu(affich);
		} else if(!strcmp(prot,"FINREFLEXION")){
			sprintf(affich,"[serveur] : Temps imparti fini. Fin de la phase de reflexion !");
			//addMessageServerPageJeu(affich);

		} else if(!strcmp(prot,"TUENCHERE")){
			sprintf(affich,"[serveur] : Enchere accepte !");
		} else if(!strcmp(prot,"ECHECENCHERE")){
			sprintf(affich,"[serveur] : Votre enchere est incoherente a cause de celle de %s qui a encherit en %s",argOne,split[2]);// ???? en X coups
		} else if(!strcmp(prot,"ILENCHERE")){
			sprintf(affich,"[serveur] : %s a effectuer une enchere en %s coups.",argOne,split[2]);// ???? en X coups
		} else if(!strcmp(prot,"FINENCHERE")){
			if(argOne!=NULL){
				sprintf(affich,"[serveur] : Les encheres sont finies, le meilleur est %s en %s coups.",argOne,split[2]); // ???? en X coups
			} else{
				sprintf(affich,"[serveur] : Les encheres sont finies, il n'y a aucun vainqueur !");
			}

		} else if(!strcmp(prot,"SASOLUTION")){
			sprintf(affich,"[serveur] : La solution de %s est : %s",argOne,split[2]);
		} else if(!strcmp(prot,"BONNE")){
			sprintf(affich,"[serveur] : La solution est bonne !");
		} else if(!strcmp(prot,"MAUVAISE")){
			if(argOne!=NULL){
				sprintf(affich,"[serveur] : La solution est mauvaise, au tour de %s" ,argOne);
			} else{
				sprintf(affich,"[serveur] : La solution est mauvaise !");
			}
		} else if(!strcmp(prot,"FINRESO")){
			sprintf(affich,"[serveur] : Phase de resolution finie !");
		} else if(!strcmp(prot,"TROPLONG")){
			if(argOne!=NULL){
				sprintf(affich,"[serveur] : Temps de resolution écoulé, au tour de %s" ,argOne);
			} else{
				sprintf(affich,"[serveur] : Temps de resolution écoulé !");
			}
		} else {
			isProtInconnu = 1;
			sprintf(affich,"Commande \"%s\" inconnue.\n", prot);
		}

		argError: // goto
		if(argCheck==0 && isProtInconnu==0) printf("Arguments de %s incomplets !!\n", prot);
		puts(affich);

	}

	printf("Fin d'écoute du server\n");
	pthread_exit(NULL);
}

// Retourne 0 si l'argument est invalide (null ou non printable)
int checkOneArgument(char* param){
	if (param==NULL || isNonPrintable(param)) return 0;
	return 1;
}

void sendToServer(char* message){
	if(send(sock, message, strlen(message), 0) < 0){
		perror("send(pseudo)");
		return;
	}
}

void quit(){
	char* message;
	strcpy(message,"SORT/");
	strcat(message, username);
	sendToServer(message);
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
	gtk_init(&argc, &argv);

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
