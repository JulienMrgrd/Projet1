CC=gcc
GTKFLAGS=-export-dynamic `pkg-config --cflags --libs gtk+-2.0`

all: clean utils client

utils:
	gcc -w -c utils.c -o utils.o
	
connexion: pagePrincipal
	gcc -w -c connexion.c -o connexion.o $(GTKFLAGS)
	
pageConnexion: connexion
	gcc -w -c pageConnexion.c -o pageConnexion.o $(GTKFLAGS)
	
pagePrincipal:
	gcc -w -c pagePrincipal.c -o pagePrincipal.o $(GTKFLAGS)
	
client2: pageConnexion
	gcc -w -c client.c -o client.o $(GTKFLAGS)
	
client: client2
	gcc -w -o client connexion.o pageConnexion.o pagePrincipal.o client.o utils.o $(GTKFLAGS)

clean:
	rm -rf *.o client server
