//
// Created by karui on 5/25/2025.
//

#ifndef GRAPH_H
#define GRAPH_H
#include "hash_table.h"

enum GRAPH_CODES {N_CYCLE = -6,DUPLICATE_VERTEX = -5, INVALID_VERTEX = -4};

enum colors{white, black, gray};

typedef struct Graph {
    HashTable* adj_lists;
} Graph;



#endif //GRAPH_H
