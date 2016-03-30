#include <pthread.h>
#include <stdarg.h>
#include "pageConnexion.h"
#include "pageAttente.h"
#include "pageJeu.h"
#include "utils.h"

extern char* username;

void fctThreadFenetreConnexion();
void fctThreadFenetreAttente();
void fctThreadFenetreJeu(char* plateau);
void fctThreadEcoute();
void sendToServer(char* message);
int checkArguments(char* arg, ...);
