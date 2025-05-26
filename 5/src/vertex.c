//
// Created by karui on 5/25/2025.
//

#include "vertex.h"

#include <stdlib.h>
#include <string.h>

#include "error/error_handle.h"

Vertex* vertex_new(const char* id, const uint8_t type) {
    Vertex* v = calloc(1, sizeof(Vertex));
    if (!v) {
        error = ERR_ALLOC;
        return v;
    }
    v->id = strdup(id);
    v->type = type;
    return v;
}

void vertex_destroy(Vertex* v) {
    if (!v || !v->id) {
        return;
    }
    free(v->id);
    free(v);
}

Vertex* vertex_copy(const Vertex* v) {
    Vertex* copy = calloc(1, sizeof(Vertex));
    if (!copy) {
        return nullptr;
    }
    copy->id = strdup(v->id);
    copy->type = v->type;
    return copy;
}