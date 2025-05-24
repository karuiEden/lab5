//
// Created by karui on 3/27/25.
//

#include "memory.h"

void copy(void *src, void *dest, size_t size){
    memcpy(dest, src, size);
}