#include <stdio.h>
#include <stdlib.h>
#include "bloom_filter.h"

uint64_t calc_hash(const char* str, uint64_t modulus, uint64_t seed) {
  uint64_t hash = 0;
  uint64_t length = 0;
  while(str[length] != '\0') {
    length++;
  }
  for(int i = 0; i < length; ++i) {
    hash = (hash + (str[i] * seed))%modulus;
    seed = (seed*seed)%modulus;
  }
  return hash;
}

void bloom_init(struct BloomFilter* bloom_filter, uint64_t set_size, hash_fn_t hash_fn,  uint64_t hash_fn_count) {
  struct BloomFilter* myfilter = bloom_filter;
  myfilter->hash_fn_count = 1;
  myfilter->hash_fn = hash_fn;

  myfilter->set_size = set_size;
  myfilter->set = malloc(set_size*sizeof(uint64_t));
  for(int i = 0; i < set_size; ++i) {
    myfilter->set[i] = 0;
  }
}
void bloom_destroy(struct BloomFilter* bloom_filter) {
  free(bloom_filter->set);
  bloom_filter->set = NULL;
}
void bloom_insert(struct BloomFilter* bloom_filter, Key key) {
  uint64_t index1 = bloom_filter->hash_fn(key, bloom_filter->set_size, 37);
  uint64_t index2 = bloom_filter->hash_fn(key, bloom_filter->set_size, 39);
  bloom_filter->set[index1 / 64] |= (1ull << (index1 % 64));
  bloom_filter->set[index2 / 64] |= (1ull << (index2 % 64));
}

bool bloom_check(struct BloomFilter* bloom_filter, Key key) {
  uint64_t index1 = bloom_filter->hash_fn(key, bloom_filter->set_size, 37);
  uint64_t index2 = bloom_filter->hash_fn(key, bloom_filter->set_size, 39);
  return (bloom_filter->set[index1 / 64] & (1ull << (index1 % 64))) && (bloom_filter->set[index2 / 64] & (1ull << (index2 % 64)));
}
