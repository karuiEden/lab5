//
// Created by karui on 4/7/25.
//

#ifndef KEY_SPACE_H
#define KEY_SPACE_H

#include <stdint.h>
#include "adj_list.h"
#include "vertex.h"

typedef struct KeySpace {
    Vertex* vertex;
    AdjList* adj;
    struct KeySpace* next;
}KeySpace;

KeySpace **ks_init(uint32_t size);

KeySpace* ks_create(Vertex* v);

void ks_destroy(KeySpace** ks, uint32_t size);

void ks_delete(KeySpace* ks);


KeySpace* ks_search(KeySpace *ks, const char* id);


void ks_print(KeySpace* ks);

#endif //KEY_SPACE_H
