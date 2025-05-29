#include <stdio.h>
#include <error/error_handle.h>
#include "graph.h"
#include <menu/menu.h>
#include "graph_wrapper.h"
#include "graphic_out.h"
int main(void) {
    func funcs[11] = {add_vertex_wrapper, add_edge_wrapper, delete_vertex_wrapper, change_vertex_wrapper, remove_edge_wrapper, bfs_wrapper, shortest_path_wrapper,
        mst_wrapper, print_graph_console, print_graph_wrapper, import_wrapper};
    const char* fnames[] = {"Добавить вершину в граф", "Добавить ребро в граф", "Удалить вершину из графа", "Изменить тип вершины","Удалить ребро из графа",
    "Поиск в ширину", "Нахождение кратчайшего пути", "Построение минимального остовного дерева", "Вывод графа в виде списков смежности", "Графический вывод графа", "Импорт графа"};
    func_vec fv = {funcs, fnames, 11};
    Graph* graph = new();
    menu(fv, graph);
    destroy(graph);
    return error;
}