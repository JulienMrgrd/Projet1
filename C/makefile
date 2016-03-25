GTKFLAGS=-export-dynamic `pkg-config --cflags --libs gtk+-2.0`

all: clean client2 server

client: pagePrincipal.h 
	gcc -c client.c pagePrincipal.c  $(GTKFLAGS) -lpthread

	
client2: client
	gcc -o client client.o pagePrincipal.o $(GTKFLAGS) -lpthread
	
	
server: 
	gcc -o server server.c
	

clean:
	rm -rf *.o client server
