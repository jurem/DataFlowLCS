/*
 * LCSCpuCode.c
 *
 *  Created on: Jun 26, 2015
 *      Author: demo
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <getopt.h>
#include <math.h>

#include "common.h"
#include "formats.h"
#include "lcscpu.h"

#include "Maxfiles.h"


// default values
#define LENX	1000
#define YXRATIO	0.1


void help(const char * cmd) {
	printf("Usage: %s [filename]\n", cmd);
	printf("\nOptions:\n");
	printf("  -h, --help\n\tPrint short help\n");
	printf("  --info\n\tPrint basic information\n");
	printf("  -i filename ,--input filename \n\tRead the input from a file\n");
	printf("  -l size, --len size \n\tGenerate a problem of size (the goal string) \n");
	printf("  -r ratio, --ratio ratio \n\tSet the ratio len(pattern)/len(goal), the default is %g \n", YXRATIO);
	printf("  -s seed, --seed seed \n\tSet the random seed for input generation\n");
	printf("  -2, --2bits \n\tUse two bits instead of a byte for a character.\n");
	printf("  -d,  --dfe \n\tRun the program on DFE (currently not implemented)\n");
	printf("  -t level, --trace level\n\tTrace level: 0,1,2\n");
}


void info() {
	printf("LCS solver\n");
}


// show help
int help_flag = 0;

// info flag
int info_flag = 0;

// read input from file flag
char *filename = 0;

// size of longest string x
int lenx = LENX;

// ratio between len(y)/len(x) in interval [0, 1]
float yxratio = YXRATIO;

// random seed, if -1 time is used
int rseed = -1;

// use 2 bits as the alphabet
int b2 = 0;

// run on DFE flag (default is CPU)
int on_dfe = 0;

// trace level
int trace = 0;


struct option options[] = {
	{ "help", no_argument, &help_flag, 1 },
	{ "info", no_argument, &info_flag, 1 },
    { "input", required_argument, 0, 'i' },
	{ "len", required_argument, 0, 'l' },
	{ "ratio", required_argument, 0, 'r' },
	{ "seed", required_argument, 0, 's' },
	{ "2bits", no_argument, &b2, 1 },
	{ "dfe", no_argument, &on_dfe, 1 },
	{ "trace", required_argument, 0, 't' },
	{ 0, 0, 0, 0 }
};


#define OPTS "hd2i:l:r:s:t:"


void parse_args(int argc, char * argv[]) {
 	// options
	while (1) {
		int option_index = 0;
		int opt = getopt_long(argc, argv, OPTS, options, &option_index);
		if (opt <= 0) break;
		switch (opt) {
			case 'h':
				help_flag = 1;
				break;
			case 'i':
				filename = optarg;
				break;
			case 'l':
				lenx = atoi(optarg);
				break;
			case 'r':
				yxratio = atof(optarg);
				break;
			case 's':
				rseed = atoi(optarg);
				break;
			case '2':
				b2 = 1;
				break;
			case 'd':
				on_dfe = 1;
				break;
			case 't':
				trace = atoi(optarg);
				break;
			case '?':
				error(1, "Invalid option '%c'", optopt);
			default:
				abort();
		}
	}
    // filename as the first arguments
    if (optind < argc) filename = argv[optind++];
	// help?
	if (help_flag) {
		help(argv[0]);
		exit(0);
	}
	if (info_flag) {
		info();
		exit(0);
	}
}


int lcs_dfe(char* x, int lenx, char* y, int leny) {
	int lenz = lenx * leny;
	uint8_t* z = malloc(lenz * sizeof(uint8_t));

	LCS(xlen, ylen, (uint8_t*)x, (uint8_t*)y, z);

	return z[0];
}


int main(int argc, char * argv[]) {
	// parse command line arguments
	parse_args(argc, argv);
	// define input strings
	char *x;
	char *y;
	int leny = yxratio * lenx;
	
	if (filename != 0)
		load_lcs(filename, &x, &lenx, &y, &leny);
	else {
		srand((rseed != -1) ? rseed: time(NULL));
		random_lcs(&x, lenx, &y, leny, b2);
	}

	if (trace >= 1) {
		printf("lenx = %d\nleny = %d\nratio = %g\n", lenx, leny, yxratio);
	}
	if (trace >= 2) {
		println_lcs_8("x: ", x);
		println_lcs_8("y: ", y);
	}

	// solve lcs
	int lcs_length = 0;
	timing_t timer;
	timer_start(&timer);
	if (b2) {
		lenx *= 4;
		leny *= 4;
		if (on_dfe)
			lcs_length = lcs_dfe(x, lenx, y, leny);
		else
			lcs_length = lcs_cpu_2bits(x, lenx, y, leny);
	} else {
		if (on_dfe)
			lcs_length = lcs_dfe(x, lenx, y, leny);
		else
			lcs_length = lcs_cpu(x, lenx, y, leny);
	}
	timer_stop(&timer);

	printf("%d %d %d %ld %ld\n", lenx, leny, lcs_length, timer.realtime, timer.cputime);
	return 0;
}
