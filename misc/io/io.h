//
// Created by karui on 08.11.24.
//

#ifndef IO_H
#define IO_H
#include "stdint.h"
#include <stdio.h>

char *freadline(FILE *type, const char *promt);

void print_message(const FILE *type,const char* message);

int32_t read_int32(FILE* stream ,const char *promt, const char *promt_error, bool (*check)(int32_t));

double read_double(FILE* stream ,const char *promt, const char *promt_error, bool (*check)(double));

uint32_t read_uint32(FILE* stream ,const char *promt, const char *promt_error, bool (*check)(uint32_t));

bool int_positive_number(const int a);

bool int_any_number(const int a);

bool double_positive_number(const double a);

bool double_any_number(const double a);

bool uint_check_32(const uint32_t value);

bool uint_check_7(const uint32_t value);

bool uint_check_2(const uint32_t value);

bool uint_any(const uint32_t value);

bool int_check_3(const int32_t a);

bool uint_positive(const uint32_t value);

bool uint_check_8(const uint32_t value);

bool uint_check_9(const uint32_t value);

#endif //IO_H
