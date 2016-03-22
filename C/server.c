/*
    C socket server example
*/

#include<stdio.h>
#include<string.h>    //strlen
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

int main(int argc , char *argv[])
{
    typedef int socklen_t;

    
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[256];
    strcpy(client_message,"");

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 0);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    //Receive a message from client
    while( (read_size = recv(client_sock , client_message , 256 , 0)) > 0 )
    {
        printf("Message recu par le client %s \n",client_message);
        //Send the message back to client
        puts("avant le write");
        char *bienv="BIENVENUE/fefe/";
        int len=strlen(bienv);
        
        write(client_sock , bienv,strlen("BIENVENUE/fefe/"));
        puts("apres le write");
        strcpy(client_message, "");
    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    return 0;
}
