// INTEGRITY STATEMENT (modify if necessary)
// I received help from the following sources:
// None. I am the sole author of this work 

// sign this statement by removing the line below and entering your name

// Name: Aditya Maru
// login ID: a2maru

// implementation:

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pq.h"


// see pq.h for details
struct pq *pq_create(void)
{
    struct pq *new_pq = malloc(sizeof(struct pq));
    new_pq->max_length = 100;
    new_pq->pq_arr = malloc(new_pq->max_length * sizeof(struct hnode *));
    new_pq->length = 0;
    return new_pq;
}


// see pq.h for details
void pq_add(struct pq *pqueue, int item, int priority)
{
    assert(pqueue != NULL);
    int length = pqueue->length;
    int parent;
    int child;
    if(length + 1 == pqueue->max_length)
    {
        pqueue->max_length = 2 * pqueue->max_length;
        pqueue->pq_arr = 
           realloc(pqueue->pq_arr, pqueue->max_length * sizeof(struct hnode *));
    }
    struct hnode *new_node = malloc(sizeof(struct hnode));
    new_node->priority = priority;
    new_node->item = item;
    if(pqueue->length == 0)
    {
        (pqueue->pq_arr)[0] = new_node;
        ++(pqueue->length);
        return;
    }
    else
    {
        child = length;
        parent = (child - 1) / 2;
        (pqueue->pq_arr[length]) = new_node;
        while(parent >= 0 && (priority < ((pqueue->pq_arr)[parent])->priority))
        {
            struct hnode *backup = new_node;
            new_node = pqueue->pq_arr[parent];
            pqueue->pq_arr[parent] = backup;
            child = parent;
            parent = (child - 1) / 2;
        }
        (pqueue->pq_arr[child]) = new_node;
        ++(pqueue->length);
    }
}


// see pq.h for details
int pq_remove(struct pq *pqueue)
{
    assert(pqueue != NULL);
    assert(pqueue->length > 0);
    int left_child;
    int right_child;
    int index = 0;
    int length = pqueue->length;
    int ret_val = (pqueue->pq_arr[0])->item;
    struct hnode *back = pqueue->pq_arr[0];
    pqueue->pq_arr[0] = pqueue->pq_arr[length - 1];
    free(back);
    --pqueue->length;
    left_child = index*2 + 1;
    right_child = index*2 + 1;
    while((left_child < length && right_child < length) &&
          (((pqueue->pq_arr[index])->priority >
            (pqueue->pq_arr[left_child])->priority)
           || 
           ((pqueue->pq_arr[index])->priority >
            (pqueue->pq_arr[right_child])->priority)))
    {
        if((pqueue->pq_arr[left_child])->priority < 
           (pqueue->pq_arr[right_child])->priority)
        {
            struct hnode *backup = pqueue->pq_arr[index];
            pqueue->pq_arr[index] = pqueue->pq_arr[left_child];
            pqueue->pq_arr[left_child] = backup;
            index = left_child;
            left_child = index * 2 + 1;
            right_child = index * 2 + 2;
        }
        else
        {
            struct hnode *backup = pqueue->pq_arr[index];
            pqueue->pq_arr[index] = pqueue->pq_arr[right_child];
            pqueue->pq_arr[right_child] = backup;
            index = right_child;
            left_child = index * 2 + 1;
            right_child = index * 2 + 2;
        }
    }
    return ret_val;
}


// see pq.h for details
int pq_length(const struct pq *pqueue)
{
    assert(pqueue != NULL);
    return pqueue->length;
}


// see pq.h for details
int pq_front_priority(const struct pq *pqueue)
{
    assert(pqueue != NULL);
    return (pqueue->pq_arr[0])->priority;
}


// see pq.h for details
int pq_front_item(const struct pq *pqueue)
{
    assert(pqueue != NULL);
    return (pqueue->pq_arr[0])->item; 
}


// see pq.h for details
void pq_print(const struct pq *pqueue)
{
    assert(pqueue != NULL);
    printf("[");
    if(pqueue->length != 0)
    {
        printf("(%d,%d)", pq_front_item(pqueue), pq_front_priority(pqueue));
        for (int i = 1; i < pqueue->length; ++ i)
        {
            printf(",(%d,%d)", 
                   ((pqueue->pq_arr)[i])->item,
                   ((pqueue->pq_arr)[i])->priority);
        }
    }
    printf("]\n");
}


// see pq.h for details
void pq_destroy(struct pq *pqueue)
{
    int length = pqueue->length;
    for(int i = 0; i < length; ++ i)
    {
        free(pqueue->pq_arr[i]);
    }
    free(pqueue->pq_arr);
    free(pqueue);
}

// see pq.h for details
bool pq_is_empty(const struct pq *pqueue)
{
    assert(pqueue != NULL);
    if(pqueue->length == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
