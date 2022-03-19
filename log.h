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

void print_log();
void log_store();
void log_store_hit();
void log_load();
void log_load_hit();
void log_other_instructions(unsigned int n);
void log_dirty_writeback();

#endif
