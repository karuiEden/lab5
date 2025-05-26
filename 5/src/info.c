//
// Created by karui on 5/26/2025.
//

#include "info.h"

#include <stdlib.h>

#include "error/error_handle.h"

InfoType* info_new(Agnode_t* node) {
    InfoType* info = calloc(1, sizeof(InfoType));
    if (!info) {
        error = ERR_ALLOC;
        return nullptr;
    }
    info->node = node;
    return info;
}

void info_destroy(InfoType* info) {
    if (!info || !info->node) {
        return;
    }
    free(info);
}
