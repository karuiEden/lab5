//
// Created by karui on 4/30/24.
//

#ifndef INC_3_KEYSPACE_H
#define INC_3_KEYSPACE_H

#include <stdint.h>
#include "../error/error_handle.h"
enum busy {
    DELETE = -2,
    NEW = 0,
    FULL = 1
};

typedef struct KeySpace {
    int8_t busy;
    uint32_t release;
    int32_t key;
    uint32_t info;
} KeySpace;

KeySpace *keyspace_create(uint32_t size);

void keyspace_print(const KeySpace *keyspace);

void keyspace_delete(KeySpace* keyspace);

void keyspace_insert(KeySpace* keyspace,int32_t key, uint32_t value, uint32_t release);

#endif //INC_3_KEYSPACE_H
