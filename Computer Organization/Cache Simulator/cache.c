/*
 * cache.c
 */


#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "cache.h"
#include "main.h"

 /* cache configuration parameters */
static int cache_split = 0;
static int cache_usize = DEFAULT_CACHE_SIZE;
static int cache_isize = DEFAULT_CACHE_SIZE;
static int cache_dsize = DEFAULT_CACHE_SIZE;
static int cache_block_size = DEFAULT_CACHE_BLOCK_SIZE;
static int words_per_block = DEFAULT_CACHE_BLOCK_SIZE / WORD_SIZE;
static int cache_assoc = DEFAULT_CACHE_ASSOC;
static int cache_writeback = DEFAULT_CACHE_WRITEBACK;
static int cache_writealloc = DEFAULT_CACHE_WRITEALLOC;

//temporal arguments
static int i;
static cache_line *item;
static cache_line *itr;

/* cache model data structures */
static Pcache icache;
static Pcache dcache;
static cache c1;
static cache c2;
static cache_stat cache_stat_inst;
static cache_stat cache_stat_data;

/************************************************************/
void set_cache_param(param, value)
int param;
int value;
{

	switch (param) {
	case CACHE_PARAM_BLOCK_SIZE:
		cache_block_size = value;
		words_per_block = value / WORD_SIZE;
		break;
	case CACHE_PARAM_USIZE:
		cache_split = FALSE;
		cache_usize = value;
		break;
	case CACHE_PARAM_ISIZE:
		cache_split = TRUE;
		cache_isize = value;
		break;
	case CACHE_PARAM_DSIZE:
		cache_split = TRUE;
		cache_dsize = value;
		break;
	case CACHE_PARAM_ASSOC:
		cache_assoc = value;
		break;
	case CACHE_PARAM_WRITEBACK:
		cache_writeback = TRUE;
		break;
	case CACHE_PARAM_WRITETHROUGH:
		cache_writeback = FALSE;
		break;
	case CACHE_PARAM_WRITEALLOC:
		cache_writealloc = TRUE;
		break;
	case CACHE_PARAM_NOWRITEALLOC:
		cache_writealloc = FALSE;
		break;
	default:
		printf("error set_cache_param: bad parameter value\n");
		exit(-1);
	}

}
/************************************************************/

/************************************************************/
void init_cache()
{
	/* initialize the cache, and cache statistics data structures */
	if (cache_split) {
		c1.size = cache_dsize;
		c1.associativity = cache_assoc;
		c1.n_sets = cache_dsize / cache_block_size / cache_assoc;
		c1.index_mask_offset = 2 + (int)(log(cache_block_size / 4) / log(2));
		c1.index_mask = (unsigned int)(pow(2, c1.index_mask_offset + (int)(log(c1.n_sets) / log(2))) - 1);
		c1.LRU_head = (Pcache_line *)malloc(sizeof(Pcache_line)*c1.n_sets);
		*c1.LRU_head = (cache_line*)malloc(sizeof(cache_line)*c1.n_sets);
		c1.LRU_tail = (Pcache_line *)malloc(sizeof(Pcache_line)*c1.n_sets);
		*c1.LRU_tail = (cache_line*)malloc(sizeof(cache_line)*c1.n_sets);
		c1.set_contents = (int *)malloc(sizeof(int)*c1.n_sets);
		for (i = 0; i < c1.n_sets; i++) {
			*(c1.set_contents + i) = 0;
			c1.LRU_head[i] = NULL;
			c1.LRU_tail[i] = NULL;
		}
		c2.size = cache_isize;
		c2.associativity = cache_assoc;
		c2.n_sets = cache_isize / cache_block_size / cache_assoc;
		c2.index_mask_offset = 2 + (int)(log(cache_block_size / 4) / log(2));
		c1.index_mask = (unsigned int)(pow(2, c1.index_mask_offset + (int)(log(c2.n_sets) / log(2))) - 1);
		c2.LRU_head = (Pcache_line *)malloc(sizeof(Pcache_line)*c2.n_sets);
		*c2.LRU_head = (cache_line*)malloc(sizeof(cache_line)*c2.n_sets);
		c2.LRU_tail = (Pcache_line *)malloc(sizeof(Pcache_line)*c2.n_sets);
		*c2.LRU_tail = (cache_line*)malloc(sizeof(cache_line)*c2.n_sets);
		c2.set_contents = (int *)malloc(sizeof(int)*c2.n_sets);
		for (i = 0; i < c2.n_sets; i++) {
			*(c2.set_contents + i) = 0;
			c2.LRU_head[i] = NULL;
			c2.LRU_tail[i] = NULL;
		}
	}
	else {
		c1.size = cache_usize;
		c1.associativity = cache_assoc;
		c1.n_sets = cache_usize / cache_block_size / cache_assoc;
		c1.index_mask_offset = 2 + (int)(log(cache_block_size / 4) / log(2));
		c1.index_mask = (unsigned int)(pow(2, c1.index_mask_offset + (int)(log(c1.n_sets) / log(2))) - 1);
		c1.LRU_head = (Pcache_line *)malloc(sizeof(Pcache_line)*c1.n_sets);
		*c1.LRU_head = (cache_line*)malloc(sizeof(cache_line)*c1.n_sets);
		c1.LRU_tail = (Pcache_line *)malloc(sizeof(Pcache_line)*c1.n_sets);
		*c1.LRU_tail = (cache_line*)malloc(sizeof(cache_line)*c1.n_sets);
		c1.set_contents = (int *)malloc(sizeof(int)*c1.n_sets);
		for (i = 0; i < c1.n_sets; i++) {
			*(c1.set_contents + i) = 0;
			c1.LRU_head[i] = NULL;
			c1.LRU_tail[i] = NULL;
		}
	}
}
/************************************************************/

/************************************************************/
void perform_access(addr, access_type)
unsigned addr, access_type;
{
	unsigned int index, tag;
	/* handle an access to the cache */
	index = ((addr & c1.index_mask) >> c1.index_mask_offset);
	if (cache_split&&access_type == 1)
		tag = (addr & (unsigned int)(0 - 1 - c1.index_mask)) >> (c1.index_mask_offset + (int)(log(c2.n_sets) / log(2)) - 1);
	else
		tag = (addr & (unsigned int)(0 - 1 - c1.index_mask)) >> (c1.index_mask_offset + (int)(log(c1.n_sets) / log(2)) - 1);
	item = (cache_line *)malloc(sizeof(cache_line));
	switch (access_type) {
	case TRACE_DATA_LOAD:
		if (c1.set_contents[index] == 0) {
			cache_stat_data.accesses++;
			cache_stat_data.misses++;
			cache_stat_data.demand_fetches += cache_block_size / 4;
			item->tag = tag;
			if (cache_writeback)
				item->dirty = 0;
			insert(&c1.LRU_head[index], &c1.LRU_tail[index], item);
			c1.set_contents[index]++;
		}
		else {
			itr = c1.LRU_head[index];
			while (itr != NULL) {
				if (itr->tag == tag)
					break;
				itr = itr->LRU_next;
			}
			if (itr == NULL) {
				cache_stat_data.accesses++;
				cache_stat_data.misses++;
				cache_stat_data.demand_fetches += cache_block_size / 4;
				if (c1.set_contents[index] == cache_assoc) {
					cache_stat_data.replacements++;
					if (cache_writeback)
						if (c1.LRU_tail[index]->dirty)
							cache_stat_data.copies_back += cache_block_size / 4;
					delete(&c1.LRU_head[index], &c1.LRU_tail[index], c1.LRU_tail[index]);
					c1.set_contents[index]--;
				}
				item->tag = tag;
				if (cache_writeback)
					item->dirty = 0;
				insert(&c1.LRU_head[index], &c1.LRU_tail[index], item);
				c1.set_contents[index]++;
			}
			else {
				item->tag = itr->tag;
				if (cache_writeback)
					item->dirty = itr->dirty;
				insert(&c1.LRU_head[index], &c1.LRU_tail[index], item);
				delete(&c1.LRU_head[index], &c1.LRU_tail[index], itr);
				cache_stat_data.accesses++;
			}
		}
		break;
	case TRACE_DATA_STORE:
		if (c1.set_contents[index] == 0) {
			cache_stat_data.accesses++;
			cache_stat_data.misses++;
			if (cache_writealloc) {
				cache_stat_data.demand_fetches += cache_block_size / 4;
				item->tag = tag;
				if (cache_writeback)
					item->dirty = 1;
				insert(&c1.LRU_head[index], &c1.LRU_tail[index], item);
				c1.set_contents[index]++;
			}
		}
		else {
			itr = c1.LRU_head[index];
			while (itr != NULL) {
				if (itr->tag == tag)
					break;
				itr = itr->LRU_next;
			}
			if (itr == NULL) {
				cache_stat_data.accesses++;
				cache_stat_data.misses++;
				if (cache_writealloc) {
					cache_stat_data.demand_fetches += cache_block_size / 4;
					if (c1.set_contents[index] == cache_assoc) {
						cache_stat_data.replacements++;
						if (cache_writeback)
							if (c1.LRU_tail[index]->dirty)
								cache_stat_data.copies_back += cache_block_size / 4;
						delete(&c1.LRU_head[index], &c1.LRU_tail[index], c1.LRU_tail[index]);
						c1.set_contents[index]--;
					}
					item->tag = tag;
					if (cache_writeback)
						item->dirty = 1;
					insert(&c1.LRU_head[index], &c1.LRU_tail[index], item);
					c1.set_contents[index]++;
				}
			}
			else {
				cache_stat_data.accesses++;
				if (cache_writealloc) {
					item->tag = itr->tag;
					if (cache_writeback)
						item->dirty = 1;
					insert(&c1.LRU_head[index], &c1.LRU_tail[index], item);
					delete(&c1.LRU_head[index], &c1.LRU_tail[index], itr);
				}
			}
		}
		if (!cache_writeback)
			cache_stat_data.copies_back += cache_block_size / 4;
		break;
	case TRACE_INST_LOAD:
		if (cache_split)
			if (c2.set_contents[index] == 0) {
				cache_stat_inst.accesses++;
				cache_stat_inst.misses++;
				cache_stat_inst.demand_fetches += cache_block_size / 4;
				item->tag = tag;
				if (cache_writeback)
					item->dirty = 0;
				insert(&c2.LRU_head[index], &c2.LRU_tail[index], item);
				c2.set_contents[index]++;
			}
			else {
				itr = c2.LRU_head[index];
				while (itr != NULL) {
					if (itr->tag == tag)
						break;
					itr = itr->LRU_next;
				}
				if (itr == NULL) {
					cache_stat_inst.accesses++;
					cache_stat_inst.misses++;
					cache_stat_inst.demand_fetches += cache_block_size / 4;
					if (c2.set_contents[index] == cache_assoc) {
						cache_stat_inst.replacements++;
						if (cache_writeback)
							if (c2.LRU_tail[index]->dirty)
								cache_stat_data.copies_back += cache_block_size / 4;
						delete(&c2.LRU_head[index], &c2.LRU_tail[index], c2.LRU_tail[index]);
						c2.set_contents[index]--;
					}
					item->tag = tag;
					if (cache_writeback)
						item->dirty = 0;
					insert(&c2.LRU_head[index], &c2.LRU_tail[index], item);
					c2.set_contents[index]++;
				}
				else {
					item->tag = itr->tag;
					if (cache_writeback)
						item->dirty = itr->dirty;
					insert(&c2.LRU_head[index], &c2.LRU_tail[index], item);
					delete(&c2.LRU_head[index], &c2.LRU_tail[index], itr);
					cache_stat_inst.accesses++;
				}
			}
		else
			if (c1.set_contents[index] == 0) {
				cache_stat_inst.accesses++;
				cache_stat_inst.misses++;
				cache_stat_inst.demand_fetches += cache_block_size / 4;
				item->tag = tag;
				if (cache_writeback)
					item->dirty = 0;
				insert(&c1.LRU_head[index], &c1.LRU_tail[index], item);
				c1.set_contents[index]++;
			}
			else {
				itr = c1.LRU_head[index];
				while (itr != NULL) {
					if (itr->tag == tag)
						break;
					itr = itr->LRU_next;
				}
				if (itr == NULL) {
					cache_stat_inst.accesses++;
					cache_stat_inst.misses++;
					cache_stat_inst.demand_fetches += cache_block_size / 4;
					if (c1.set_contents[index] == cache_assoc) {
						cache_stat_inst.replacements++;
						if (cache_writeback)
							if (c1.LRU_tail[index]->dirty)
								cache_stat_data.copies_back += cache_block_size / 4;
						delete(&c1.LRU_head[index], &c1.LRU_tail[index], c1.LRU_tail[index]);
						c1.set_contents[index]--;
					}
					item->tag = tag;
					if (cache_writeback)
						item->dirty = 0;
					insert(&c1.LRU_head[index], &c1.LRU_tail[index], item);
					c1.set_contents[index]++;
				}
				else {
					item->tag = itr->tag;
					if (cache_writeback)
						item->dirty = itr->dirty;
					insert(&c1.LRU_head[index], &c1.LRU_tail[index], item);
					delete(&c1.LRU_head[index], &c1.LRU_tail[index], itr);
					cache_stat_inst.accesses++;
				}
			}
		break;
	}
}
/************************************************************/

/************************************************************/
void flush()
{
	/* flush the cache */
	//write back
	if (cache_writeback)
		for (i = 0; i < c1.n_sets; i++) {
			itr = c1.LRU_head[i];
			while (itr != NULL) {
				if (itr->dirty)
					cache_stat_data.copies_back += cache_block_size / 4;
				itr = itr->LRU_next;
			}
		}
	//free
	free(c1.LRU_head);
	free(*c1.LRU_head);
	free(c1.LRU_tail);
	free(*c1.LRU_tail);
	free(c1.set_contents);
	if (cache_split) {
		free(c2.LRU_head);
		free(*c2.LRU_head);
		free(c2.LRU_tail);
		free(*c2.LRU_tail);
		free(c2.set_contents);
	}
}
/************************************************************/

/************************************************************/
//Shuang Lian Biao shanchu
void delete(head, tail, item)
Pcache_line *head, *tail;
Pcache_line item;
{
	if (item->LRU_prev) {
		item->LRU_prev->LRU_next = item->LRU_next;
	}
	else {
		/* item at head */
		*head = item->LRU_next;
	}

	if (item->LRU_next) {
		item->LRU_next->LRU_prev = item->LRU_prev;
	}
	else {
		/* item at tail */
		*tail = item->LRU_prev;
	}
}
/************************************************************/

/************************************************************/
/* inserts at the head of the list */
//Shuang Lian Biao charu
void insert(head, tail, item)
Pcache_line *head, *tail;
Pcache_line item;
{
	item->LRU_next = *head;
	item->LRU_prev = (Pcache_line)NULL;

	if (item->LRU_next)
		item->LRU_next->LRU_prev = item;
	else
		*tail = item;

	*head = item;
}
/************************************************************/

/************************************************************/
void dump_settings()
{
	printf("*** CACHE SETTINGS ***\n");
	if (cache_split) {
		printf("  Split I- D-cache\n");
		printf("  I-cache size: \t%d\n", cache_isize);
		printf("  D-cache size: \t%d\n", cache_dsize);
	}
	else {
		printf("  Unified I- D-cache\n");
		printf("  Size: \t%d\n", cache_usize);
	}
	printf("  Associativity: \t%d\n", cache_assoc);
	printf("  Block size: \t%d\n", cache_block_size);
	printf("  Write policy: \t%s\n",
		cache_writeback ? "WRITE BACK" : "WRITE THROUGH");
	printf("  Allocation policy: \t%s\n",
		cache_writealloc ? "WRITE ALLOCATE" : "WRITE NO ALLOCATE");
}
/************************************************************/

/************************************************************/
void print_stats()
{
	printf("\n*** CACHE STATISTICS ***\n");

	printf(" INSTRUCTIONS\n");
	printf("  accesses:  %d\n", cache_stat_inst.accesses);
	printf("  misses:    %d\n", cache_stat_inst.misses);
	if (!cache_stat_inst.accesses)
		printf("  miss rate: 0 (0)\n");
	else
		printf("  miss rate: %2.4f (hit rate %2.4f)\n",
		(float)cache_stat_inst.misses / (float)cache_stat_inst.accesses,
			1.0 - (float)cache_stat_inst.misses / (float)cache_stat_inst.accesses);
	printf("  replace:   %d\n", cache_stat_inst.replacements);

	printf(" DATA\n");
	printf("  accesses:  %d\n", cache_stat_data.accesses);
	printf("  misses:    %d\n", cache_stat_data.misses);
	if (!cache_stat_data.accesses)
		printf("  miss rate: 0 (0)\n");
	else
		printf("  miss rate: %2.4f (hit rate %2.4f)\n",
		(float)cache_stat_data.misses / (float)cache_stat_data.accesses,
			1.0 - (float)cache_stat_data.misses / (float)cache_stat_data.accesses);
	printf("  replace:   %d\n", cache_stat_data.replacements);

	printf(" TRAFFIC (in words)\n");
	printf("  demand fetch:  %d\n", cache_stat_inst.demand_fetches +
		cache_stat_data.demand_fetches);
	printf("  copies back:   %d\n", cache_stat_inst.copies_back +
		cache_stat_data.copies_back);
}
/************************************************************/
