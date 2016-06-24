#include <stdlib.h>


int lcs_cpu(char *x, int lenx, char *y, int leny) {
	int *l = calloc(leny, sizeof(int));

	for (int i = 0; i < lenx; i++) {
		int curr_lp = 0;
		int curr_zp = 0;
		for (int j = 0; j < leny; j++) {
			if (x[i] == y[j]) {
				curr_zp = curr_lp + 1;
				curr_lp = l[j];
				l[j] = curr_zp;
			} else {
				curr_lp = l[j];
				curr_zp = l[j] > curr_zp ? l[j] : curr_zp;
				l[j] = curr_zp;
			}
		}
	}
	return l[leny - 1];
}


int lcs_cpu_2bits(char* x, int lenx, char* y, int leny) {
	int *l = calloc(leny, sizeof(int));
	char yb2[4];

	for (int i = 0; i < lenx; i++) {
		int curr_zp = 0;
		int curr_lp = 0;
		// extract 2 bits from x[i/4]
		char xb2 = (x[i / 4] >> (2 * (3 - i % 4))) & 0b11;
		for (int j = 0; j < leny / 4; j++) {
			char y_t = y[j / 4];
			yb2[0] = y_t & 0b11;
			yb2[1] = y_t & 0b1100;
			yb2[2] = y_t & 0b110000;
			yb2[3] = y_t & 0b11000000;
			for (int k = 0; k < 4; k++) {
				if (yb2[k] == xb2) {
					curr_zp = curr_lp + 1;
					curr_lp = l[j];
					l[j] = curr_zp;
				} else {
					curr_lp = l[j];
					curr_zp = l[j] > curr_zp ? l[j] : curr_zp;
					l[j] = curr_zp;
				}
			}
		}
	}
	return l[leny - 1];
}
