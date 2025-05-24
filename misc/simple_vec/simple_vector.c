//
// Created by karui on 4/10/25.
//

#include "simple_vector.h"

SimpleVector* simple_vector_new(uint32_t size) {
    SimpleVector* sv = calloc(1, sizeof(SimpleVector));
    if (sv == nullptr) {
        error = ERR_ALLOC;
        return sv;
    }
    sv->size = size;
    sv->data = calloc(size, sizeof(Data));
    if (sv->data == nullptr) {
        error = ERR_ALLOC;
        free(sv);
        return nullptr;
    }
    return sv;
}

void simple_vector_destroy(SimpleVector* sv) {
    if (!sv || !sv->data) {
        return;
    }
    for (int i = 0; i < sv->size; ++i) {
        info_free(sv->data[i]);
    }
    free(sv->data);
    free(sv);
}