//
// Created by karui on 3/23/2025.
//

#include "adj_list.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error/error_handle.h>



AdjList* list_new() {
    AdjList* list = calloc(1, sizeof(AdjList));
    if (!list) {
        error = ERR_ALLOC;
        return nullptr;
    }
    return list;
}

void list_destroy(AdjList* l) {
    if (!l) {
        return;
    }
    AdjNode* node = l->head;
    while (node) {
        AdjNode* prev = node;
        node = node->next;
        node_destroy(prev);
    }
    free(l);
}


AdjNode* list_search(AdjList* adj, const char* id) {
    if (!adj || !id) {
        error = ERR_PTR;
        return nullptr;
    }
    AdjNode* ptr = adj->head;
    while (ptr) {
        if (!strcmp(id, ptr->vertex->id)) {
            return ptr;
        }
        ptr = ptr->next;
    }
    error = INVALID_EDGE;
    return nullptr;
}

void list_insert(AdjList* adj, Vertex* v) {
    if (!adj || !v) {
        error = ERR_PTR;
        return;
    }
    AdjNode* node = node_new(v);
    if (error) {
        return;
    }
    if (!adj->head) {
        adj->head = node;
        return;
    }
    if (strcmp(adj->head->vertex->id, v->id) > 0) {
        node->next = adj->head;
        adj->head = node;
        return;
    }
    AdjNode* ptr = adj->head, *prev = ptr;
    while (ptr) {
        if (strcmp(v->id, ptr->vertex->id) == 0) {
            node_destroy(node);
            error = DUPLICATE_EDGE;
            return;
        }
        if (strcmp(v->id, ptr->vertex->id) < 0) {
            break;
        }
        prev = ptr;
        ptr = ptr->next;
    }
    node->next = prev->next;
    prev->next = node;
}

void list_erase(AdjList* adj, const char* id) {
    if (!adj || !id) {
        error = ERR_PTR;
        return;
    }
    error = INVALID_EDGE;
    AdjNode* ptr = adj->head, *prev = ptr;
    while (ptr) {
        if (!strcmp(id, ptr->vertex->id)) {
            error = OK;
           break;
        }
        prev = ptr;
        ptr = ptr->next;
    }
    if (error) {
        return;
    }
    prev->next = ptr->next;
    if (adj->head == ptr) {
        adj->head = ptr->next;
    }
    node_destroy(ptr);
}

void list_print(AdjList* adj) {
    if (!adj) {
        return;
    }
    AdjNode* ptr = adj->head;
    while (ptr) {
        printf("%s -> ", ptr->vertex->id);
        ptr = ptr->next;
    }
}
