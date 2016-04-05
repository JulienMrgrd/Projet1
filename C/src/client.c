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
		quit(); // Envoie SORT/user
	}
	printf("(thread) Fin affichage connexion\n");
	threadFenetreConnexion = NULL;
	pthread_exit(NULL);
}

void fctThreadFenetreAttente(){
	printf("fctThreadFenetreAttente\n");
	sleep(1);
	if(startPageAttente(username)==1){
		printf("Bouton \"X\" cliqué !\n");
		quit();
	}
	printf("(thread) Fin affichage d'attente\n");
	threadFenetreAttente = NULL;
	pthread_exit(NULL);
}

void fctThreadFenetreJeu(char* plateau){
	printf("fctThreadFenetreJeu\n");
	if(startPageJeu(plateau,username)==1){
		printf("Bouton \"X\" cliqué !\n");
		quit();
	}
	printf("(thread) Fin affichage jeu\n");
	threadFenetreJeu = NULL;
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
		argCheck = 1;
		sizeMessageServer=0;
		memset(affich, 0, sizeof affich);
		memset(recvBuffer, 0, sizeof recvBuffer);
		if((sizeMessageServer = recv(sock, recvBuffer, sizeof recvBuffer, 0)) <= 0){
			puts("Serveur déconnecté ...\n");
			break;
		} else {
			if(strstr(recvBuffer, "PING") || sizeMessageServer < 3 || isNonPrintable(recvBuffer)){
				continue; // le cas des ping
			}
			printf("Message reçu : %s\n", recvBuffer);
		}

		char** split = splitWithChar(recvBuffer, '/');
		for (i=0; (split[i] != NULL && isNonPrintable(split[i])==0) ; i++) {
			printf("Resultat %d = %s\n", i, split[i]);
		}

		char* prot = split[0];
		char* argOne = split[1];
		char* argTwo = split[2];

		if(!strcmp(prot,"BIENVENUE")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError; // Vérifie que les arguments sont !=NULL
			if(username!=NULL){
				printf("J'ai déjà un username\n");
				continue;
			}
			username = strdup(argOne);
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
			addMessageServerPageJeu(affich);

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

		} else if(!strcmp(prot,"DECONNEXION")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;
			sprintf(affich,"[serveur] : Nous avons un rage quit de la part de %s",argOne);
			addMessageServerPageAttente(affich);
			addMessageServerPageJeu(affich);

		} else if(!strcmp(prot,"SESSION")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;
			destroyPageAttente();
			if(pthread_create(&threadFenetreJeu, NULL, fctThreadFenetreJeu, argOne)) {
				perror("pthread_create");
				return EXIT_FAILURE;
			}
			printf("after session creation fenetre jeu\n");

		} else if(!strcmp(prot,"VAINQUEUR")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;
			sprintf(affich,"[serveur] : Félicitation a %s, vainqueur de cette session !",argOne);
			addMessageServerPageJeu(affich);
			// displayVainqueur(argOne);
			sleep(2);
			destroyPageJeu();
			if(	threadFenetreAttente==NULL ){
				if(pthread_create(&threadFenetreAttente, NULL, fctThreadFenetreAttente, NULL)) {
					perror("pthread_create");
					return EXIT_FAILURE;
				}
			}

		} else if(!strcmp(prot,"TOUR")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;  // enigme
			if((argCheck=checkOneArgument(argTwo))==0) goto argError; // bilan
			sprintf(affich,"[serveur] : Nouveau tour !");
			addMessageServerPageJeu(affich);
			sleep(1);
			startReflexion(argOne, argTwo);

		} else if(!strcmp(prot,"TUASTROUVE")){
			sprintf(affich,"[serveur] : Tu es le premier à avoir trouvé !");
			addMessageServerPageJeu(affich);
			meilleureProposition(username, NULL);
			setPhase("ENCHERE");

		} else if(!strcmp(prot,"ILATROUVE")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;
			if((argCheck=checkOneArgument(argTwo))==0) goto argError;
			sprintf(affich,"[serveur] : %s a trouve une solution en %s coups",argOne,argTwo); // ???? en X coups
			addMessageServerPageJeu(affich);
			meilleureProposition(argOne, argTwo);
			setPhase("ENCHERE");

		} else if(!strcmp(prot,"FINREFLEXION")){
			sprintf(affich,"[serveur] : Temps imparti fini. Fin de la phase de reflexion !");
			addMessageServerPageJeu(affich);
			setPhase("ENCHERE");

		} else if(!strcmp(prot,"VALIDATION")){
			meilleureProposition(username, NULL);
			sprintf(affich,"[serveur] : Enchere acceptee !");
			addMessageServerPageJeu(affich);

		} else if(!strcmp(prot,"ECHEC")){
			sprintf(affich,"[serveur] : Votre enchere est incoherente a cause de celle de %s qui a encherit en %s",argOne,argTwo);// ???? en X coups
			addMessageServerPageJeu(affich);

		} else if(!strcmp(prot,"NOUVELLEENCHERE")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;
			if((argCheck=checkOneArgument(argTwo))==0) goto argError;
			sprintf(affich,"[serveur] : %s a effectué une enchere en %s coups.",argOne,argTwo);// ???? en X coups
			meilleureProposition(argOne, argTwo);
			addMessageServerPageJeu(affich);

		} else if(!strcmp(prot,"FINENCHERE")){
			if((argCheck=checkOneArgument(argOne))!=0 && (argCheck=checkOneArgument(argTwo))!=0 ){
				//auTourDe(argOne);
				sprintf(affich,"[serveur] : Les encheres sont finies, le meilleur est %s en %s coups.",argOne,argTwo); // ???? en X coups
			} else{
				sprintf(affich,"[serveur] : Les encheres sont finies, il n'y a aucun vainqueur !");
			}
			addMessageServerPageJeu(affich);
			setPhase("RESOLUTION");

		} else if(!strcmp(prot,"SASOLUTION")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;  // user
			if((argCheck=checkOneArgument(argTwo))==0) goto argError; // solution
			sprintf(affich,"[serveur] : La solution de %s est : %s",argOne,argTwo);
			addMessageServerPageJeu(affich);

		} else if(!strcmp(prot,"BONNE")){
			sprintf(affich,"[serveur] : La solution est bonne !");
			addMessageServerPageJeu(affich);

		} else if(!strcmp(prot,"MAUVAISE")){
			if((argCheck=checkOneArgument(argOne))!=0){
				//auTourDe(argOne);
				setPhase("RESOLUTION");
				sprintf(affich,"[serveur] : La solution est mauvaise, au tour de %s" ,argOne);
			} else {
				sprintf(affich,"[serveur] : La solution est mauvaise !");
			}
			addMessageServerPageJeu(affich);

		} else if(!strcmp(prot,"FINRESO")){
			sprintf(affich,"[serveur] : Phase de resolution finie !");
			addMessageServerPageJeu(affich);

		} else if(!strcmp(prot,"TROPLONG")){
			if((argCheck=checkOneArgument(argOne))!=0){
				//auTourDe(argOne);
				setPhase("RESOLUTION");
				sprintf(affich,"[serveur] : Temps de resolution écoulé, au tour de %s" ,argOne);
			} else{
				sprintf(affich,"[serveur] : Temps de resolution écoulé !");
			}
			addMessageServerPageJeu(affich);

		} else if(!strcmp(prot,"CHAT")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;  // user
			if((argCheck=checkOneArgument(argTwo))==0) goto argError; // message

			if(argOne==username) sprintf(affich,"%s", argTwo); // Si le message vient de moi
			else sprintf(affich,"[%s] : %s", argOne, argTwo);
			addMessageServerPageJeu(affich);

		} else if(!strcmp(prot,"BEFORE_BAN")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;  // message
			sprintf(affich,"[serveur] : %s" ,argOne);

		} else if(!strcmp(prot,"BANNI")){
			if((argCheck=checkOneArgument(argOne))==0) goto argError;  // user
			if((argCheck=checkOneArgument(argTwo))==0) goto argError;  // messageServer

			if(user==username) quit();
			else sprintf(affich,"[serveur] : %s" ,argTwo);

		} else {
			isProtInconnu = 1;
			sprintf(affich,"Commande \"%s\" inconnue.\n", prot);
		}

		argError: // goto
		if(argCheck==0 && isProtInconnu==0) printf("Arguments de %s incomplets !!\n", prot);
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
	shutdown(sock, SHUT_RDWR); // Coupe le thread d'écoute, et donc termine le programme
}

int main(int argc, char* argv[]){
	sock = socket(AF_INET , SOCK_STREAM , 0);

	if (sock == INVALID_SOCKET){
		printf("Socket non crée\n");
	}

	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_family = AF_INET;
	server.sin_port = htons(2016);

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
