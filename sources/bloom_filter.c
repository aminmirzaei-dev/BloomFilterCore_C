#include "bloom_filter.h"
#include <stdlib.h>
#include <string.h>

struct bloom_filter {
    uint8_t *bit_array;
    size_t bit_count;
    size_t byte_count;
    size_t hash_count;
};

/*
 * FNV-1a hash function with seed variation.
 */
static uint32_t fnv1a_hash(const void *data, size_t len, uint32_t seed)
{
    const unsigned char *bytes = data;
    uint32_t hash = 2166136261u ^ seed;

    for (size_t i = 0; i < len; i++) {
        hash ^= bytes[i];
        hash *= 16777619u;
    }

    return hash;
}

static void set_bit(uint8_t *array, size_t index)
{
    array[index / 8] |= (uint8_t)(1u << (index % 8));
}

static int get_bit(const uint8_t *array, size_t index)
{
    return (array[index / 8] >> (index % 8)) & 1u;
}

bloom_filter_t* bloom_create(size_t bit_count, size_t hash_count)
{
    bloom_filter_t *filter;

    if (bit_count == 0 || hash_count == 0) {
        return NULL;
    }

    filter = malloc(sizeof(bloom_filter_t));
    if (!filter) {
        return NULL;
    }

    filter->bit_count = bit_count;
    filter->hash_count = hash_count;
    filter->byte_count = (bit_count + 7) / 8;

    filter->bit_array = calloc(filter->byte_count, sizeof(uint8_t));
    if (!filter->bit_array) {
        free(filter);
        return NULL;
    }

    return filter;
}

void bloom_add(bloom_filter_t *filter, const void *data, size_t length)
{
    uint32_t hash;

    if (!filter || !data) {
        return;
    }

    for (size_t i = 0; i < filter->hash_count; i++) {
        hash = fnv1a_hash(data, length, (uint32_t)i);
        set_bit(filter->bit_array, hash % filter->bit_count);
    }
}

int bloom_might_contain(const bloom_filter_t *filter,
                        const void *data,
                        size_t length)
{
    uint32_t hash;

    if (!filter || !data) {
        return 0;
    }

    for (size_t i = 0; i < filter->hash_count; i++) {
        hash = fnv1a_hash(data, length, (uint32_t)i);
        if (!get_bit(filter->bit_array, hash % filter->bit_count)) {
            return 0;
        }
    }

    return 1;
}

void bloom_destroy(bloom_filter_t *filter)
{
    if (!filter) {
        return;
    }

    free(filter->bit_array);
    free(filter);
}

