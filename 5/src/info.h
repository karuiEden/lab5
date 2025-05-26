//
// Created by karui on 5/26/2025.
//

#ifndef INFO_H
#define INFO_H
#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>

typedef struct InfoType {
    Agnode_t* node;
} InfoType;

InfoType* info_new(Agnode_t* node);

void info_destroy(InfoType* info);

#endif //INFO_H
