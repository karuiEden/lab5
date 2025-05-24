
#include "table.h"

bool check_prime(uint32_t n){
    for (uint32_t i = 2; i * i <= n; ++i) {
        if (n % i  == 0) return false;
    }
    return true;
}

uint32_t find_prime(uint32_t previous){
    uint32_t new = previous + 1;
    while (true) {
        if (check_prime(new)) return new;
        ++new;
    }
}

uint32_t hash1(int32_t key, uint32_t msize){
    uint32_t hash = INT32_MAX;
    for (; key > 0; key /= 10) {
        hash = hash * 37 + (key % 10);
    }
    return hash;
}

uint32_t hash2(int32_t key, uint32_t msize){
    uint32_t key_sum = 0;
    for (; key > 0; key /= 10) {
        key_sum += key % 10;
    }
    if (msize != 1) {
        return 1 + (key_sum % (msize - 1));
    }
    return 1 + (key_sum % msize);
}

uint32_t hash(const int32_t key, const uint32_t index, const uint32_t msize){
    return (hash1(key, msize) + index * hash2(key, msize)) % msize;
}

void table_resize(Table *table){
    const uint32_t new_size = find_prime(table->msize);
    KeySpace *new_ks = keyspace_create(new_size);
    if (!new_ks){
        error = ERR_ALLOC;
        return;
    }
    for (uint32_t i = 0; i < table->msize; ++i) {
        if (table->ks[i].busy == FULL){
            uint32_t index;
            for (uint32_t j = 0; j < table->msize; ++j) {
                index = hash(table->ks[i].key, j, new_size);
                if (new_ks[index].busy != FULL){
                    break;
                }
            }
            keyspace_insert(&new_ks[index], table->ks[i].key, table->ks[i].info, table->ks[i].release);
            keyspace_delete(&table->ks[i]);
        }
    }
    free(table->ks);
    table->ks = new_ks;
    table->msize = new_size;
}

Table* table_create(const uint32_t msize){
    Table* table = calloc(1, sizeof(Table));
    if(!table){
        error = ERR_ALLOC;
        return table;
    }
    table->msize = msize;
    table->csize = 0;
    table->ks = keyspace_create(msize);
    if(error != OK){
        return nullptr;
    }
    return table;
}

void table_delete(Table *table){
    for (uint32_t i = 0; i < table->msize; ++i) {
        if (table->ks[i].busy == FULL) {
            keyspace_delete(&table->ks[i]);
        }
    }
    free(table->ks);
    free(table);
}

void table_insert(Table* table, const int32_t key, const int32_t value){
    if (table->csize == table->msize){
        table_resize(table);
    }
    uint32_t release = 0;
    uint32_t index = 0;
    for (uint32_t i = 0; i < table->msize; ++i) {
        index = hash(key, i, table->msize);
        if(table->ks[index].busy == FULL && key ==  table->ks[index].key){
            ++release;
        }
        if (table->ks[index].busy != FULL){
            break;
        }
    }
    keyspace_insert(&table->ks[index], key, value, release);
    table->csize++;
}

// KeySpace* table_search(const Table* const table, int32_t key, uint32_t *size){
//     if(table->csize == 0){
//         error = EMPTY_TABLE;
//         return nullptr;
//     }
//     uint32_t index, add = 0;
//     *size = 0;
//     for (uint32_t i = 0; i < table->msize; ++i) {
//         index = hash(key, i, table->msize);
//         if (table->ks[index].busy == FULL && key == table->ks[index].key) {
//             ++(*size);
//         }
//     }
//     if(*size == 0){
//         error = ERR_INDEX;
//         return nullptr;
//     }
//     KeySpace *ans = calloc(*size, sizeof(KeySpace));
//     if(!ans){
//         return nullptr;
//     }
//     add = *size;
//     for (uint32_t i = 0; i < table->msize && add != 0; ++i) {
//         index = hash(key, i, table->msize);
//
//         if (table->ks[index].busy == FULL && key == table->ks[index].key) {
//             --add;
//             keyspace_insert(&ans[add], table->ks[index].key, table->ks[index].info, table->ks[index].release);
//         }
//     }
//     return ans;
// }

int32_t max_release_key(const Table* const table) {
    int32_t max_release = -1;
    int32_t max = 0;
    for (uint32_t i = 0; i < table->msize; ++i) {
        if (table->ks[i].busy == FULL && (int32_t)table->ks[i].release > max_release) {
            max_release = (int32_t)table->ks[i].release;
            max = table->ks[i].key;
        }
    }
    return max;
}

// InfoType* table_remove(Table* const table, const char* key){
//     if(table->csize == 0){
//         error = EMPTY_TABLE;
//         return NULL;
//     }
//     bool ready = false;
//     uint32_t index = 0;
//     for (uint32_t release = 0; release < table->msize; ++release) {
//         index = hash(key, release, table->msize);
//         if(table->ks[index].busy == FULL && cmp(key, table->ks[index].key) == 0){
//             ready = true;
//             break;
//         }
//     }
//     if (!ready){
//         error = ERR_INDEX;
//         return NULL;
//     }
//     InfoType* value = info_create(get_info(table->ks[index].info));
//     keyspace_delete(&table->ks[index]);
//     table->csize--;
//     return value;
// }

// KeySpace* table_search(const Table* const table, const char* key, uint32_t *size){
//     if(table->csize == 0){
//         error = EMPTY_TABLE;
//         return NULL;
//     }
//     uint32_t index, add = 0;
//     *size = 0;
//     for (uint32_t i = 0; i < table->msize; ++i) {
//         index = hash(key, i, table->msize);
//         if (table->ks[index].busy == FULL && cmp(key, table->ks[index].key) == 0) {
//             ++(*size);
//         }
//     }
//     if(*size == 0){
//         error = ERR_INDEX;
//         return NULL;
//     }
//     KeySpace *ans = calloc(*size, sizeof(KeySpace));
//     if(!ans){
//         return NULL;
//     }
//     add = *size;
//     for (uint32_t i = 0; i < table->msize && add != 0; ++i) {
//         index = hash(key, i, table->msize);
//
//         if (table->ks[index].busy == FULL && cmp(key, table->ks[index].key) == 0) {
//             --add;
//             keyspace_insert(&ans[add], table->ks[index].key, table->ks[index].info->info, table->ks[index].release);
//         }
//     }
//     return ans;
// }

// void table_print(const Table* const table){
//     printf("Current size: %u Total size: %u\n", table->csize, table->msize);
//     printf("\n");
//     for (uint32_t i = 0; i < table->msize; ++i) {
//         if(table->ks[i].busy == FULL){
//             printf("%u.", i);
//             keyspace_print(&table->ks[i]);
//         }
//     }
//     printf("\n");
// }

// void table_export(const Table *const table, FILE *out) {
//     uint32_t len_str;
//     fseek(out, 0, SEEK_SET);
//     uint32_t sc = fwrite("LaLaLa", 6, 1, out);
//     if (sc != 1) {
//         error = ERR_WRITE;
//         return;
//     }
//     sc = fwrite(&table->csize, sizeof(uint32_t), 1, out);
//     if (sc != 1) {
//         error = ERR_WRITE;
//         return;
//     }
//     for (uint32_t i = 0; i < table->msize; i++) {
//         if(table->ks[i].busy == FULL) {
//             len_str = strlen(table->ks[i].key);
//             sc = fwrite(&len_str, sizeof(uint32_t), 1, out);
//             if (sc != 1) {
//                 error = ERR_WRITE;
//                 return;
//             }
//             sc = fwrite(table->ks[i].key, 1, len_str + 1, out);
//             if (sc != len_str + 1) {
//                 error = ERR_WRITE;
//                 return;
//             }
//             len_str = strlen(table->ks[i].info->info);
//             sc = fwrite(&len_str, sizeof(uint32_t), 1, out);
//             if (sc != 1) {
//                 error = ERR_WRITE;
//                 return;
//             }
//             sc = fwrite(table->ks[i].info->info, 1, len_str + 1, out);
//             if (sc != len_str + 1) {
//                 error = ERR_WRITE;
//                 return;
//             }
//             sc = fwrite(&table->ks[i].release, sizeof(int32_t), 1, out);
//             if (sc != 1) {
//                 error = ERR_WRITE;
//                 return;
//             }
//         }
//     }
// }
//
// void table_import(Table* const table, FILE* stream){
//     uint32_t len;
//     char* check_type = calloc(7, sizeof(char));
//     uint32_t sc = fread(check_type, sizeof(char), 6, stream);
//     if(sc != 6){
//         error = ERR_READ;
//         return;
//     }
//     check_type[6] = '\0';
//     if(cmp(check_type, "LaLaLa") != 0){
//         error = ERR_FILE;
//         EMPTY_TABLE(check_type);
//         return;
//     }
//     EMPTY_TABLE(check_type);
//     sc = fread(&len, sizeof(uint32_t), 1, stream);
//     if(sc != 1){
//         error = ERR_READ;
//         return;
//     }
//     while (len > 0){
//         uint32_t len_str, release, index;
//         sc = fread(&len_str, sizeof(uint32_t), 1, stream);
//         if(sc != 1){
//             error = ERR_READ;
//             return;
//         }
//         char* key = calloc(len_str + 1, sizeof(char));
//         sc = fread(key, 1, len_str + 1, stream);
//         if(sc != len_str + 1){
//             error = ERR_READ;
//             return;
//         }
//         sc = fread(&len_str, sizeof(uint32_t), 1, stream);
//         if(sc != 1){
//             error = ERR_READ;
//             return;
//         }
//         char* value = calloc(len_str + 1, sizeof(char));
//         sc = fread(value, 1, len_str + 1, stream);
//         if(sc != len_str + 1){
//             error = ERR_READ;
//             return;
//         }
//         sc = fread(&release, sizeof(uint32_t), 1, stream);
//         if(sc != 1){
//             error = ERR_READ;
//             return;
//         }
//         bool ready = false;
//         while (!ready){
//             for (uint32_t i = 0; i < table->msize; ++i) {
//                 index = hash(key, i, table->msize);
//                 if (table->ks[index].busy != FULL){
//                     ready = true;
//                     break;
//                 }
//             }
//             if (!ready){
//                 table_resize(table);
//                 release = 0;
//             }
//         }
//         keyspace_insert(&table->ks[index], key, value, release);
//         ++table->csize;
//         if(table->csize == table->msize){
//             table_resize(table);
//         }
//         EMPTY_TABLE(key);
//         EMPTY_TABLE(value);
//         --len;
//     }
//     error = OK;
// }

// KeySpace* table_supersearch(const Table* const table, const char* key, const uint32_t release){
//     if(table->csize == 0){
//         error = EMPTY_TABLE;
//         return NULL;
//     }
//     uint32_t index = 0;
//     bool success = false;
//     for (uint32_t i = 0; i < table->msize; ++i) {
//         index = hash(key, i, table->msize);
//         if (table->ks[index].busy == FULL && cmp(key, table->ks[index].key) == 0 && table->ks[index].release == release) {
//             success = true;
//             break;
//         }
//     }
//     if(!success){
//         error = ERR_INDEX;
//         return NULL;
//     }
//     KeySpace *ans = calloc(1, sizeof(KeySpace));
//     if(!ans){
//         return NULL;
//     }
//     keyspace_insert(ans, table->ks[index].key, table->ks[index].info->info, table->ks[index].release);
//     return ans;
// }
