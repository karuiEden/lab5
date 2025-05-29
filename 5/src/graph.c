//
// Created by karui on 5/25/2025.
//

#include "graph.h"

#include "queue_list.h"
#include "io/io.h"
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
    const KeySpace* from = search(g->adj_lists, from_id);
    if (error) {
        return;
    }
    const KeySpace* to = search(g->adj_lists, to_id);
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
    const KeySpace* from = search(g->adj_lists, from_id);
    if (error) {
        return;
    }
    const KeySpace* to = search(g->adj_lists, to_id);
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

Vertex* find_vertex(Graph* g, const char* id) {
    if (!g || !id) {
        error = ERR_PTR;
        return nullptr;
    }
    const KeySpace* ks = search(g->adj_lists, id);
    if (error) {
        return nullptr;
    }
    return ks->vertex;
}

void remove_vertex(Graph* g, const char* id) {
    if (!g || !id) {
        error = ERR_PTR;
        return;
    }
    const KeySpace* ks = search(g->adj_lists, id);
    if (error) {
        return;
    }
    for (int i = 0; i < g->adj_lists->msize; ++i) {
        if (!g->adj_lists->ks[i]) continue;
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
    const KeySpace* ks = search(g->adj_lists, id);
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

Table* bfs(Graph* g, const Vertex* s) {
    if (!g) {
        error = ERR_PTR;
        return nullptr;
    }
    FILE* f;
    if (g->adj_lists->csize >= 1000) {
        f = fopen("bfs", "w");
        if (!f) {
            error = ERR_ALLOC;
            return nullptr;
        }
    }
    Table* t = init(g->adj_lists->csize);
    if (!t) {
        error = ERR_ALLOC;
        return nullptr;
    }
    for (uint32_t i = 0; i < g->adj_lists->msize; ++i) {
        KeySpace* ks = g->adj_lists->ks[i];
        while (ks) {
            InfoType* info = info_new(white, INT32_MAX);
            if (!info) {
                error = ERR_ALLOC;
                delete(t);
                return nullptr;
            }
            t_insert(t, ks->vertex->id, info);
            ks = ks->next;
        }
    }
    key_space* s_ks = t_search(t, s->id);
    if (error) {
        delete(t);
        return nullptr;
    }
    s_ks->info->color = gray;
    s_ks->info->dist = 0;
    queue* q = queue_init();
    if (!q) {
        delete(t);
        return nullptr;
    }
    queue_insert(q, s_ks);
    while (q->tail) {
        key_space* u = queue_read(q);
        if (g->adj_lists->csize >= 1000) {
            fprintf(f, "%s\n", u->key);
        }
        KeySpace* u_adj = search(g->adj_lists, u->key);
        if (!u_adj) {
            error = ERR_PTR;
            queue_delete(q);
            delete(t);
            return nullptr;
        }
        const AdjNode* adj = u_adj->adj->head;
        while (adj) {
            key_space *v = t_search(t, adj->vertex->id);
            if (error) {
                delete(t);
                queue_delete(q);
            }
            if (v->info->color == white) {
                v->info->color = gray;
                v->info->dist = u->info->dist + 1;
                v->prev = u;
                queue_insert(q, v);
            }
            adj = adj->next;
        }
        u->info->color = black;
    }
    queue_delete(q);
    free(q);
    if (g->adj_lists->csize >= 1000) {
        fclose(f);
    }
    return t;
}

Table* shortest_path(Graph* g, Vertex* s, Vertex* to) {
    if (!g || !s || !to) {
        error = ERR_PTR;
        return nullptr;
    }
    Table* t = init(g->adj_lists->csize);
    if (!t) {
        error = ERR_ALLOC;
        return nullptr;
    }
    for (uint32_t i = 0; i < g->adj_lists->msize; ++i) {
        const KeySpace* ks = g->adj_lists->ks[i];
        while (ks) {
            InfoType* info = info_new(white, UINT32_MAX);
            if (!info) {
                error = ERR_ALLOC;
                delete(t);
                return nullptr;
            }
            t_insert(t, ks->vertex->id, info);
            ks = ks->next;
        }
    }
    key_space* s_ks = t_search(t, s->id);
    if (error) {
        delete(t);
        return nullptr;
    }
    s_ks->info->dist = 0;
    for (int k = 0; k < g->adj_lists->csize - 1; ++k){
        for (int i = 0; i < g->adj_lists->msize; ++i) {
            const KeySpace* ks = g->adj_lists->ks[i];
            while (ks) {
                key_space* u = t_search(t, ks->vertex->id);
                if (error) {
                    delete(t);
                }
                const AdjNode* adj = ks->adj->head;
                while (adj) {
                    key_space* v = t_search(t, adj->vertex->id);
                    if (error) {
                        delete(t);
                        return nullptr;
                    }
                    if (v->info->dist > u->info->dist + 1) {
                        v->info->dist = u->info->dist + 1;
                        v->prev = u;
                    }
                    adj = adj->next;
                }
                ks = ks->next;
            }
        }
    }
    for (int k = 0; k < g->adj_lists->csize - 1; ++k) {
        for (int i = 0; i < g->adj_lists->msize; ++i) {
            const KeySpace* ks = g->adj_lists->ks[i];
            while (ks) {
                key_space* u = t_search(t, ks->vertex->id);
                if (error) {
                    delete(t);
                }
                const AdjNode* adj = ks->adj->head;
                while (adj) {
                    const key_space *v = t_search(t, adj->vertex->id);
                    if (error) {
                        delete(t);
                        return nullptr;
                    }
                    if (v->info->dist > u->info->dist + 1) {
                        error = N_CYCLE;
                        delete(t);
                        return nullptr;
                    }
                    adj = adj->next;
                }
                ks = ks->next;
            }
        }
    }
    key_space* to_ks = t_search(t, to->id);
    if (to_ks->info->dist == INT32_MAX) {
        error = INVALID_VERTEX;
        delete(t);
        return nullptr;
    }
    const key_space* from_ks = t_search(t, s->id);
    FILE* f;
    if (g->adj_lists->csize >= 1000) {
        f = fopen("shortest_path", "w");
        if (!f) {
            error = ERR_ALLOC;
            return nullptr;
        }
    } else {
        f = stdout;
    }
    for (const key_space* i = to_ks; i && i != from_ks ; i = i->prev) {
        fprintf(f,"%s <- ", i->key);
    }
    fprintf(f, "%s\n", from_ks->key);
    return t;
}

Table* mst(Graph* g, Vertex* r) {
    if (!g || !r) {
        error = ERR_PTR;
        return nullptr;
    }
    Table* t = init(g->adj_lists->csize);
    if (!t) {
        error = ERR_ALLOC;
        return nullptr;
    }
    queue* q = queue_init();
    if (error) {
        delete(t);
        return nullptr;
    }
    for (uint32_t i = 0; i < g->adj_lists->msize; ++i) {
        const KeySpace* ks = g->adj_lists->ks[i];
        while (ks) {
            InfoType* info = info_new(ks->vertex->type, UINT32_MAX);
            if (!info) {
                error = ERR_ALLOC;
                delete(t);
                return nullptr;
            }
            t_insert(t, ks->vertex->id, info);
            key_space* ks_c = t_search(t, ks->vertex->id);
            queue_insert(q, ks_c);
            ks = ks->next;
        }
    }
    key_space* r_ks = t_search(t, r->id);
    if (error) {
        delete(t);
        return nullptr;
    }
    r_ks->info->dist = 0;
    while (q->tail) {
        key_space* ks_min = extract_min(q);
        KeySpace* ks_min_adj = search(g->adj_lists, ks_min->key);
        if (!ks_min_adj) {
            error = ERR_PTR;
            delete(t);
            return nullptr;
        }
        const AdjNode* adj = ks_min_adj->adj->head;
        while (adj) {
            key_space* ks_v = t_search(t, adj->vertex->id);
            if (error) {
                delete(t);
                queue_delete(q);
                return nullptr;
            }
            if (in_queue(q, ks_v->key) && ks_v->info->dist > 1) {
                ks_v->prev = ks_min;
                ks_v->info->dist = 1;
            }
            adj = adj->next;
        }
    }
    queue_delete(q);
    free(q);
    return t;
}

void import_graph(Graph* g, FILE* vertices, FILE* edges) {
    if (!g || !vertices || !edges) {
        error = ERR_PTR;
        return;
    }
    destroy_ht(g->adj_lists);
    g->adj_lists = new_ht(1);
    if (error) {
        return;
    }
    while (!error) {
        char* v_str = freadline(vertices, nullptr);
        if (error) {
            if (v_str) {
                free(v_str);
            }
            break;
        }
        char* tokens = strtok(v_str, ",");
        char* id = strdup(tokens);
        tokens = strtok(nullptr, ",");
        uint8_t type;
        if (strcmp(tokens, "ENTRANCE") == 0) {
            type = ENTRANCE;
        } else if (strcmp(tokens, "EXIT") == 0) {
            type = EXIT;
        } else {
            type = PASS;
        }
        Vertex* v = vertex_new(id, type);
        free(v_str);
        free(id);
        if (!v) {
            error = ERR_ALLOC;
            return;
        }
        add_vertex(g, v);
        error = OK;
    }
    error = OK;
    while (!error) {
        char* e_str = freadline(edges, nullptr);
        if (error) {
            if (e_str) {
                free(e_str);
            }
            break;
        }
        const char* tokens = strtok(e_str, ",");
        char* from_id = strdup(tokens);
        tokens = strtok(nullptr, ",");
        const char* to_id = tokens;
        add_edge(g, from_id, to_id);
        error = OK;
        free(e_str);
        free(from_id);
        error = OK;
    }
    error = OK;
}