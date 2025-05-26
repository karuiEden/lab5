//
// Created by karui on 5/25/2025.
//

#include "graph.h"

#include "queue_list.h"
#include "maps/table.h"

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

Table* bfs(Graph* g, Vertex* s) {
    if (!g) {
        error = ERR_PTR;
        return nullptr;
    }
    Table* t = init(g->adj_lists->csize);
    if (!t) {
        error = ERR_ALLOC;
        return nullptr;
    }
    for (uint32_t i = 0; i < g->adj_lists->msize; ++i) {
        KeySpace* ks = g->adj_lists->ks[i];
        while (ks) {
            InfoType* info = info_new(white, UINT32_MAX);
            if (!info) {
                error = ERR_ALLOC;
                delete(t);
                return nullptr;
            }
            t_insert(t, ks->vertex->id, info);
            info_destroy(info);
            ks = ks->next;
        }
    }
    const uint32_t idx = binary_search(t->keySpace, s->id, 0, t->csize - 1);
    if (error) {
        delete(t);
    }
    t->keySpace[idx].info->color = gray;
    t->keySpace[idx].info->dist = 0;
    queue* q = queue_init();
    if (!q) {
        delete(t);
        return nullptr;
    }
    queue_insert(q, &t->keySpace[idx]);
    while (q->tail) {
        key_space* u = queue_read(q);
        KeySpace* u_adj = search(g->adj_lists, u->key);
        if (!u_adj) {
            error = ERR_PTR;
            queue_delete(q);
            delete(t);
            return nullptr;
        }
        AdjNode* adj = u_adj->adj->head;
        while (adj) {
            const uint32_t idx_v = binary_search(t->keySpace, adj->vertex->id, 0, t->csize - 1);
            if (error) {
                delete(t);
            }
            if (t->keySpace[idx_v].info->color == white) {
                t->keySpace[idx_v].info->color = gray;
                t->keySpace[idx_v].info->dist = u->info->dist + 1;
                t->keySpace[idx_v].prev = u;
                queue_insert(q, &t->keySpace[idx_v]);
            }
            adj = adj->next;
        }
        u->info->color = black;
    }
    return t;
}

void shortest_path(Graph* g, Vertex* s, Vertex* to) {
    if (!g || !s || !to) {
        error = ERR_PTR;
        return;
    }
    Table* t = init(g->adj_lists->csize);
    if (!t) {
        error = ERR_ALLOC;
        return;
    }
    for (uint32_t i = 0; i < g->adj_lists->msize; ++i) {
        KeySpace* ks = g->adj_lists->ks[i];
        while (ks) {
            InfoType* info = info_new(white, UINT32_MAX);
            if (!info) {
                error = ERR_ALLOC;
                delete(t);
                return;
            }
            t_insert(t, ks->vertex->id, info);
            info_destroy(info);
            ks = ks->next;
        }
    }
    for (int i = 0; i < g->adj_lists->csize - 1; ++i) {
        KeySpace* ks = g->adj_lists->ks[i];
        const uint32_t idx_u = binary_search(t->keySpace, ks->vertex->id, 0, t->csize - 1);
        if (error) {
            delete(t);
        }        while (ks) {
           AdjNode* adj = ks->adj->head;
            while (adj) {
                const uint32_t idx_v = binary_search(t->keySpace, adj->vertex->id, 0, t->csize - 1);
                if (error) {
                    delete(t);
                    return;
                }
                if (t->keySpace[idx_v].info->dist > t->keySpace[idx_u].info->dist + 1) {
                    t->keySpace[idx_v].info->dist = t->keySpace[idx_u].info->dist + 1;
                    t->keySpace[idx_v].prev = &t->keySpace[idx_u];
                }
                adj = adj->next;
            }
            ks = ks->next;
        }
    }
    for (int i = 0; i < g->adj_lists->csize - 1; ++i) {
        KeySpace* ks = g->adj_lists->ks[i];
        const uint32_t idx_u = binary_search(t->keySpace, ks->vertex->id, 0, t->csize - 1);
        if (error) {
            delete(t);
        }
        while (ks) {
            AdjNode* adj = ks->adj->head;
            while (adj) {
                const uint32_t idx_v = binary_search(t->keySpace, adj->vertex->id, 0, t->csize - 1);
                if (error) {
                    delete(t);
                    return;
                }
                if (t->keySpace[idx_v].info->dist > t->keySpace[idx_u].info->dist + 1) {
                   error = N_CYCLE;
                    delete(t);
                    return;
                }
                adj = adj->next;
            }
            ks = ks->next;
        }
    }
    const uint32_t idx_to = binary_search(t->keySpace, to->id, 0, t->csize - 1);
    if (t->keySpace[idx_to].info->dist == INT32_MAX) {
        error = INVALID_VERTEX;
        delete(t);
        return;
    }
    const uint32_t idx_from = binary_search(t->keySpace, s->id, 0, t->csize - 1);
    for (const key_space* i = &t->keySpace[idx_to]; i != &t->keySpace[idx_from] ; i = i->prev) {
        printf("%s <- ", i->key);
    }
    printf("%s\n", t->keySpace[idx_from].key);
    delete(t);
}

