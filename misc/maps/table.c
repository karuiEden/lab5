//
// Created by karui on 3/23/2025.
//

#include "table.h"

#include <stdlib.h>
#include <io/io.h>

#include "error/error_handle.h"

Table *init(const uint32_t size) {
    Table *t = calloc(sizeof(Table), 1);
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

void t_insert(Table *t, const char* key, InfoType* value) {
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
    while(index > 0 && t->keySpace[index - 1].key > key){
        t->keySpace[index].key = t->keySpace[index - 1].key;
        t->keySpace[index].info = t->keySpace[index - 1].info;
        --index;
    }
    t->keySpace[index].key = strdup(key);
    t->keySpace[index].info = value;
    t->csize++;
}

InfoType *t_search(Table *t, const char* key) {
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
    return t->keySpace[index].info;
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
    ks_delete(&t->keySpace[idx]);
    for (uint32_t i = idx; i < t->csize - 1; ++i) {
        t->keySpace[i].key = t->keySpace[i + 1].key;
        t->keySpace[i].info = t->keySpace[i + 1].info;
    }
    if (t->csize > 1) {
        t->keySpace[t->csize - 1].info = nullptr;
    }
    t->csize--;
}

void t_print(Table *t) {
    if (!t || !t->keySpace) {
        return;
    }
    for (uint32_t i = 0; i < t->csize; ++i) {
        ks_print(&t->keySpace[i]);
        printf("; ");
    }
}