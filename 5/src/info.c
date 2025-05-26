//
// Created by karui on 5/26/2025.
//

#include "info.h"

#include <stdlib.h>

#include "error/error_handle.h"

InfoType* info_new(const uint8_t color, const uint32_t dist) {
    InfoType* info = calloc(1, sizeof(InfoType));
    if (!info) {
        error = ERR_ALLOC;
        return nullptr;
    }
    info->color = color;
    info->dist = dist;
    return info;
}

void info_destroy(InfoType* info) {
    if (!info) {
        return;
    }
    free(info);
}
