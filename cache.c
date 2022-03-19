/*************************************************************************
*       filename:  config.c
*       description:  implementation for the cache
*
*       author:  Schoonover, Noah
*
*        class:       CSE 331
*        instructor:  Tong
*        assignment:  Lab #2
*       due:           3/21/2022
*************************************************************************/

#include "cache.h"
#include "config.h"
#include "trace.h"
#include "util.h"
#include "string.h"
#include "log.h"
#include <stdbool.h>

typedef struct {
	unsigned int tag;
	bool valid;
	bool dirty;
	unsigned int fifo;
} Line;

Line *cache;
uint32_t num_lines;     /* the number of lines in the cache */
uint32_t block_bits;	/* number of bits to represent each byte in the block */
uint32_t set_offset;	/* bit offset for the set index */
uint32_t sets;			/* number of sets in the cache */
uint32_t set_mask;		/* bit mask for set index */
uint32_t set_bits;		/* number of bits to represent each set */
uint32_t tag_offset;	/* bit offset for the tag */
uint32_t lines_per_set;

/**
 * [create_cache description]
 * @return [description]
 */
CacheStatus create_cache() {

    int data_size_bytes = cfg.data_size * (1 << 10);	/* bit hack for data_size * 2^10 */
    num_lines = data_size_bytes / cfg.line_size;
    printf("\ndata_size: %d\n", cfg.data_size);
    printf("data_size_bytes: %d\n", data_size_bytes);
    printf("line_size: %d\n", cfg.line_size);
    printf("num_lines: %d\n", num_lines);

	if (cfg.associativity) { lines_per_set = cfg.associativity; }
	else { lines_per_set = 1; }

	sets = num_lines / lines_per_set;
	set_mask = sets - 1;
	printf("set_mask: %d\n", set_mask);
	set_bits = __builtin_popcount(set_mask);
	printf("set_bits: %d\n", set_bits);

	printf("sets: %d\n", sets);

	block_bits = __builtin_popcount(cfg.line_size - 1); 	/* bit hack for log2(line_size) */
	printf("block_bits: %d\n", block_bits);
	set_offset = block_bits;
	printf("set_offset: %d\n", set_offset);

	tag_offset = block_bits + set_bits;
	printf("tag_offset: %d\n", tag_offset);

	cache = malloc(num_lines * sizeof(Line));
	if (!cache) {
		printf("error: could not allocate cache!\n");
		return CACHE_FAILURE;
	}
	memset(cache, 0, num_lines * sizeof(Line));

    return CACHE_SUCCESS;

}

/**
 * [get_set description]
 * @param  address               [description]
 * @return         [description]
 */
uint32_t get_set(uint32_t address) {
	uint32_t shifted_address = address >> set_offset;
	return shifted_address & set_mask;
}

/**
 * [get_tag description]
 * @param  address               [description]
 * @return         [description]
 */
uint32_t get_tag(uint32_t address) {
	return address >> tag_offset;
}

/**
 * [insert_cache description]
 * @param set  [description]
 * @param tag  [description]
 */
void insert_cache(uint32_t set, uint32_t tag) {

	uint32_t insert_index = 0;
	bool found_invalid_index = false;
	uint32_t start_line = set * 2;
	uint32_t max_fifo = -1;		/* will overflow to max uint */
	uint32_t min_fifo = -1;		/* will overflow to max uint */
	uint32_t min_fifo_index = 0;

	for (int i = start_line; i < start_line + lines_per_set; i++) {
		// first, search for invalid block
		if (!found_invalid_index && cache[i].valid == false) {
			insert_index = i;
			found_invalid_index = true;
		}
		// otherwise, we will use the fifo counter
		if (cache[i].fifo > max_fifo) {
			max_fifo = cache[i].fifo;
		}
		if (cache[i].fifo < min_fifo) {
			min_fifo = cache[i].fifo;
			min_fifo_index = i;
		}

	}

	// if all blocks were valid, use fifo index
	if (!found_invalid_index) {
		insert_index = min_fifo_index;
	}

	// now insert the cache line
	cache[insert_index].fifo = max_fifo + 1;
	cache[insert_index].tag = tag;
	cache[insert_index].valid = true;
	cache[insert_index].dirty = false;

}

/**
 * [query_cache description]
 * @return [description]
 */
CacheStatus query_cache() {

	uint32_t set = get_set(memacc.address);
	printf("set: %d\n", set);
	printf("set bin:"); printbin(set); printf("\n");

	uint32_t tag = get_tag(memacc.address);
	printf("tag: %d\n", tag);
	printf("tag bin:"); printbin(tag); printf("\n");

	uint32_t start_line = set * 2;
	bool hit = false;
	for (int i = start_line; i < start_line + lines_per_set; i++) {
		printf("line %d---> tag: %d \t valid: %d \t dirty: %d \t fifo: %d\n",
			i, cache[i].tag, cache[i].valid, cache[i].dirty, cache[i].fifo);
		if (cache[i].tag == tag) {
			hit = true;
			printf("cache hit\n");
			log_hit();
			break;
		}
	}

	if (!hit) {
		log_miss();
		insert_cache(set, tag);
	}

    return CACHE_SUCCESS;

}

void free_cache() {
	if (cache) free(cache);
}
