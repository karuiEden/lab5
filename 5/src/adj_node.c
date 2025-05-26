//
// Created by karui on 5/25/2025.
//

#include "adj_node.h"

#include "vertex.h"

AdjNode* node_new(Vertex* v) {
    AdjNode* gl = calloc(1, sizeof(AdjNode));
    if (!gl) {
        error = ERR_ALLOC;
        return nullptr;
    }
    gl->vertex = v;
    return gl;
}

void node_destroy(AdjNode* n) {
    if (!n) {
        return;
    }
    free(n);
}