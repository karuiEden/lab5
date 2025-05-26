//
// Created by karui on 3/23/2025.
//

#ifndef ADJ_LIST_H
#define ADJ_LIST_H
#include <stdint.h>
#include <stdio.h>

#include "adj_node.h"
#include "vertex.h"

enum EDGES_CODES {INVALID_EDGE = -7, DUPLICATE_EDGE = -6};



typedef struct AdjList {
    AdjNode* head;
} AdjList;

AdjList* list_new();

void list_destroy(AdjList* l);

AdjNode* list_search(AdjList* adj, const char* id);

void list_insert(AdjList* adj, Vertex* v);

void list_erase(AdjList* adj, const char* id);

void list_print(AdjList* adj);

#endif //ADJ_LIST_H
