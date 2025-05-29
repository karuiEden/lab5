//
// Created by karui on 3/23/2025.
//

#include "ks.h"

#include <stdlib.h>
#include <error/error_handle.h>

#include "table.h"



uint32_t binary_search(key_space **ks, const char* key, int32_t l, int32_t r) {
    if (!ks) {
        error = ERR_PTR;
        return -1;
    }
    uint32_t mid = (l + r) / 2;
    while (l<=r) {
        if (!strcmp(key, ks[mid]->key)) {
            return mid;
        }
        if (strcmp(key, ks[mid]->key) > 0) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
        mid = (l + r) / 2;
    }
    error = INVALID_ELEM;
    return mid;

}

key_space** ks_new(const uint32_t size) {
    key_space** ks = calloc(size, sizeof(key_space*));
    if (!ks) {
        error = ERR_ALLOC;
        return nullptr;
    }
    return ks;
}

key_space* map_ks_create(const char* key, InfoType* info) {
    if (!key || !info) {
        error = ERR_PTR;
        return nullptr;
    }
    key_space *res = calloc(1, sizeof(key_space));
    if (!res) {
        error = ERR_ALLOC;
        return nullptr;
    }
    res->key = strdup(key);
    res->info = info;
    res->prev = nullptr;
    return res;
}

void map_ks_delete(key_space* ks) {
    if (!ks || !ks->info) {
        return;
    }
    info_destroy(ks->info);
    free(ks->key);
    ks->info = nullptr;
    ks->prev = nullptr;
    free(ks);
}

void ks_free(key_space** ks, const uint32_t size) {
    for (uint32_t i = 0; i < size; ++i) {
        map_ks_delete(ks[i]);
    }
    free(ks);
}

