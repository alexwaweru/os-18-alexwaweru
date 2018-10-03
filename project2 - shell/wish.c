#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int scan_s(char *str, int str_len, int *s, int *e, const char delim) {
    if (*e == str_len)
        return -1;
    for (*e = *s; *e <= str_len; *e+=1) {
        if ((str[*e] == delim) || (*e == str_len)) {
            return *e - *s;
        }
    }
    return -2;
}

char *test="There is a natural language processing class happening now";
int main() {
    int str_len = strlen(test);
    int tok_len = 0, s = 0, e = 0;
    char *tok;
    char **tokens = malloc(4 * sizeof(char));
    int position = 0;
    int bufsize = 64;

    while (scan_s(test, str_len, &s, &e, ' ') >= 0 ) {
        int ss = 0, ee = 0;
        while (scan_s(test + s, e-s, &ss, &ee, ' ') >= 0 ) {
            tok = (char *) malloc((ee-ss + 1) * sizeof(char));
            char *dst = stpncpy(tok, test + s + ss, ee-ss);
            tok[ee-ss]='\0';
            tokens[position] = tok;
            position++;
            ss = ee + 1;

            if (position == bufsize){
                bufsize = bufsize + 64;
                tokens =realloc(tokens, bufsize);
            }

            if (!tokens){
                fprintf(stderr, "memory reallocation error!\n");
                exit(EXIT_FAILURE);
            }
        }
        s = e + 1;
    }

    for (int i = 0; i < position; i++){
        printf("%s\n", tokens[i]);
    }

    return 0;
}