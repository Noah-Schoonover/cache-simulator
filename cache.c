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
#include <time.h>

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

	#if CACHE_VERBOSE == 1
    printf("\ndata_size: %d\n", cfg.data_size);
    printf("data_size_bytes: %d\n", data_size_bytes);
    printf("line_size: %d\n", cfg.line_size);
    printf("num_lines: %d\n", num_lines);
	#endif

	if (cfg.associativity) {
		// direct mapping or n-way associativity
		lines_per_set = cfg.associativity;
		sets = num_lines / lines_per_set;
	}
	else {
		// full associativity
		lines_per_set = num_lines;
		sets = 1;
	}

	set_mask = sets - 1;
	set_bits = __builtin_popcount(set_mask);
	block_bits = __builtin_popcount(cfg.line_size - 1); 	/* bit hack for log2(line_size) */
	set_offset = block_bits;
	tag_offset = block_bits + set_bits;

	#if CACHE_VERBOSE == 1
	printf("set_mask: %d\n", set_mask);
	printf("set_bits: %d\n", set_bits);
	printf("sets: %d\n", sets);
	printf("block_bits: %d\n", block_bits);
	printf("set_offset: %d\n", set_offset);
	printf("tag_offset: %d\n", tag_offset);
	#endif

	cache = malloc(num_lines * sizeof(Line));
	if (!cache) {
		printf("error: could not allocate cache!\n");
		return CACHE_FAILURE;
	}
	memset(cache, 0, num_lines * sizeof(Line));

	if (cfg.replace_policy == RANDOM_REPLACEMENT) {
		// initialize random generator
		srand(time(NULL));   // Initialization, should only be called once.
	}

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
	uint32_t start_line = set * lines_per_set;

	for (int i = start_line; i < start_line + lines_per_set; i++) {
		// first, search for invalid block
		if (!cache[i].valid) {
			#if CACHE_VERBOSE == 1
			printf("found invalid index: %u\n", i);
			#endif
			insert_index = i;
			found_invalid_index = true;
			break;
		}
	}

	uint32_t max_fifo = 0;
	uint32_t min_fifo = 0;
	uint32_t min_fifo_index = 0;
	bool fifo_init = false;

	// get the min and max fifo
	for (int i = start_line; i < start_line + lines_per_set; i++) {
		if (!fifo_init) {
			// use the first valid fifo as the init
			// if there are no valid blocks, min and max fifo will be 0
			if (cache[i].valid) {
				max_fifo = cache[i].fifo;
				min_fifo = cache[i].fifo;
				min_fifo_index = i;
			}
			fifo_init = true;
		} else {
			// otherwise, we will use the fifo counter
			if (cache[i].valid && cache[i].fifo > max_fifo) {
				max_fifo = cache[i].fifo;
			}
			if (cache[i].valid && cache[i].fifo < min_fifo) {
				min_fifo = cache[i].fifo;
				min_fifo_index = i;
			}
		}
	}

	#if CACHE_VERBOSE == 1
	printf("FIFO_REPLACEMENT\n");
	printf("max_fifo: %u\n", max_fifo);
	printf("min_fifo : %u\n", min_fifo);
	printf("min_fifo_index: %u\n", min_fifo_index);
	#endif


	if (!found_invalid_index) {

		if (cfg.replace_policy == RANDOM_REPLACEMENT) {
			#if CACHE_VERBOSE == 1
			printf("RANDOM_REPLACEMENT\n");
			#endif
			// generate a random number between 0 and lines_per_set-1 (inclusive)
			int r = rand() % lines_per_set;
			// add to the start_line to get a random line index in the set
			insert_index = start_line + r;

		} else {	// FIFO_REPLACEMENT
			insert_index = min_fifo_index;
		}

	}

	cache[insert_index].fifo = max_fifo + 1;
	#if CACHE_VERBOSE == 1
	printf("insert_index: %u\n", insert_index);
	#endif

	// now insert the cache line
	if (cache[insert_index].valid && cache[insert_index].dirty) {
		log_dirty_writeback();
	}
	cache[insert_index].tag = tag;
	cache[insert_index].valid = true;
	if (memacc.type == STORE) {
		cache[insert_index].dirty = true;
	} else {
		cache[insert_index].dirty = false;
	}

}

/**
 * [query_cache description]
 * @return [description]
 */
CacheStatus query_cache() {

	uint32_t set = get_set(memacc.address);
	uint32_t tag = get_tag(memacc.address);

	#if CACHE_VERBOSE == 1
	printf("set: %d = ", set); printbin(set); printf("\n");
	printf("tag: %d = ", tag); printbin(tag); printf("\n");
	#endif

	uint32_t start_line = set * lines_per_set;
	#if CACHE_VERBOSE == 1
	for (int i = start_line; i < start_line + lines_per_set; i++) {
		printf("line %d---> tag: %d \t valid: %d \t dirty: %d \t fifo: %d\n",
			i, cache[i].tag, cache[i].valid, cache[i].dirty, cache[i].fifo);
	}
	#endif

	for (int i = start_line; i < start_line + lines_per_set; i++) {

		if (cache[i].tag == tag && cache[i].valid) {
			// cache hit

			if (memacc.type == LOAD) { log_load_hit(); }
			else { log_store_hit(); cache[i].dirty = true; }

			return CACHE_SUCCESS;
		}
	}

	// cache miss
	// insert into cache unless storing on no-write-allocate
	if (!(cfg.write_alloc == NO_WRITE_ALLOCATE && memacc.type == STORE)) {
        insert_cache(set, tag);
    }

    return CACHE_SUCCESS;

}

void free_cache() {
	if (cache) free(cache);
}
