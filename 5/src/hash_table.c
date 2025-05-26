//
// Created by karui on 4/7/25.
//
#include "hash_table.h"

#include "maps/table.h"


uint32_t hash(const char* key) {
    uint32_t idx = UINT32_MAX;
    for (int i = 0; i < strlen(key); ++i) {
        idx = idx * 41 + key[i];
    }
    return idx;
}

bool check_prime(uint32_t n) {
    for (uint32_t i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

uint32_t find_prime(uint32_t previous) {
    uint32_t new = previous + 1;
    while (true) {
        if (check_prime(new)) return new;
        ++new;
    }
}

void resize(HashTable *ht) {
    const uint32_t new_size = find_prime(ht->msize);
    KeySpace **new_ks = ks_init(new_size);
    if (!new_ks) {
        error = ERR_ALLOC;
        return;
    }
    for (uint32_t i = 0; i < ht->msize; ++i) {
        KeySpace *k_curr = ht->ks[i];
        while (k_curr) {
            Node *n_curr = k_curr->node;
            while (n_curr) {
                const uint32_t idx = hash(k_curr->key) % new_size;
                ks_search(new_ks[idx], k_curr->key);
                if (!error) {
                    ks_extend(new_ks[idx], n_curr->info);
                    return;
                }
                error = OK;
                KeySpace *k = ks_create(k_curr->key, n_curr->info);
                if (!k) {
                    error = ERR_ALLOC;
                    return;
                }
                k->next = new_ks[idx];
                new_ks[idx] = k;
                n_curr = n_curr->next;
            }
            k_curr = k_curr->next;
        }
    }
    ks_destroy(ht->ks, ht->msize);
    ht->ks = new_ks;
    ht->msize = new_size;
}


HashTable *new_ht(const uint32_t msize) {
    HashTable *ht = calloc(1, sizeof(HashTable));
    if (!ht) {
        error = ERR_ALLOC;
        return nullptr;
    }
    ht->ks = ks_init(msize);
    ht->msize = msize;
    return ht;
}

void destroy_ht(HashTable *ht) {
    if (!ht) {
        return;
    }
    ks_destroy(ht->ks, ht->msize);
    free(ht);
}

void insert_key(HashTable *ht, Vertex* v) {
    if (!ht) {
        error = ERR_PTR;
        return;
    }
    const uint32_t idx = hash(v->id) % ht->msize;
    ks_search(ht->ks[idx], v->id);
    if (!error) {
        error = INVALID_ELEM;
        return;
    }
    KeySpace *k = ks_create(v);
    if (!k) {
        error = ERR_ALLOC;
        return;
    }
    k->next = ht->ks[idx];
    ht->ks[idx] = k;
    ht->csize++;
    if (ht->csize == ht->msize) {
        resize(ht);
    }
}


void erase(HashTable *ht, const char* id) {
    if (!ht || !ht->ks) {
        error = ERR_PTR;
        return;
    }
    const uint32_t idx = hash(id) % ht->msize;
    KeySpace *ks_curr = ht->ks[idx], *prev = ks_curr;
    error = INVALID_ELEM;
    while (ks_curr) {
        if (!strcmp(ks_curr->vertex->id, id)) {
            error = OK;
            break;
        }
        prev = ks_curr;
        ks_curr = ks_curr->next;
    }
    if (error) {
        return;
    }
    prev->next = ks_curr->next;
    if (ks_curr == ht->ks[idx]) {
        ht->ks[idx] = ks_curr->next;
        ks_delete(ks_curr);
    } else {
        ks_delete(ks_curr);
    }
    ht->csize -= 1;
}

void print(const HashTable *ht) {
    if (!ht) {
        return;
    }
    printf("Количество вершин: %u\n", ht->csize);
    for (int i = 0; i < ht->msize; ++i) {
        if (ht->ks[i]) {
            ks_print(ht->ks[i]);
        }
    }
}

KeySpace* search(const HashTable *ht, const char* id) {
    if (!ht) {
        error = ERR_PTR;
        return nullptr;
    }
    const uint32_t idx = hash(id) % ht->msize;
    KeySpace* res = ks_search(ht->ks[idx], id);
    if (error) {
        return nullptr;
    }
    return res;
}
