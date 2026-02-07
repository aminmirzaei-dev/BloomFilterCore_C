#include "../include/bloomfiltercore.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    bloom_filter_t *filter;
    const char *items[] = {
        "apple",
        "banana",
        "orange"
    };

    filter = bloom_create(1024, 3);
    if (!filter) {
        return 1;
    }

    for (size_t i = 0; i < 3; i++) {
        bloom_add(filter, items[i], strlen(items[i]));
    }

    printf("apple  : %d\n",
           bloom_might_contain(filter, "apple", 5));

    printf("grape  : %d\n",
           bloom_might_contain(filter, "grape", 5));

    bloom_destroy(filter);
    return 0;
}

