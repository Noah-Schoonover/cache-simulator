/*************************************************************************
*      	filename:  config.h
*   	description:  Header file for config.c
*       author:  Schoonover, Noah
*
*	    class:       CSE 331
*    	instructor:  Tong
*    	assignment:  Lab #2
*       due:  	     3/21/2022
*************************************************************************/
#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>

#define CONFIG_VERBOSE 1
#define NCONFIG_PARAMS 6
typedef enum { CONFIG_SUCCESS, CONFIG_FAILURE } ConfigStatus;

/**
 * struct config - globally stores config parameters
 */
struct config {
	unsigned int line_size;
	unsigned int associativity;
	unsigned int data_size;
	unsigned int replace_policy;
	unsigned int miss_penalty;
	unsigned int write_alloc;
} cfg;

ConfigStatus read_config(char *filename);

#endif
