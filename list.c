#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

List *newList() {
    List *list = malloc(sizeof(*list));
    assert(list);

    list->head = NULL;
    list->size = 0;

    return list;
}

void freeList(List *list) {
    while (list->size > 0) {
        listPop(list);
    }

    free(list);
}

Node *newNode(Data data) {
    Node *node = malloc(sizeof(*node));
    assert(node);

    node->next = NULL;
    node->data = data;

    return node;
}

void freeNode(Node *node) {
    free(node);
}

void listAppend(List *list, Data data) {
    Node *new = newNode(data);

    if (list->size > 0) {
        Node *oldBottom = list->head;
        while (oldBottom->next != NULL) {
            oldBottom = oldBottom->next;
        }
        oldBottom->next = new;
    }
    else {
        list->head = new;
    }

    list->size++;
}

Data listPop(List *list) {
    if (list->size == 0) {
        printf("Can't pop from empty list.");
        exit(1);
    }

    Node *oldHead = list->head;
    Data data = oldHead->data;

    if (list-> size == 1) {
        list->head = NULL;
    }
    else {
        list->head = oldHead->next;
    }

    list->size--;
    free(oldHead);
    return data;
}

void printList(List *list) {
    struct data listData;
    if (list->size > 0) {
        Node *node = list->head;
        while (node != NULL) {
            listData = node->data;
            printf("%d %d %d %d %d %d\n", listData.timeArrived, listData.processId, listData.executionTime, listData.parallelisable, listData.complete, listData.waitingTime);
            node = node->next;
        }
    }
    return;
}