/*************************************************************************
*      	filename:  trace.c
*   	description:  functions for reading the trace file
*			trace line info is stored in a global struct
*
*       author:  Schoonover, Noah
*
*	    class:       CSE 331
*    	instructor:  Tong
*    	assignment:  Lab #2
*       due:  	     3/21/2022
*************************************************************************/

#include "trace.h"

/*
 * these globals are defined here to limit visibility to trace.c functions
 * trace_file and line are globals by necessity,
 * len and read are globals to increase read function call efficiency
 */
FILE *trace_file;
char *line;
size_t len;
ssize_t read;

/**
 * opens the trace file for reading, stores the file pointer globally
 * @param  filename               the trace filename
 * @return          TRACE_FAILURE or TRACE_SUCCESS
 */
TraceStatus begin_trace(char *filename) {

	// open trace file
	if ((trace_file = fopen(filename, "r")) == NULL) {
		printf("error: can't open trace file %s\n", filename);
		return TRACE_FAILURE;
	}

	return TRACE_SUCCESS;

}

/**
 * get the next line from the trace file, store details in global memacc struct
 * automatically frees memory when the end of the file is reached
 * @return TRACE_SUCCESS, TRACE_FAILURE, or TRACE_END
 */
TraceStatus get_next_trace() {

	// get the next line from the trace file
	if ((read = getline(&line, &len, trace_file)) != -1) {

		char c;
		sscanf(line, "%c %x %d", &c, &memacc.address, &memacc.islma);

		if (c == 'l') { memacc.type = LOAD; }
		else if (c == 's') { memacc.type = STORE; }
		else { printf("invalid memory access type\n"); return TRACE_FAILURE; }

		#if TRACE_VERBOSE == 1
		printf("\n(trace) read line: %s", line);
		printf("memacc.type = %d\n", memacc.type);
		printf("memacc.address = %x\n", memacc.address);
		printf("memacc.islma = %d\n", memacc.islma);
		#endif

	} else {
		// end of file reached
		if (line) free(line);
		fclose(trace_file);
		return TRACE_END;
	}

	return TRACE_SUCCESS;

}
