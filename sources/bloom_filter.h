#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <stddef.h>
#include <stdint.h>

typedef struct bloom_filter bloom_filter_t;

/*
 * Creates a Bloom filter.
 *
 * bit_count   : Total number of bits in the filter
 * hash_count  : Number of hash functions
 *
 * Returns a pointer to the Bloom filter or NULL on failure.
 */
bloom_filter_t* bloom_create(size_t bit_count, size_t hash_count);

/*
 * Inserts an element into the Bloom filter.
 */
void bloom_add(bloom_filter_t *filter, const void *data, size_t length);

/*
 * Checks whether an element might exist in the Bloom filter.
 *
 * Returns:
 *  1 -> possibly present
 *  0 -> definitely not present
 */
int bloom_might_contain(const bloom_filter_t *filter,
                        const void *data,
                        size_t length);

/*
 * Frees all resources associated with the Bloom filter.
 */
void bloom_destroy(bloom_filter_t *filter);

#endif /* BLOOM_FILTER_H */

