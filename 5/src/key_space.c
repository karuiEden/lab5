//
// Created by karui on 4/7/25.
//

#include "key_space.h"

#include <stdio.h>
#include <stdlib.h>
#include <error/error_handle.h>

#include "hash_table.h"


KeySpace **ks_init(uint32_t size) {
    KeySpace **ks = calloc(size, sizeof(KeySpace*));
    if (!ks) {
        error = ERR_ALLOC;
        return nullptr;
    }
    return ks;
}

KeySpace* ks_create(Vertex* v) {
    KeySpace *ks = calloc(1, sizeof(KeySpace));
    if (!ks) {
        error = ERR_ALLOC;
        return nullptr;
    }
    ks->vertex = v;
    ks->adj = list_new();
    ks->next = nullptr;
    return ks;

}

KeySpace* ks_search(KeySpace *ks, const char* id) {
    if (!ks) {
        error = INVALID_ELEM;
        return nullptr;
    }
    KeySpace *ks_curr = ks;
    while (ks_curr) {
        if (strcmp(ks_curr->vertex->id, id) == 0) {
            return ks_curr;
        }
        ks_curr = ks_curr->next;
    }
    error = INVALID_ELEM;
    return nullptr;
}



void ks_delete(KeySpace* ks) {
    if (!ks) {
        return;
    }
    vertex_destroy(ks->vertex);
    list_destroy(ks->adj);
    free(ks);
}



void ks_destroy(KeySpace** ks, const uint32_t size) {
    if (!ks) {
        return;
    }
    for (int i = 0; i < size; ++i) {
        while (ks[i]) {
            KeySpace* prev = ks[i];
            ks[i] = ks[i]->next;
            ks_delete(prev);
        }
    }
    free(ks);
}

void ks_print(KeySpace* ks) {
    if (!ks) {
        return;
    }
    const KeySpace* ks_curr = ks;
    uint32_t i = 0;
    while (ks_curr) {
        printf("Vertex: %s: ", ks_curr->vertex->id);
        list_print(ks_curr->adj);
        ks_curr = ks_curr->next;
        ++i;
    }
}

