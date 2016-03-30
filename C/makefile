# Les différents répertoires contenant respectivement les fichiers : Sources *.c, Headers *.h, Objets *.o, l'exécutable
SRCDIR=src
HEADDIR=include
LIBDIR=objet
 
 
# Les différentes options de compilations, soyons strictes !
CFLAGS= -I$(HEADDIR) -w
# Les différents FrameWorks et bibliothèques pour le linkage
GTKFLAGS=-export-dynamic `pkg-config --cflags --libs gtk+-3.0`
 
# L'exécutable
BIN=client

all: clean utils pageConnexion pageAttente pageJeu client

utils:
	gcc -c $(SRCDIR)/utils.c -o $(LIBDIR)/utils.o $(CFLAGS) 
	
pageConnexion:
	gcc -c $(SRCDIR)/pageConnexion.c -o $(LIBDIR)/pageConnexion.o $(CFLAGS) $(GTKFLAGS)
	
pageAttente: 
	gcc -c $(SRCDIR)/pageAttente.c -o $(LIBDIR)/pageAttente.o $(CFLAGS) $(GTKFLAGS)
	
pageJeu:
	gcc -c $(SRCDIR)/pageJeu.c -o $(LIBDIR)/pageJeu.o $(CFLAGS) $(GTKFLAGS)
	
client2:
	gcc -c $(SRCDIR)/client.c -o $(LIBDIR)/client.o $(CFLAGS) $(GTKFLAGS)
	
$(BIN): client2
	gcc -o client $(LIBDIR)/pageConnexion.o $(LIBDIR)/pageAttente.o $(LIBDIR)/pageJeu.o $(LIBDIR)/client.o $(LIBDIR)/utils.o $(CFLAGS) $(GTKFLAGS)

# Nettoyage => Tout sera recompiler !
clean:
	rm -rf $(LIBDIR)/*.o client
