#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<gtk/gtk.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
struct sockaddr_in server;
int affichageConnexion();
 
