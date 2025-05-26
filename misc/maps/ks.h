//
// Created by karui on 3/23/2025.
//

#ifndef KS_H
#define KS_H
#include <stdint.h>

#include "info.h"
#include "../src/info.h"
typedef struct ks {
  const char* key;
  InfoType *info;
}key_space;

key_space* ks_init(const uint32_t size);

void ks_free(key_space* ks, const uint32_t size);

uint32_t binary_search(const key_space *ks, const char* key, int32_t l, int32_t r);

void ks_delete(key_space* ks);

key_space ks_create(const char* key, const InfoType* info);

void ks_print(const key_space* ks);


#endif //KS_H
