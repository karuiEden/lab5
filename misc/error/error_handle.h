//
// Created by karui on 08.11.24.
//

#ifndef ERROR_HANDLE_H
#define ERROR_HANDLE_H
#include "stdint.h"
enum codes {OK, ERR_ALLOC, ERR_INPUT, ERR_ENV, ERR_INDEX, ERR_SEEK, ERR_READ, ERR_WRITE, ERR_FILE, ERR_PTR};
extern int16_t error;

void handle_error();

void handle_specific_error();

#endif //ERROR_HANDLE_H
