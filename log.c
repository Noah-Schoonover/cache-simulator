/*************************************************************************
*      	filename:  log.c
*   	description:  logging functions
*
*       author:  Schoonover, Noah
*
*	    class:       CSE 331
*    	instructor:  Tong
*    	assignment:  Lab #2
*       due:  	     3/21/2022
*************************************************************************/

#include "log.h"

void log_hit() {
	log_memory_ops++;
	log_hits++;
}

void log_miss() {
	log_memory_ops++;
	log_misses++;
}

void print_log() {
	printf("hits: %d\n", log_hits);
	printf("misses: %d\n", log_misses);
	printf("memory ops: %d\n", log_memory_ops);
	printf("hits\%: %f\n", (float) log_hits / (float) log_memory_ops);
	printf("misses\%: %f\n", (float) log_misses / (float) log_memory_ops);
}
