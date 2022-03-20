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
#include "log.h"
#include <string.h>

int main(int argc, char *argv[]) {

	if (argc != 3) {

		printf("Usage: ./lab2 [config file] [trace file]\n");
      	exit(EXIT_FAILURE);

   	} else {

		if (read_config(*++argv) == CONFIG_FAILURE) {
			printf("error reading config file\n");
			exit(EXIT_FAILURE);
		}

		if (create_cache() == CACHE_FAILURE) {
			exit(EXIT_FAILURE);
		}

		if (begin_trace(*++argv) == TRACE_FAILURE) {
			printf("error opening trace file\n");
			exit(EXIT_FAILURE);
		}
		trace_filename = strdup(*argv);

		while (get_next_trace() != TRACE_END) {
			if (memacc.type == STORE) { log_store(); }
			else { log_load(); }
			log_other_instructions(memacc.islma);
			query_cache();
		}

		free_cache();
		print_log(trace_filename);
		exit(EXIT_SUCCESS);

	}


    return 0;
}
