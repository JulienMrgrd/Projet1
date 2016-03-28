#include <stdlib.h>

char **split(char *str, char separator) {
	printf("Split : str= %s , separator = %c\n", str, separator);
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
