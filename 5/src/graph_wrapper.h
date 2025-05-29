//
// Created by karui on 5/28/2025.
//

#ifndef GRAPH_WRAPPER_H
#define GRAPH_WRAPPER_H
#include "graph.h"

void add_vertex_wrapper(Graph* graph);

void delete_vertex_wrapper(Graph* graph);

void add_edge_wrapper(Graph* graph);

void remove_edge_wrapper(Graph* graph);

void change_vertex_wrapper(Graph* graph);

void shortest_path_wrapper(Graph* graph);

void bfs_wrapper(Graph* graph);

void mst_wrapper(Graph* graph);

void import_wrapper(Graph* graph);

void print_graph_wrapper(Graph* graph);

#endif //GRAPH_WRAPPER_H
