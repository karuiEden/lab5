//
// Created by karui on 3/23/2025.
//

#include "table.h"

#include <stdlib.h>
#include <io/io.h>

#include "error/error_handle.h"

Table *init(const uint32_t size) {
    Table *t = calloc(1, sizeof(Table));
    if (!t) {
        error = ERR_ALLOC;
        return nullptr;
    }
    t->csize = 0;
    t->msize = size;
    t->keySpace = ks_new(size);
    if (error != OK) {
        delete(t);
        return nullptr;
    }
    return t;
}

void delete(Table *t) {
    if (!t || !t->keySpace) {
        error = ERR_PTR;
        return;
    }
    ks_free(t->keySpace, t->msize);
    free(t);
}

void t_insert(Table *t, const char* key, const InfoType* value) {
    if (t->csize == t->msize){
        error = TABLE_FULL;
        return;
    }
    binary_search(t->keySpace, key, 0, t->csize - 1);
    if(!error){
        error = ERR_INDEX;
        return;
    }
    error = OK;
    uint32_t index = t->csize;
    while(index > 0 && strcmp(t->keySpace[index - 1]->key, key) > 0){
        t->keySpace[index] = t->keySpace[index - 1];
        --index;
    }
    key_space* ks = map_ks_create(key, value);
    if (error) {
        return;
    }
    t->keySpace[index] = ks;
    t->csize++;
}

key_space* t_search(Table *t, const char* key) {
    if (!t || !key || !t->keySpace) {
        error = ERR_PTR;
        return nullptr;
    }
    if (t->csize == 0) {
        error = TABLE_EMPTY;
        return nullptr;
    }
    const uint32_t index = binary_search(t->keySpace, key, 0, t->csize - 1);
    if (error) {
        return nullptr;
    }
    return t->keySpace[index];
}

void t_erase(Table *t, const char* key) {
    if (!t || !key || !t->keySpace) {
        error = ERR_PTR;
        return;
    }
    if (t->csize == 0) {
        error = TABLE_EMPTY;
        return;
    }
    const uint32_t idx = binary_search(t->keySpace, key, 0, t->csize - 1);
    if (error) return;
    map_ks_delete(t->keySpace[idx]);
    for (uint32_t i = idx; i < t->csize - 1; ++i) {
        t->keySpace[i] = t->keySpace[i + 1];
    }
    if (t->csize > 1) {
        t->keySpace[t->csize - 1] = nullptr;
    }
    t->csize--;
}

// void t_print(Table *t) {
//     if (!t || !t->keySpace) {
//         return;
//     }
//     for (uint32_t i = 0; i < t->csize; ++i) {
//         map_ks_print(t->keySpace[i]);
//         printf("; ");
//     }
// }