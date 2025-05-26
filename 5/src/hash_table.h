//
// Created by karui on 4/7/25.
//

#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <stdint.h>
#include "key_space.h"
#include <stdlib.h>
#include <error/error_handle.h>

#include <stdio.h>
#include <string.h>

#include <simple_vec/simple_vector.h>
enum HASHTABLE_CODES {HTABLE_FULL = -5,  HTABLE_EMPTY = -4, HINVALID_ELEM=-3};


typedef struct HashTable {
  KeySpace **ks;
  uint32_t csize;
  uint32_t msize;
}HashTable;

HashTable* new_ht(uint32_t msize);

void destroy_ht(HashTable *ht);

uint32_t hash(const char* key);

void insert_key(HashTable *ht, Vertex* v);

SimpleVector *find(const HashTable *ht, const uint32_t key);

void erase(HashTable *ht, const char* key);

void erase_release(HashTable *ht, const uint32_t key, const uint32_t release);

KeySpace* search(const HashTable *ht, const char* id);

void print(const HashTable *ht);

void export(const HashTable *ht, FILE *pf, FILE *sf);

void import(HashTable *ht, FILE* pf, FILE* sf);

#endif //HASH_TABLE_H
