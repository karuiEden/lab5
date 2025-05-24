//
// Created by karui on 11.11.24.
//

#include "menu.h"
#include "../error/error_handle.h"
#include "../io/io.h"

void menu(const func_vec funcs, structure *value) {
    int8_t ch = -1;
    while (error != EOF || ch !=0 || error != ERR_ALLOC) {
        printf("Menu of lab\n");
        for (int i = 0; i < funcs.len; ++i) {
            printf("%d. %s\n", i + 1, funcs.msg[i]);
        }
        printf("0. Exit\n");
        ch = read_uint32(stdin, nullptr, "Введите числа, указанные в меню", uint_check_9);
        if (error == EOF || ch == 0) break;
        funcs.functions[ch - 1](value);
    }
}
