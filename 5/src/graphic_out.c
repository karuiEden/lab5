//
// Created by karui on 4/25/25.
//

#include "graphic_out.h"
#include "key_space.h"

#include "adj_list.h"
#include "vertex.h"


void link_graph(Graph* gd, Agraph_t* gg) {
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
            agsafeset(n, "fillcolor", "", "");
            InfoType* info = info_new(n);
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
               InfoType* to = t_search(t, node->vertex->id);
                agedge(gg, t_search(t, ks->vertex->id)->node, to->node, nullptr, 1);
            }
            ks = ks->next;
            }
        }
}

void print(Graph* gd) {
    if (!gd) {
        error = ERR_PTR;
        return;
    }
    GVC_t *gvc = gvContext();
    Agraph_t *g = agopen("graph", Agdirected, nullptr);
    if (!g) {
        error = ERR_ALLOC;
        gvFreeContext(gvc);
        return;
    }
    link_graph(gd, g);
    if (gvLayout(gvc, g, "dot")) {
        gvFreeLayout(gvc, g);
        agclose(g);
        gvFreeContext(gvc);
        error = ERR_ALLOC;
        return;
    }
    if (gvRenderFilename(gvc, g, "png", "tbst.png")) {
        error = ERR_ALLOC;
    }
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);
    if (!error) system("catimg tbst.png");
}

