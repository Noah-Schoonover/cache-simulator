/*************************************************************************
*      	filename:  log.h
*   	description:  Header file for log.c
*       author:  Schoonover, Noah
*
*	    class:       CSE 331
*    	instructor:  Tong
*    	assignment:  Lab #2
*       due:  	     3/21/2022
*************************************************************************/
#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include "config.h"

unsigned int log_memory_ops;
unsigned int log_hits;
unsigned int log_misses;
unsigned int log_run_time;
unsigned int log_memory_access_latency;

void print_log();
void log_hit();
void log_miss();

#endif
