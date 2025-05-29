//
// Created by karui on 5/25/2025.
//

#ifndef GRAPH_H
#define GRAPH_H
#include "hash_table.h"
#include "maps/table.h"

enum GRAPH_CODES {N_CYCLE = -6,DUPLICATE_VERTEX = -5, INVALID_VERTEX = -4};

enum colors{white=0, black=1, gray=2};

typedef struct Graph {
    HashTable* adj_lists;
} Graph;

Graph* new();

void destroy(Graph* g);

void add_vertex(Graph* g, Vertex* v);

bool edge_exist(Graph* g, const char* from_id, const char* to_id);

void add_edge(Graph* g, const char* from_id, const char* to_id);

void remove_edge(Graph* g, const char* from_id, const char* to_id);

void remove_vertex(Graph* g, const char* id);

Vertex* find_vertex(Graph* g, const char* id);

void import_graph(Graph* g, FILE* vertices, FILE* edges);

void change_vertex(Graph* g, const char* id, const uint8_t type);

void print_graph_console(Graph* g);

Table* bfs(Graph* g, const Vertex* s);

Table* shortest_path(Graph* g, Vertex* s, Vertex* to);

Table* mst(Graph* g, Vertex* r);

#endif //GRAPH_H
