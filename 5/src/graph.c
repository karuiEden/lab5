//
// Created by karui on 5/25/2025.
//

#include "graph.h"

Graph* new(){
    Graph* g = calloc(1, sizeof(Graph));
    if (!g) {
        error = ERR_ALLOC;
        return nullptr;
    }
    g->adj_lists = new_ht(1);
    if (error) {
        free(g);
        return nullptr;
    }
    return g;
}

void destroy(Graph* g) {
    destroy_ht(g->adj_lists);
    free(g);
}

void add_vertex(Graph* g, Vertex* v) {
    if (!g || !v) {
        error = ERR_PTR;
        return;
    }
    insert_key(g->adj_lists, v);
    if (error == INVALID_ELEM) {
        error = DUPLICATE_VERTEX;
    }
}

bool edge_exist(Graph* g, const char* from_id, const char* to_id) {
    if (!g || !from_id || !to_id) {
        error = ERR_PTR;
        return false;
    }
    const uint32_t from_idx = hash(from_id) % g->adj_lists->msize;
    const KeySpace* from_ks = ks_search(g->adj_lists->ks[from_idx], from_id);
    if (error) {
        return false;
    }
    list_search(from_ks->adj, from_id);
    if (error) {
        return false;
    }
    const uint32_t to_idx = hash(to_id) % g->adj_lists->msize;
    const KeySpace* to_ks = ks_search(g->adj_lists->ks[to_idx], to_id);
    if (error) {
        return false;
    }
    list_search(to_ks->adj, to_id);
    if (error) {
        return false;
    }
    return true;
}

void add_edge(Graph* g, const char* from_id, const char* to_id) {
    if (!g || !from_id || !to_id) {
        error = ERR_PTR;
        return;
    }
    const uint32_t from_idx = hash(from_id) % g->adj_lists->msize;
    const KeySpace* from = ks_search(g->adj_lists->ks[from_idx], from_id);
    if (error) {
        return;
    }
    const uint32_t to_idx = hash(to_id) % g->adj_lists->msize;
    const KeySpace* to = ks_search(g->adj_lists->ks[to_idx], to_id);
    if (error) {
        return;
    }
    list_insert(from->adj, to->vertex);
    if (error) {
        error = DUPLICATE_EDGE;
        return;
    }
    list_insert(to->adj, from->vertex);
    if (error) {
        error = DUPLICATE_EDGE;
    }
}

void remove_edge(Graph* g, const char* from_id, const char* to_id) {
    if (!g || !from_id || !to_id) {
        error = ERR_PTR;
        return;
    }
    const uint32_t from_idx = hash(from_id) % g->adj_lists->msize;
    const KeySpace* from = ks_search(g->adj_lists->ks[from_idx], from_id);
    if (error) {
        return;
    }
    const uint32_t to_idx = hash(to_id) % g->adj_lists->msize;
    const KeySpace* to = ks_search(g->adj_lists->ks[to_idx], to_id);
    if (error) {
        return;
    }
    list_erase(from->adj, to_id);
    if (error) {
        error = INVALID_EDGE;
        return;
    }
    list_erase(to->adj, from_id);
    if (error) {
        error = INVALID_EDGE;
    }
}

void remove_vertex(Graph* g, const char* id) {
    if (!g || !id) {
        error = ERR_PTR;
        return;
    }
    const uint32_t idx = hash(id) % g->adj_lists->msize;
    const KeySpace* ks = ks_search(g->adj_lists->ks[idx], id);
    if (error) {
        return;
    }
    for (int i = 0; i < g->adj_lists->msize; ++i) {
        if (!g->adj_lists[i]) continue;
        const KeySpace* ks_node = g->adj_lists->ks[i];
        while (ks_node) {
            list_search(ks_node->adj, id);
            if (!error) {
                list_erase(ks_node->adj, id);
                list_erase(ks->adj, ks_node->vertex->id);
            }
            error = OK;
            ks_node = ks_node->next;
        }
    }
    erase(g->adj_lists, id);
    if (error) {
        error = INVALID_VERTEX;
    }
}

void change_vertex(Graph* g, const char* id, const uint8_t type) {
    if (!g || !id) {
        error = ERR_PTR;
        return;
    }
    const uint32_t idx = hash(id) % g->adj_lists->msize;
    const KeySpace* ks = ks_search(g->adj_lists->ks[idx], id);
    if (error) {
        error = INVALID_VERTEX;
        return;
    }
    ks->vertex->type = type;
}

void print_graph_console(Graph* g) {
    if (!g) {
        return;
    }
    print(g->adj_lists);
}