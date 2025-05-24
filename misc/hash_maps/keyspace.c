#include "keyspace.h"
#include <stdlib.h>
#include <stdio.h>

KeySpace *keyspace_create(uint32_t size){
    KeySpace *keyspace = calloc(size, sizeof(KeySpace));
    if(!keyspace){
        error = ERR_ALLOC;
        return nullptr;
    }
    for (uint32_t i = 0; i < size; ++i) {
        keyspace[i].busy = NEW;
    }
    return keyspace;
}

void keyspace_print(const KeySpace* keyspace){
    printf("Key: %d ", keyspace->key);
    printf("Release: %u ", keyspace->release);
    printf("Value: %d", keyspace->info);
}
 void keyspace_delete(KeySpace* keyspace){
    keyspace->busy = DELETE;
}

void keyspace_insert(KeySpace* keyspace, const int32_t key, const uint32_t value, uint32_t release){
    keyspace->key = key;
    keyspace->busy = FULL;
    keyspace->release = release;
    keyspace->info = value;
}
