GTKFLAGS=-export-dynamic `pkg-config --cflags --libs gtk+-2.0`

all: clean utils client

utils:
	gcc -w -c utils.c -o utils.o

pageConnexion: pagePrincipale
	gcc -w -c pageConnexion.c -o pageConnexion.o $(GTKFLAGS)
	
pagePrincipale:
	gcc -w -c pagePrincipale.c -o pagePrincipale.o $(GTKFLAGS)
	
client2: pageConnexion
	gcc -w -c client.c -o client.o $(GTKFLAGS)
	
client: client2
	gcc -w -o client pageConnexion.o pagePrincipale.o client.o utils.o $(GTKFLAGS)

clean:
	rm -rf *.o client
