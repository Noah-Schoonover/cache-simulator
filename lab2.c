/*************************************************************************
*      	filename:  lab2.c
*   	description:  main function implements the cache simulator
*
*       author:  Schoonover, Noah
*
*	    class:       CSE 331
*    	instructor:  Tong
*    	assignment:  Lab #2
*       due:  	     3/21/2022
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "trace.h"
#include "cache.h"

int main(int argc, char *argv[]) {

	if (argc != 3) {

		printf("Usage: ./lab2 [config file] [trace file]\n");
      	exit(EXIT_FAILURE);

   	} else {

		if (read_config(*++argv) == CONFIG_FAILURE) {
			printf("error reading config file\n");
			exit(EXIT_FAILURE);
		}
		create_cache();

		if (begin_trace(*++argv) == TRACE_FAILURE) {
			printf("error opening trace file\n");
			exit(EXIT_FAILURE);
		}

		int lines = 1; // temp counter for testing
		// while (get_next_trace() != TRACE_END) {
		// 	lines++;
		// }
		get_next_trace();
		printf("trace file lines: %d\n", lines);


		exit(EXIT_SUCCESS);

	}


    return 0;
}
