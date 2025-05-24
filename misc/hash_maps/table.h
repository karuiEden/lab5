#ifndef INC_3_TABLE_H
#define INC_3_TABLE_H
#include <stdio.h>
#include "keyspace.h"
#include <stdint.h>
#include "../error/error_handle.h"
#include <stdlib.h>

enum table_codes {
    EMPTY_TABLE = -1,
    BUSY_TABLE = -2,
};

typedef struct Table {

    KeySpace *ks;

    uint32_t msize;

    uint32_t csize;

} Table;

Table* table_create(uint32_t msize);

void table_delete(Table* table);

void table_insert(Table* table, const int32_t key, const int32_t value);

// InfoType* table_remove( Table* const table, int32_t key);

KeySpace* table_search(const Table* const table, int32_t key, uint32_t *size);

void table_print(const Table* const  table);

// Table* table_multisearch(Table* const table, uint32_t key1, uint32_t key2);

int32_t max_release_key(const Table* const table);

bool *find_primes();

void table_resize(Table* const table);

// void table_import(Table* const table, FILE* stream);
//
// void table_export(const Table *const table, FILE *out);
//
// KeySpace* table_supersearch(const Table* const table, int32_t key, uint32_t release);

#endif //INC_3_TABLE_H
