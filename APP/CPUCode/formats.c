#include <stdlib.h>
#include <stdio.h>

#include "formats.h"
#include "common.h"


void println_lcs_8(char *msg, char *x) {
    printf("%s%s\n", msg, x);
}


void println_lcs_2(char *msg, char *x) {
    static char dna[] = { 'C', 'T', 'A', 'G' };
    while (*x != 0) {
        char a = (*x >> 6) & 0b11;
        char b = (*x >> 4) & 0b11;
        char c = (*x >> 2) & 0b11;
        char d = 0b11;
        printf("%s%c%c%c%c\n", msg, dna[a], dna[b], dna[c], dna[d]);
        x++;
    }
}


void random_lcs_8(char **x, int lenx, char **y, int leny) {
    *x = malloc((lenx + 1) * sizeof(char));
    *y = malloc((leny + 1) * sizeof(char));
    for (int i = 0; i < lenx; i++) (*x)[i] = rand_letter();
    (*x)[lenx] = 0;
    for (int i = 0; i < leny; i++) (*y)[i] = rand_letter();
    (*y)[leny] = 0;
}


void random_lcs_2(char **x, int lenx, char **y, int leny) {
    random_lcs_8(x, lenx / 4, y, leny / 4);
}


void random_lcs(char **x, int lenx, char **y, int leny, int b2) {
    if (b2) random_lcs_2(x, lenx, y, leny);
    else random_lcs_8(x, lenx, y, leny);
}


void load_lcs(const char *filename, char **x, int *lenx, char **y, int *leny) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    if (fscanf(fp, "%d %d\n", lenx, leny) != 2) {
        fclose(fp);
        printf("Error reading input file %s\n", filename);
        exit(ferror(fp));
    }
    *x = malloc(*lenx * sizeof(char));
    *y = malloc(*leny * sizeof(char));
    for (int i = 0; i < *lenx; i++) *x[i] = fgetc(fp);
    fgetc(fp); // newline
for (size_t i = 0; i < *leny; i++) *y[i] = fgetc(fp);
    fclose(fp);
}

