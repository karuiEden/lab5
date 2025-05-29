//
// Created by karui on 5/28/2025.
//

#include "graph_wrapper.h"

#include "graphic_out.h"
#include "io/io.h"

void add_vertex_wrapper(Graph* graph) {
    if (!graph) {
        error = ERR_PTR;
        handle_error();
        return;
    }
    char* id = freadline(stdin, "Введите id вершины: ");
    if (error) {
        handle_error();
        if (id) {
            free(id);
        }
        return;
    }
    uint32_t type = read_uint32(stdin, "Введите тип комнаты(0 - вход, 1 - выход, 2 - проход): ", "Тип комнаты не должен быть выше 2", uint_check_2);
    if (error) {
        handle_error();
        free(id);
        return;
    }
    Vertex* vertex = vertex_new(id, type);
    if (error) {
        free(id);
        handle_error();
        return;
    }
    add_vertex(graph, vertex);
    if (error) {
        handle_specific_error();
    }
    free(id);
}

void delete_vertex_wrapper(Graph* graph) {
    if (!graph) {
        error = ERR_PTR;
        handle_error();
        return;
    }
    char* id = freadline(stdin, "Введите id вершины: ");
    if (error) {
        handle_error();
        if (id) {
            free(id);
        }
        return;
    }
    remove_vertex(graph, id);
    if (error) {
        handle_specific_error();
    }
    free(id);
}

void add_edge_wrapper(Graph* graph) {
    if (!graph) {
        error = ERR_PTR;
        handle_error();
        return;
    }
    char* id_from = freadline(stdin, "Введите id первой вершины: ");
    if (error) {
        handle_error();
        if (id_from) {
            free(id_from);
        }
        return;
    }
    char* id_to = freadline(stdin, "Введите id второй вершины: ");
    if (error) {
        handle_error();
        if (id_to) {
            free(id_to);
        }
        return;
    }
    add_edge(graph, id_from, id_to);
    if (error) {
        handle_specific_error();
    }
    free(id_from);
    free(id_to);
}

void remove_edge_wrapper(Graph* graph) {
    if (!graph) {
        error = ERR_PTR;
        handle_error();
        return;
    }
    char* id_from = freadline(stdin, "Введите id первой вершины: ");
    if (error) {
        handle_error();
        if (id_from) {
            free(id_from);
        }
        return;
    }
    char* id_to = freadline(stdin, "Введите id второй вершины: ");
    if (error) {
        handle_error();
        if (id_to) {
            free(id_to);
        }
        return;
    }
    remove_edge(graph, id_from, id_to);
    if (error) {
        handle_specific_error();
    }
    free(id_from);
    free(id_to);
}

void change_vertex_wrapper(Graph* graph) {
    if (!graph) {
        error = ERR_PTR;
        handle_error();
        return;
    }
    char* id = freadline(stdin, "Введите id вершины: ");
    if (error) {
        handle_error();
        if (id) {
            free(id);
        }
        return;
    }
    uint32_t type = read_uint32(stdin, "Введите новый тип комнаты(0 - вход, 1 - выход, 2 - проход): ", "Тип комнаты не должен быть выше 2", uint_check_2);
    if (error) {
        handle_error();
        free(id);
        return;
    }
    change_vertex(graph, id, type);
    if (error) {
        handle_specific_error();
    }
    free(id);
}

void shortest_path_wrapper(Graph* graph) {
    if (!graph) {
        error = ERR_PTR;
        handle_error();
        return;
    }
    Vertex *from, *to;
    while (true){
        char *id_from = freadline(stdin, "Введите id вершины-входа: ");
        if (error) {
            handle_error();
            if (id_from) {
                free(id_from);
            }
            return;
        }
        from = find_vertex(graph, id_from);
        free(id_from);
        if (error) {
            continue;
        }
        if (from->type == ENTRANCE) {
            break;
        }
    }
    while (true){
        char *id_to = freadline(stdin, "Введите id вершины-выхода: ");
        if (error) {
            handle_error();
            if (id_to) {
                free(id_to);
            }
            return;
        }
        to = find_vertex(graph, id_to);
        free(id_to);
        if (error) {
            continue;
        }
        if (to->type == EXIT) {
            break;
        }
    }
    Table* t = shortest_path(graph, from, to);
    if (error) {
        if (error > 0) handle_error();
        else handle_specific_error();
        return;
    }
    key_space* to_ks = t_search(t, to->id);
    print_graphic(graph, to_ks);
    delete(t);
}

void bfs_wrapper(Graph* graph) {
    if (!graph) {
        error = ERR_PTR;
        handle_error();
        return;
    }
    Vertex *from, *to;
    while (true){
        char *id_from = freadline(stdin, "Введите id вершины-входа: ");
        if (error) {
            handle_error();
            if (id_from) {
                free(id_from);
            }
            return;
        }
        from = find_vertex(graph, id_from);
        free(id_from);
        if (error) {
            continue;
        }
        if (from->type == ENTRANCE) {
            break;
        }
    }
    Table* t = bfs(graph, from);
    if (error) {
        if (error > 0) handle_error();
        else handle_specific_error();
        return;
    }
    while (true){
        char *id_to = freadline(stdin, "Введите id вершины-выхода: ");
        if (error) {
            handle_error();
            if (id_to) {
                free(id_to);
            }
            return;
        }
        to = find_vertex(graph, id_to);
        free(id_to);
        if (error) {
            continue;
        }
        if (to->type == EXIT) {
            break;
        }
    }
    key_space* exit = t_search(t, to->id);
    while (exit) {
        if (!strcmp(exit->key, from->id)) {
            printf("До этого выхода можно добраться!\n");
            delete(t);
            return;
        }
        exit = exit->prev;
    }
    delete(t);
    printf("До этого выхода невозможно добраться!\n");
}

void mst_wrapper(Graph* graph) {
    if (!graph) {
        error = ERR_PTR;
        handle_error();
        return;
    }
    Vertex* from;
    while (true){
        char *id_from = freadline(stdin, "Введите id вершины-входа: ");
        if (error) {
            handle_error();
            if (id_from) {
                free(id_from);
            }
            return;
        }
        from = find_vertex(graph, id_from);
        if (error) {
            free(id_from);
            continue;
        }
        free(id_from);
        if (from->type == ENTRANCE) {
            break;
        }
    }
    Table* t = mst(graph, from);
    error = OK;
    if (error) {
        if (error > 0) handle_error();
        else handle_specific_error();
        delete(t);
        return;
    }
    Graph* st = new();
    for (uint32_t i = 0; i < t->csize; ++i) {
        Vertex* v = vertex_new(t->keySpace[i]->key, t->keySpace[i]->info->color);
        add_vertex(st, v);
    }
    error = OK;
    for (uint32_t i = 0; i < t->csize; ++i) {
        key_space* ks = t->keySpace[i];
        if (ks->prev) {
            add_edge(st, ks->prev->key, ks->key);
        }
    }
    error = OK;
    print_graphic(st, nullptr);
    destroy(st);
    delete(t);
}

void import_wrapper(Graph* graph) {
    if (!graph) {
        error = ERR_PTR;
        handle_error();
        return;
    }
    FILE *v, *e;
    while (true){
        char *v_name = freadline(stdin, "Введите название файла с вершинами: ");
        if (error) {
            handle_error();
            if (v_name) {
                free(v_name);
            }
            return;
        }
        v = fopen(v_name, "r");
        free(v_name);
        if (v) {
            break;
        }
    }
    while (true){
        char *e_name = freadline(stdin, "Введите название файла с рёбрами: ");
        if (error) {
            handle_error();
            if (e_name) {
                free(e_name);
            }
            return;
        }
        e = fopen(e_name, "r");
        free(e_name);
        if (e) {
            break;
        }
    }
    import_graph(graph, v, e);
    if (error) {
        if (error > 0) handle_error();
        else handle_specific_error();
    }
    fclose(v);
    fclose(e);
}

void print_graph_wrapper(Graph* graph) {
    if (!graph) {
        error = ERR_PTR;
        handle_error();
        return;
    }
    print_graphic(graph, nullptr);
}