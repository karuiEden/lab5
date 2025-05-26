#ifndef DSAA2_QUEUE_H
#define DSAA2_QUEUE_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "vertex.h"
#include "maps/ks.h"


typedef struct queue_node{
    key_space* ks;
    struct queue_node *next;
} queue_node;

typedef struct queue{
    queue_node *tail;
} queue;




enum capacity{
    FULL = -3,
    FREE,
};

queue* queue_init();



key_space* queue_read(queue *q);

void queue_delete(queue *q);

void queue_print(queue q);

void queue_insert(queue* queue, key_space* value);

#endif //DSAA2_QUEUE_H
