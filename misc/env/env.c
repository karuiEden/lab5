//
// Created by karui on 17.11.24.
//

#include "env.h"

#include <errno.h>
#include <stdlib.h>

#include "../error/error_handle.h"

int32_t get_num_env(const char* env_name) {
    char* str_value = getenv(env_name);
    if (!str_value) {
        error = ERR_ENV;
        return error;
    }
    const int32_t value = strtol(str_value, nullptr, 10);
    if (errno == ERANGE) {
        error = ERR_ENV;
        errno = OK;
        return error;
    }
    return value;
}
