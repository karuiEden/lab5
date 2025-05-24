//
// Created by karui on 3/23/2025.
//

#include "key_space.h"

#include <stdio.h>
#include <stdlib.h>
#include <error/error_handle.h>

#include "table.h"



uint32_t binary_search(const KeySpace *ks, const uint32_t key, int32_t l, int32_t r) {
    if (!ks) {
        error = ERR_PTR;
        return -1;
    }
    uint32_t mid = (l + r) / 2;
    while (l<=r) {
        if (key == ks[mid].key) {
            return mid;
        }
        if (key > ks[mid].key) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
        mid = (l + r) / 2;
    }
    error = INVALID_ELEM;
    return mid;

}

KeySpace ks_default() {
    return (KeySpace){0, nullptr};
}

KeySpace* ks_init(const uint32_t size) {
    KeySpace* ks = calloc(size, sizeof(KeySpace));
    if (!ks) {
        error = ERR_ALLOC;
        return nullptr;
    }
    for (int i = 0; i < size; ++i) {
        ks[i].info = nullptr;
        ks[i].key = 0;
    }
    return ks;
}

KeySpace ks_create(const uint32_t key, const InfoType* info) {
    if (!key || !info) {
        error = ERR_PTR;
        return ks_default();
    }
    KeySpace res;
    res.key = key;
    res.info = info_copy(info);
    return res;
}

void ks_delete(KeySpace* ks) {
    if (!ks || !ks->info) {
        return;
    }
    info_delete(ks->info);
    ks->info = nullptr;
}

void ks_free(KeySpace* ks, const uint32_t size) {
    for (uint32_t i = 0; i < size; ++i) {
        ks_delete(&ks[i]);
    }
    free(ks);
}

void ks_print(const KeySpace* ks) {
    printf("Key: %u ", ks->key);
    info_print(ks->info);
}