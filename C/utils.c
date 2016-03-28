#include <stdlib.h>
#include <string.h>
#include "utils.h"

/*const char* Protocole_Strings[] = {"CONNEX","BIENVENUE","CONNECTE","SORT","SORTI","SESSION","VAINQUEUR",
		"TOUR","TROUVE","TUASTROUVE","ILATROUVE","FINREFLEXION","ENCHERE","TUENCHERE","ECHECENCHERE","ILENCHERE",
		"FINENCHERE","SOLUTION","SASOLUTION","BONNE","MAUVAISE","FINRESO","TROPLONG","USERNAME_ALREADY_USED","UNKNOWN_CMD",
		"BAD_PARAMETERS","BYE","WAIT","START_TIME_SESSION","START_CANCEL_SESSION","LE_SAVIEZ_VOUS"};
*/
protocole getProtocoleByString(char *str){
	if (strcmp(str,"CONNEX")==0) {
	     return CONNEX;
	} else if (strcmp(str,"BIENVENUE")==0) {
	     return BIENVENUE;
	} else if (strcmp(str,"CONNECTE")==0) {
	     return CONNECTE;
	} else if (strcmp(str,"SORT")==0) {
	     return SORT;
	} else if (strcmp(str,"SORTI")==0) {
	     return SORTI;
	} else if (strcmp(str,"SESSION")==0) {
	     return SESSION;
	} else if (strcmp(str,"VAINQUEUR")==0) {
	     return VAINQUEUR;
	} else if (strcmp(str,"TOUR")==0) {
	     return TOUR;
	} else if (strcmp(str,"TROUVE")==0) {
	     return TROUVE;
	} else if (strcmp(str,"TUASTROUVE")==0) {
	     return TUASTROUVE;
	} else if (strcmp(str,"ILATROUVE")==0) {
	     return ILATROUVE;
	} else if (strcmp(str,"FINREFLEXION")==0) {
	     return FINREFLEXION;
	} else if (strcmp(str,"ENCHERE")==0) {
	     return ENCHERE;
	} else if (strcmp(str,"TUENCHERE")==0) {
	     return TUENCHERE;
	} else if (strcmp(str,"ECHECENCHERE")==0) {
	     return ECHECENCHERE;
	} else if (strcmp(str,"ILENCHERE")==0) {
	     return ILENCHERE;
	} else if (strcmp(str,"FINENCHERE")==0) {
	     return FINENCHERE;
	} else if (strcmp(str,"SOLUTION")==0) {
	     return SOLUTION;
	} else if (strcmp(str,"SASOLUTION")==0) {
	     return SASOLUTION;
	} else if (strcmp(str,"BONNE")==0) {
	     return BONNE;
	} else if (strcmp(str,"MAUVAISE")==0) {
	     return MAUVAISE;
	} else if (strcmp(str,"FINRESO")==0) {
	     return FINRESO;
	} else if (strcmp(str,"TROPLONG")==0) {
	     return TROPLONG;
	} else if (strcmp(str,"USERNAME_ALREADY_USED")==0) {
	     return USERNAME_ALREADY_USED;
	} else if (strcmp(str,"UNKNOWN_CMD")==0) {
	     return UNKNOWN_CMD;
	} else if (strcmp(str,"BAD_PARAMETERS")==0) {
	     return BAD_PARAMETERS;
	} else if (strcmp(str,"BYE")==0) {
	     return BYE;
	} else if (strcmp(str,"WAIT")==0) {
	     return WAIT;
	} else if (strcmp(str,"START_TIME_SESSION")==0) {
	     return START_TIME_SESSION;
	} else if (strcmp(str,"START_CANCEL_SESSION")==0) {
	     return START_CANCEL_SESSION;
	} else if (strcmp(str,"LE_SAVIEZ_VOUS")==0) {
	     return LE_SAVIEZ_VOUS;
	}
	return UNKNOWN_CMD;
}

char ** splitWithString(char *str, char* separator) {
	return;
}

char **splitWithChar(char *str, char separator) {
	char **res = NULL;
	int  nbstr = 1;
	int  len;
	int  from = 0;
	int  i;
	int  j;
	res = (char **) malloc(sizeof (char *));
	len = strlen(str);
	for (i = 0; i <= len; ++i) {
		if ((i == len) || (str[i] == separator)) {
			res = (char **) realloc(res, ++nbstr * sizeof (char *));
			res[nbstr - 2] = (char *) malloc((i - from + 1) * sizeof (char));
			for (j = 0; j < (i - from); ++j)
				res[nbstr - 2][j] = str[j + from];
			res[nbstr - 2][i - from] = '\0';
			from = i + 1;
			++i;
		}
	}
	res[nbstr - 1] =  NULL;
	return res;
}
