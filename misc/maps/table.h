//
// Created by karui on 3/23/2025.
//

#ifndef TABLE_H
#define TABLE_H
#include <stdio.h>

#include "ks.h"


enum TABLE_CODES {TABLE_FULL = -5,  TABLE_EMPTY = -4, INVALID_ELEM=-3};

typedef struct Table {
    key_space** keySpace;
    uint32_t msize;
    uint32_t csize;
}Table;


void t_insert(Table *t, const char* key, const InfoType* value);

void t_erase(Table* t, const char* key);

key_space* t_search(Table* t, const char* key);

Table* init(const uint32_t size);

void delete(Table* t);

void t_print(Table *t);

#endif //TABLE_H
