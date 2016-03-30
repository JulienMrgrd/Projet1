#include <string.h>
#include <stdlib.h>

typedef enum {
	// connexion
	CONNEX,
	BIENVENUE,
	CONNECTE,

	// deconnexion
	SORT,
	SORTI,

	// debut partie
	SESSION,
	VAINQUEUR,

	// Phase reflexion
	TOUR,
	TROUVE,
	TUASTROUVE,
	ILATROUVE,
	FINREFLEXION,

	// Phase d'enchere
	ENCHERE,
	TUENCHERE,
	ECHECENCHERE,
	ILENCHERE,
	FINENCHERE,

	// Phase de résolution
	SOLUTION,
	SASOLUTION,
	BONNE,
	MAUVAISE,
	FINRESO,
	TROPLONG,

	// Autres : Protocole ajouté
	USERNAME_ALREADY_USED, // Nom déjà utilisé
	UNKNOWN_CMD, // Commande inconnue
	BAD_PARAMETERS,
	BYE,
	WAIT, // indique au client d'attendre le prochain tour/lancement de la partie
	START_TIME_SESSION, // indique au client que la partie va commencer dans X secondes
	START_CANCEL_SESSION,
	LE_SAVIEZ_VOUS
} protocole;

extern protocole PROTOCOLE;

protocole getProtocoleByString(char *str);

char **splitWithString(char *str, char* separator);
char **splitWithChar(char *str, char separator);
