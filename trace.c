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

}

/**
 * get the next line from the trace file, store details in global memacc struct
 * automatically frees memory when the end of the file is reached
 * @return TRACE_SUCCESS, TRACE_FAILURE, or TRACE_END
 */
TraceStatus get_next_trace() {

	// get the next line from the trace file
	if ((read = getline(&line, &len, trace_file)) != -1) {
	    printf("Retrieved line of length %zu:\n", read);
	    printf("%s", line);
		// sscanf(line, "%d", &d)
	} else {
		// end of file reached
		if (line) free(line);
		fclose(trace_file);
		return TRACE_END;
	}

	return TRACE_SUCCESS;

}
