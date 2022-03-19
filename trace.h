/*************************************************************************
*      	filename:  trace.h
*   	description:  Header file for trace.c
*       author:  Schoonover, Noah
*
*	    class:       CSE 331
*    	instructor:  Tong
*    	assignment:  Lab #2
*       due:  	     3/21/2022
*************************************************************************/
#ifndef TRACE_H
#define TRACE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define TRACE_VERBOSE 0
typedef enum { TRACE_SUCCESS, TRACE_FAILURE, TRACE_END } TraceStatus;
typedef enum { LOAD, STORE } AccessType;

/**
 * memacc - a global memory_access struct for storing information from
 * each line in the trace file.
 */
struct memory_access {
    AccessType type;    /* access type: load or store */
    uint32_t address;        /* memory access address */
    uint32_t islma;          /* instructions since last memory access */
} memacc;

TraceStatus begin_trace(char *filename);
TraceStatus get_next_trace();

#endif
