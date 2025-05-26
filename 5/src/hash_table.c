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

void insert_key(HashTable *ht, Vertex *val) {
    if (!ht) {
        error = ERR_ALLOC;
        return;
    }
    const uint32_t idx = hash(val->id) % ht->msize;
    ks_search(ht->ks[idx], val->id);
    if (!error) {
        error = INVALID_ELEM;
        return;
    }
    error = OK;
    KeySpace *k = ks_create(val);
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

void export(const HashTable *ht, FILE *pf, FILE *sf) {
    if (!ht || !ht->ks || !pf || !sf) {
        error = ERR_PTR;
        return;
    }
    uint32_t offset = 0;
    size_t n = fwrite(&ht->csize, sizeof(uint32_t), 1, pf);
    if (n != 1) {
        error = ERR_PTR;
        return;
    }
    for (int i = 0; i < ht->msize; ++i) {
        const KeySpace *ks_curr = ht->ks[i];
        while (ks_curr) {
            const Node *node_curr = ks_curr->node;
            while (node_curr) {
                n = fwrite(&ks_curr->key, sizeof(uint32_t), 1, pf);
                if (n != 1) {
                    error = ERR_WRITE;
                    return;
                }
                n = fwrite(&offset, sizeof(uint32_t), 1, pf);
                if (n != 1) {
                    error = ERR_WRITE;
                    return;
                }
                uint32_t isize = info_size(node_curr->info);
                n = fwrite(&isize, sizeof(uint32_t), 1, sf);
                if (n != 1) {
                    error = ERR_WRITE;
                    return;
                }
                n = info_fwrite(node_curr->info, sf);
                if (n != 1) {
                    error = ERR_WRITE;
                    return;
                }
                offset += info_size(node_curr->info) + sizeof(uint32_t);
                node_curr = node_curr->next;
            }
            ks_curr = ks_curr->next;
        }
    }
}

void import(HashTable *ht, FILE *pf, FILE *sf) {
    if (!ht || !pf || !sf) {
        error = ERR_PTR;
        return;
    }
    ks_destroy(ht->ks, ht->msize);
    uint32_t amount;
    size_t n = fread(&amount, sizeof(uint32_t), 1, pf);
    if (n != 1) {
        error = ERR_READ;
        return;
    }
    ht->ks = ks_init(amount);
    if (error) {
        return;
    }
    for (int i = 0; i < amount; ++i) {
        uint32_t key, offset;
        n = fread(&key, sizeof(uint32_t), 1, pf);
        if (n != 1) {
            error = ERR_READ;
            return;
        }
        n = fread(&offset, sizeof(uint32_t), 1, pf);
        if (n != 1) {
            error = ERR_READ;
            return;
        }
        uint32_t isize;
        n = fseek(sf, offset, SEEK_SET);
        if (n) {
            error = ERR_SEEK;
            return;
        }
        n = fread(&isize, sizeof(uint32_t), 1, sf);
        if (n != 1) {
            error = ERR_READ;
            return;
        }
        char *info_str = calloc(1, isize + 1);
        n = fread(info_str, sizeof(char), isize, sf);
        if (n != isize) {
            error = ERR_READ;
            return;
        }
        info_str[isize] = '\0';
        InfoType *info = info_init(info_str);
        if (error) {
            return;
        }
        insert(ht, key, info);
        if (error) {
            return;
        }
        info_free(info);
        free(info_str);
    }
}
