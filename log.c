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
#include "config.h"

unsigned int l_stores;
unsigned int l_store_hits;
unsigned int l_loads;
unsigned int l_load_hits;
unsigned int l_other_instructions;
unsigned int l_dirty_writebacks;

void log_store() {
	l_stores++;
}

void log_store_hit() {
	l_store_hits++;
}

void log_load() {
	l_loads++;
}

void log_load_hit() {
	l_load_hits++;
}

void log_other_instructions(unsigned int n) {
	l_other_instructions += n;
}

void log_dirty_writeback() {
	l_dirty_writebacks++;
}

void print_log() {

	unsigned int total_memory_ops = l_stores + l_loads;
	unsigned int total_hits = l_load_hits + l_store_hits;
	unsigned int misses = total_memory_ops - l_load_hits - l_store_hits;
	unsigned int total_run_time = l_other_instructions + total_hits + misses * cfg.miss_penalty;

	unsigned int total_mem_acc_latency = misses * cfg.miss_penalty + total_hits;
	float avg_mem_acc_latency = (float) total_mem_acc_latency / (float) total_memory_ops;

	printf("l_other_instructions: %d\n", l_other_instructions);
	printf("l_loads: %d\n", l_loads);
	printf("l_stores: %d\n", l_stores);
	printf("l_dirty_writebacks: %d\n", l_dirty_writebacks);
	printf("l_dirty_writebacks * miss_penalty: %d\n", l_dirty_writebacks * cfg.miss_penalty);

	printf("Total Hit Percentage: %.2f\n",
		(float) (total_hits) / (float) (total_memory_ops) * 100.0);

	printf("Load Hit Percentage: %.2f\n", (float) l_load_hits / (float) l_loads * 100.0);
	printf("Store Hit Percentage: %.2f\n", (float) l_store_hits / (float) l_stores * 100.0);
	printf("Total Run Time: %d cycles\n", total_run_time);
	printf("Average Memory Access Latency: %.2f\n", avg_mem_acc_latency);


}
