//
// Created by karui on 5/25/2025.
//

#ifndef ADJ_NODE_H
#define ADJ_NODE_H
#include "vertex.h"
#include <stdlib.h>
#include <error/error_handle.h>

typedef struct AdjNode {
    Vertex* vertex;
    struct AdjNode* next;
}AdjNode;

AdjNode* node_new(Vertex* v);

void node_destroy(AdjNode* n);


#endif //ADJ_NODE_H
