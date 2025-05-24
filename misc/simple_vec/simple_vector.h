//
// Created by karui on 4/10/25.
//

#ifndef SIMPLE_VECTOR_H
#define SIMPLE_VECTOR_H
#include <error/error_handle.h>
#include <stdlib.h>

typedef char* Data;

typedef struct SimpleVector {
    Data* data;
    uint32_t size;
}SimpleVector;

SimpleVector* simple_vector_new(uint32_t size);

void simple_vector_destroy(SimpleVector* sv);

#endif //SIMPLE_VECTOR_H
