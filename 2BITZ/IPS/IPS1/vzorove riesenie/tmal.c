#include "tmal.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
 
/**
 * Global base pointer to block tables. Thread index is the index to blk_table.
 */
struct blk_array_info_t *blks_table = NULL;
 
/**
 * Allocate sparse table of blocks for several threads.
 * @param  nthreads     number of threads/items in the table
 * @return              pointer to the first block array, NULL = failed
 */
struct blk_array_info_t *tal_alloc_blks_table(unsigned nthreads)
{
    if (blks_table)
        return NULL;
    blks_table = (struct blk_array_info_t*)
        malloc(nthreads*sizeof(struct blk_array_info_t));
    return blks_table;
}
 
static void blk_ctor(struct blk_info_t *blk)
{
    blk->ptr = NULL;
    blk->prev_idx = -1;
    blk->next_idx = -1;
    blk->used = false;
}
 
static struct blk_info_t *get_blk(unsigned tid, unsigned i)
{
    return &blks_table[tid].blk_info_arr[i];
}
 
/**
 * Allocates and initialize array of blocks.
 * @param  tid      thread index.
 * @param  nblks    capacity in number of blocks in the array.
 * @param  theap    heap capacity for a given thread.
 * @return          pointer to the first block in an array.
 */
struct blk_info_t *tal_init_blks(unsigned tid, unsigned nblks, size_t theap)
{
    blks_table[tid].blk_info_arr = (struct blk_info_t*)
        malloc(nblks*sizeof(struct blk_info_t));
    if (blks_table[tid].blk_info_arr == NULL)
        return NULL;
 
    blks_table[tid].nblks = nblks;
 
    for (unsigned i = 0; i < nblks; i++)
        blk_ctor(get_blk(tid, i));
 
    struct blk_info_t *blk = get_blk(tid, 0);
    blk->ptr = malloc(theap);
    if (blk->ptr == NULL)
    {
        blks_table[tid].heap_size = 0;
        return NULL;
    }
    else
        blks_table[tid].heap_size = theap;
 
    blk->size = theap;
 
    return blk;
}
 
static int find_free_blk_info(unsigned tid)
{
    for (unsigned i = 0; i < blks_table[tid].nblks; i++)
        if (get_blk(tid, i)->ptr == NULL)
            return i;
    return -1;
}
 
/**
 * Splits one block into two.
 * @param tid       thread index
 * @param blk_idx  index of the block to be split
 * @param req_size  requested size of the block
 * @return          index of a new block created as remainder.
 */
int tal_blk_split(unsigned tid, int blk_idx, size_t req_size)
{
    int nblk_idx = find_free_blk_info(tid);
    if (nblk_idx == -1)
        return -1;
 
    struct blk_info_t *oblk = get_blk(tid, blk_idx);
    struct blk_info_t *nblk = get_blk(tid, nblk_idx);
 
    *nblk = *oblk;
    nblk->ptr = (char*)oblk->ptr + req_size;
    nblk->size -= req_size;
    nblk->prev_idx = blk_idx;
 
    oblk->size = req_size;
    oblk->next_idx = nblk_idx;
 
    return nblk_idx;
}
 
/**
 * Merge two blocks in the block list/array.
 * @param tid       thread index
 * @param left_idx  index of the left block
 * @param right_idx index of the right block
 */
void tal_blk_merge(unsigned tid, int left_idx, int right_idx)
{
    struct blk_info_t *lblk = get_blk(tid, left_idx);
    struct blk_info_t *rblk = get_blk(tid, right_idx);
 
    lblk->size += rblk->size;
    lblk->next_idx = rblk->next_idx;
 
    if (rblk->next_idx >= 0)
        get_blk(tid, rblk->next_idx)->prev_idx = left_idx;
    blk_ctor(rblk);
}
 
static size_t allign(size_t req)
{
    int m = sizeof(size_t);
    return (req + m-1) / m * m;
}
 
static unsigned first_fit(unsigned tid, size_t reqsize)
{
    for (unsigned i = 0; i < blks_table[tid].nblks; i++)
    {
        struct blk_info_t *blk = get_blk(tid, i);
        if (!blk->used && blk->ptr && blk->size >= reqsize)
        {
            return i;
        }
    }
    return -1;
}
 
/**
 * Allocate memory for a given thread. Note that allocated memory will be
 * aligned to sizeof(size_t) bytes.
 * @param  tid  thread index (in the blocks table)
 * @param  size requested allocated size
 * @return      pointer to allocated space, NULL = failed
 */
void *tal_alloc(unsigned tid, size_t size)
{
    size = allign(size);
 
    unsigned i = first_fit(tid, size);
    if ((int)i == -1)
        return NULL;
 
    struct blk_info_t *blk = get_blk(tid, i);
    size_t remainder = blk->size - size;
    if (remainder > allign(1))
        tal_blk_split(tid, i, size);
 
    blk->used = true;
    return blk->ptr;
 
}
 
static unsigned find_allocated_blk(unsigned tid, void *ptr)
{
    for (unsigned i = 0; i < blks_table[tid].nblks; i++)
    {
        if (get_blk(tid, i)->ptr == ptr)
            return i;
    }
    return -1;
}
 
static void release_blk(unsigned tid, unsigned idx)
{
    struct blk_info_t *blk = get_blk(tid, idx);
    blk->used = false;
    if (blk->next_idx >= 0 && get_blk(tid, blk->next_idx)->used != true)
        tal_blk_merge(tid, idx, blk->next_idx);
    if (blk->prev_idx >= 0 && get_blk(tid, blk->prev_idx)->used != true)
        tal_blk_merge(tid, blk->prev_idx, idx);
}
 
/**
 * Realloc memory for a given thread.
 * @param tid   thread index
 * @param ptr   pointer to allocated memory, NULL = allocate a new memory.
 * @param size  a new requested size (may be smaller than already allocated),
 *              0 = equivalent to free the allocated memory.
 * @return      pointer to reallocated space, NULL = failed.
 */
void *tal_realloc(unsigned tid, void *ptr, size_t size)
{
    if (ptr == NULL)
        return tal_alloc(tid, size);
    unsigned i = find_allocated_blk(tid, ptr);
    if ((int)i == -1)
        return NULL;
 
    struct blk_info_t *blk = get_blk(tid, i);
    // FIXME
    if (blk->size >= size)
        return blk->ptr;
 
    if (blk->next_idx >= 0)
    {
        struct blk_info_t *rblk = get_blk(tid, blk->next_idx);
        if (!rblk->used && blk->size + rblk->size >= size)
            tal_blk_merge(tid, i, blk->next_idx);
        return blk->ptr;
    }
 
    void *newptr = tal_alloc(tid, size);
    if (newptr == NULL)
        return NULL;
 
    memcpy(newptr, blk->ptr, blk->size);
    release_blk(tid, i);
    return newptr;
}
 
/**
 * Free memory for a given thread.
 * @param tid   thread index
 * @param ptr   pointer to memory allocated by tal_alloc or tal_realloc.
 *              NULL = do nothing.
 */
void tal_free(unsigned tid, void *ptr)
{
    if (ptr == NULL)
        return;
    unsigned i = find_allocated_blk(tid, ptr);
    if ((int)i == -1)
        return;
    release_blk(tid, i);
}