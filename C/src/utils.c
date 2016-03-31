#include <stdlib.h>
#include <string.h>
#include "utils.h"

char ** splitWithString(char *str, char* separator) {
	// TODO:
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

char *replace(char *st, char *orig, char *repl) {
	static char buffer[4096];
	char *ch;
	if (!(ch = strstr(st, orig))) return st;
	strncpy(buffer, st, ch-st);
	buffer[ch-st] = 0;
	sprintf(buffer+(ch-st), "%s%s", repl, ch+strlen(orig));
	return buffer;
}

// Retourne 1 si la chaine n'est pas printable
int isNonPrintable(char* chaine){
	if(chaine==NULL) return 1;
	char* str = chaine;
	while(*str){
		if(isprint(*str)){
			return 0;
		}
		str++;
	}
	return 1;
}
