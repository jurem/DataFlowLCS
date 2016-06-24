#ifndef FORMATS_H
#define FORMATS_H



void println_lcs_8(char *msg, char *x);

void println_lcs_2(char *msg, char *x);


void random_lcs_8(char **x, int lenx, char **y, int leny);

// lenx and leny must be divisible by 4
void random_lcs_2(char **x, int lenx, char **y, int leny);

void random_lcs(char **x, int lenx, char **y, int leny, int b2);

void load_lcs(const char *filename, char **x, int *lenx, char **y, int *leny);


#endif
