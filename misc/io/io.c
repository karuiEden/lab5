//
// Created by karui on 08.11.24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#include "io.h"
#include "../error/error_handle.h"

char *freadline(FILE *type, const char *promt) {
    if(type == stdin && promt){
        printf("%s", promt);
    }
    int32_t ch;
    size_t line_len = 0;
    size_t len = 0;
    char *line = calloc(1, sizeof(char));
    do {
        char buffer[51];
        ch = fscanf(type, "%50[^\n]", buffer);
        if (ch == EOF && len != 0) {
            clearerr(type);
        } else if (ch == EOF) {
            free(line);
            error = EOF;
            return nullptr;
        } else if (ch == 0) {
            fscanf(type, "%*c");
        } else {
            len += strlen(buffer);
            line = (char *)realloc((char *)line, len + 1);
            if (!line) {
                error = ERR_ALLOC;
                handle_error();
                return nullptr;
            }
            memcpy(line + line_len, buffer, strlen(buffer));
            line_len = len;
        }
    } while (ch > 0);
    line[len] = '\0';
    return line;
}

//void print_message(const FILE *type,const char* message){
//    if(type == stdin || type == stdout){
//        printf("%s", message);
//    }
//}


double read_double(FILE* stream, const char *promt, const char *promt_error, bool (*check)(double)){
    double value;
    while (true){
        char* line = freadline(stream, promt);
        if(!line){
            free(line);
            error = EOF;
            break;
        }
        value = strtod(line, nullptr);
        if(errno == ERANGE || !check(value)){
            free(line);
            errno = OK;
            printf("%s", promt_error);
            continue;
        }
        free(line);
        break;
    }
    return value;
}

int32_t read_int32(FILE* stream, const char *promt, const char *promt_error, bool (*check)(int32_t)){
    int32_t value = 0;
    while (true){
        char* line = freadline(stream ,promt);
        if(!line){
            error = EOF;
            break;
        }
        value = strtol(line, NULL, 10);
        if(errno == ERANGE || !check(value)){
            free(line);
            errno = OK;
            printf("%s", promt_error);
            continue;
        }
        free(line);
        break;
    }
    return value;
}

uint32_t read_uint32(FILE* stream ,const char *promt, const char *promt_error, bool (*check)(uint32_t)){
    uint32_t value;
    while (true){
        char* line = freadline(stream, promt);
        if(!line){
            free(line);
            error = EOF;
            break;
        }
        value = strtoul(line, nullptr, 10);
        if(errno == ERANGE || !check(value)){
            free(line);
            errno = OK;
            printf("%s", promt_error);
            continue;
        }
        free(line);
        break;
    }
    return value;
}

bool uint_any(const uint32_t value) {
    return true;
}

bool uint_positive(const uint32_t value) {
    return value > 0;
}

bool uint_check_2(const uint32_t value){
    return value < 3;
}

bool uint_check_32(const uint32_t value){
    return value <= UINT32_MAX;
}

bool uint_check_7(const uint32_t value){
    return value < 8;
}

bool uint_check_8(const uint32_t value){
    return value < 9;
}

bool uint_check_9(const uint32_t value){
    return value < 10;
}


bool int_positive_number(const int a){
    return a > 0;
}

bool int_any_number(const int a){
    return 1;
}

bool double_positive_number(const double a){
    return a > 0;
}

bool double_any_number(const double a){
    return true;
}

bool int_check_3(const int32_t a) {
    return a >= 0 && a <= 3;
}

