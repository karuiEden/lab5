//
// Created by karui on 4/2/25.
//

#include "iterator.h"

Iterator null_iterator() {
    return (Iterator){nullptr, nullptr, nullptr};
}

Iterator new_iterator(Data* data, const uint32_t size, const uint32_t idx) {
    return (Iterator){data + idx, data, data + size};
}

bool cmp_iterator(const Iterator* a, const Iterator* b) {
    return a->curr == b->curr;
}

bool has_next_iterator(const Iterator* i) {
    return i->curr < i->end;
}

bool has_prev_iterator(const Iterator* i) {
    return i->curr >= i->begin;
}

bool next_iterator(Iterator* const i) {
    if (!has_next_iterator(i)) {
        return false;
    }
    i->curr++;
    return true;
}

bool prev_iterator(Iterator* i) {
    if (!has_prev_iterator(i)) {
        return false;
    }
    i->curr--;
    return true;
}

void advance_iterator(Iterator* i, int32_t n) {
    while (has_next_iterator(i) && n > 0){
        i->curr++;
        n--;
    }
    while (has_prev_iterator(i) && n < 0) {
        i->curr--;
        n++;
    }
}

Data data_iterator(const Iterator *i) {
    if (i->curr >= i->end) {
        return *(i->end - 1);
    }
    return *(i->curr);
}

