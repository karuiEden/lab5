//
// Created by karui on 5/25/2025.
//

#ifndef VERTEX_H
#define VERTEX_H
#include <stdint.h>

enum VERTEX_TYPE {ENTRANCE = 0, EXIT = 1, PASS = 2};

typedef struct Vertex {
    char* id;
    uint8_t type;
}Vertex;

Vertex* vertex_new(const char* id, const uint8_t type);

void vertex_destroy(Vertex* v);

Vertex* vertex_copy(const Vertex* v);


#endif //VERTEX_H
