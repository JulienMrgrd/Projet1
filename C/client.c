/*
    C ECHO client example using sockets
 */
#include "pageConnexion.h"
#include "utils.h"

typedef int SOCKET;
SOCKET sock;

int main(int argc, char* argv[]){
	int i;
	char proto[]="TROUVE/user/5";
	char** res = split(proto, '/');
	puts(proto);
	for (i=0; res[i] != NULL; i++) {
		printf("Resultat %d = %s\n", i, res[i]);
	}

	return 0;
	
	/*
    sock = socket(AF_INET , SOCK_STREAM , 0);

    if (sock == INVALID_SOCKET){
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    //puts(server.sin_addr.s_addr);

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR){
    	perror("connect()");
    	return;
    }

    affichageConnexion();

    return 0;
	 */
}
