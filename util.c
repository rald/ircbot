#include "util.h"

char *skip(char *s, char c) {
	while(*s != c && *s != '\0')
		s++;
	if(*s != '\0')
		*s++ = '\0';
	return s;
}

void trim(char *s) {
    // Trim trailing whitespace
    int i = strlen(s) - 1;
    while (i >= 0 && isspace((unsigned char)s[i])) {
        s[i] = '\0';
        i--;
    }

    // Trim leading whitespace
    int j = 0;
    while (s[j] != '\0' && isspace((unsigned char)s[j])) {
        j++;
    }
    if (j > 0) {
        memmove(s, s + j, strlen(s) - j + 1);
    }
}

