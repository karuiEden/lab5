#include "queue_list.h"
#include <stdlib.h>
#include <unistd.h>

#include "error/error_handle.h"


queue* queue_init(){
    queue *q = calloc(1, sizeof(queue));
    q->tail = nullptr;
    return q;
}

void queue_delete(queue* q){
    if(!q->tail){
        return;
    }
    queue_node *ptr = q->tail, *ptr_prev = nullptr;
    while (!ptr->next){
        ptr_prev = ptr;
        ptr = ptr->next;
        free(ptr_prev);
    }
    free(q->tail);
    free(q);

}

void queue_insert(queue *queue, key_space* value){
    queue_node *new_node = calloc(1, sizeof(queue_node));
    if (new_node == NULL){
        error = ERR_ALLOC;
        return;
    }
    new_node->ks = value;
    if(queue->tail == NULL){
        new_node->next = new_node;
    } else {
        new_node->next = queue->tail->next;
        queue->tail->next = new_node;
    }
    queue->tail = new_node;
}



key_space* extract_min(queue *q) {
    if (!q) {
        error = ERR_PTR;
        return nullptr;
    }
    if (!q->tail) {
        error = FREE;
        return nullptr;
    }
    queue_node *min_data_ptr = nullptr, *data_ptr = q->tail->next, *prev = nullptr, *prev_min = nullptr;
    if (data_ptr->next == data_ptr) {
        key_space *data = data_ptr->ks;
        free(q->tail);
        q->tail = nullptr;
        return data;
    }
    uint32_t min_dist = UINT32_MAX;
    do {
        if (data_ptr->ks->info->dist < min_dist) {
            min_dist = data_ptr->ks->info->dist;
            min_data_ptr = data_ptr;
            prev_min = prev;
        }
        prev = data_ptr;
        data_ptr = data_ptr->next;
    } while (prev != q->tail);
    if (min_data_ptr == q->tail) {
        q->tail = prev_min;
        key_space *data = min_data_ptr->ks;
        free(min_data_ptr);
        return data;
    }
    if (prev_min) {
        prev_min->next = min_data_ptr->next;
    } else {
        q->tail->next = min_data_ptr->next;
    }
    key_space *data = min_data_ptr->ks;
    free(min_data_ptr);
    return data;
}

bool in_queue(queue *q, const char* value) {
    if (!q || !value) {
        error = ERR_PTR;
        return false;
    }
    if (!q->tail) {
        error = FREE;
        return false;
    }
    queue_node *ptr = q->tail->next;
    do {
        if (!strcmp(ptr->ks->key, value)) {
            return true;
        }
        ptr = ptr->next;
    } while (ptr != q->tail->next);
    return false;
}

key_space* queue_read(queue *q){
    if(!q->tail){
        return nullptr;
    }
    queue_node *ptr = q->tail->next;
    key_space *data = ptr->ks;
    if(ptr->next == ptr){
        free(q->tail);
        q->tail = nullptr;
    } else {
        q->tail->next = ptr->next;
        free(ptr);
    }
    return data;
}

void queue_print(const queue q){
    if(q.tail) {
        const queue_node *ptr = q.tail->next;
        do {
            printf("%s ", ptr->ks->key);
            ptr = ptr->next;
        } while (ptr != q.tail->next);
    }
    printf("\n");
}