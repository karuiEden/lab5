//
// Created by karui on 3/23/2025.
//

#ifndef TABLE_H
#define TABLE_H
#include <stdio.h>

#include "key_space.h"


enum TABLE_CODES {TABLE_FULL = -5,  TABLE_EMPTY = -4, INVALID_ELEM=-3};

typedef struct Table {
    KeySpace* keySpace;
    uint32_t msize;
    uint32_t csize;
}Table;


void t_insert(Table *t, const uint32_t key, const InfoType* value);

void t_erase(Table* t, const uint32_t key);

InfoType* t_search(Table* t, const uint32_t key);

Table* init(const uint32_t size);

void delete(Table* t);

void t_print(Table *t);

#endif //TABLE_H
