//
// Created by karui on 5/26/2025.
//

#ifndef INFO_H
#define INFO_H
#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>

typedef struct InfoType {
    // Как и цвет в BFS, так и тип вершины
    uint8_t color;
    uint64_t dist;
    Agnode_t* g_node;
} InfoType;

InfoType* info_new(uint8_t color, uint32_t dist);

void info_destroy(InfoType* info);

#endif //INFO_H
