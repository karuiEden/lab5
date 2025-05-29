//
// Created by karui on 4/25/25.
//

#include "graphic_out.h"
#include "key_space.h"

#include "adj_list.h"
#include "vertex.h"


void link_graph(Graph* gd, Agraph_t* gg, key_space* add_info) {
    if (!gd || !gg) {
        return;
    }
    Table* t = init(gd->adj_lists->csize);
    for (uint32_t i = 0; i < gd->adj_lists->msize; ++i) {
        KeySpace* ks = gd->adj_lists->ks[i];
        while (ks) {
            Agnode_t *n = agnode(gg, ks->vertex->id, 1);
            agsafeset(n, "label", ks->vertex->id, "");
            if (ks->vertex->type != PASS) {
                if (ks->vertex->type == EXIT) {
                    agsafeset(n, "color", "green", "");
                } else {
                    agsafeset(n, "color", "red", "");
                }
            }
            InfoType* info = info_new(0, 0);
            info->g_node = n;
            t_insert(t, ks->vertex->id, info);
            ks = ks->next;
        }
    }

    for (uint32_t i = 0; i < gd->adj_lists->msize; ++i) {
        KeySpace* ks = gd->adj_lists->ks[i];
        while (ks) {
            AdjList* adj = ks->adj;
            if (!adj) {
                continue;
            }
            AdjNode* node = adj->head;
            while (node) {
                key_space* from = t_search(t, ks->vertex->id);
               key_space* to = t_search(t, node->vertex->id);
                if (strcmp(from->key, to->key) < 0) {
                    Agedge_t *edge = agedge(gg, from->info->g_node, to->info->g_node, nullptr, 1);
                    if (add_info) {
                        key_space* from_path = add_info;
                        while (from_path && strcmp(from_path->key, from->key) != 0) {
                            from_path = from_path->prev;
                        }
                        key_space* to_path = add_info;
                        while (to_path && strcmp(to_path->key, to->key) != 0) {
                            to_path = to_path->prev;
                        }
                        if (from_path && to_path) {
                            if (from_path->prev == to_path || to_path->prev == from_path) {
                                agsafeset(edge, "color", "red", "");
                            }
                        }
                    }
                }
                node = node->next;
            }
            ks = ks->next;
            }
        }
    delete(t);
}

void print_graphic(Graph* gd, key_space* add_info) {
    if (!gd) {
        error = ERR_PTR;
        return;
    }
    GVC_t *gvc = gvContext();
    Agraph_t *g = agopen("graph", Agundirected, nullptr);
    agsafeset(g, "overlap", "scale", "");
    agsafeset(g, "sep", "0.1", "");
    agsafeset(g, "len", "2", "");
    // agsafeset(g, "ranksep", "1", "");
    // agsafeset(g, "nodesep", "0.25", "");

    if (!g) {
        error = ERR_ALLOC;
        gvFreeContext(gvc);
        return;
    }
    link_graph(gd, g, add_info);
    if (gvLayout(gvc, g, "sfdp")) {
        gvFreeLayout(gvc, g);
        agclose(g);
        gvFreeContext(gvc);
        error = ERR_ALLOC;
        return;
    }
    if (gvRenderFilename(gvc, g, "png", "graph.png")) {
        error = ERR_ALLOC;
    }
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
    // if (!error) system("catimg graph.png");
}

