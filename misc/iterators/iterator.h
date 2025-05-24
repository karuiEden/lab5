//
// Created by karui on 4/2/25.
//

#ifndef ITERATOR_H
#define ITERATOR_H
#include "table.h"

// in type data insert your structure
typedef KeySpace Data;

typedef struct Iterator {
    Data* curr;
    Data* begin;
    Data* end;
} Iterator;

enum ITERATOR_CODES {ITERATOR_OUT_OF_BOUNDS = -20, ITERATOR_EMPTY=-19, ITERATOR_ADVANCE_FAILED = -18};

Iterator new_iterator(Data* data, const uint32_t size, const uint32_t idx);


bool cmp_iterator(const Iterator* a, const Iterator* b);

bool next_iterator(Iterator* const i);

bool prev_iterator(Iterator* const i);

Data data_iterator(const Iterator *i);

Data* begin_iterator(const Iterator* i);

Iterator null_iterator();

bool has_next_iterator(const Iterator* i);

bool has_prev_iterator(const Iterator* i);

#endif //ITERATOR_H
