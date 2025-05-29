//
// Created by karui on 11.11.24.
//

#ifndef MENU_H
#define MENU_H
#include <stdint.h>

#include "graph.h"

typedef Graph structure;
typedef void (*func)(structure *);

typedef struct {
    const func* functions;
    const char** msg;
    uint32_t len;
} func_vec;

void menu(func_vec funcs, structure *value);

#endif //MENU_H
