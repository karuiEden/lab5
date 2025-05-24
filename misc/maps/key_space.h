//
// Created by karui on 3/23/2025.
//

#ifndef KEY_SPACE_H
#define KEY_SPACE_H
#include <stdint.h>
#include "../4b/src/info.h"
typedef struct KeySpace {
  uint32_t key;
  InfoType *info;
}KeySpace;

KeySpace* ks_init(const uint32_t size);

void ks_free(KeySpace* ks, const uint32_t size);

uint32_t binary_search(const KeySpace *ks, const uint32_t key, int32_t l, int32_t r);

void ks_delete(KeySpace* ks);

KeySpace ks_create(const uint32_t key, const InfoType* info);

void ks_print(const KeySpace* ks);


#endif //KEY_SPACE_H
