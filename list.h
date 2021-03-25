#include <stdbool.h>

#ifndef LIST_H
#define LIST_H

typedef struct data Data;

typedef struct list List;

typedef struct node Node;

struct data
{
    int timeArrived;
    int processId;
    int executionTime;
    bool parallelisable;
    bool complete;
    int waitingTime;
    int remainingTime;
};

struct list
{
    Node *head;
    int size;
};

struct node
{
    Node *next;
    Data data;
};

List *newList();

void freeList(List *list);

Node *newNode(Data data);

void freeNode(Node *node);

void listAppend(List *list, Data data);

Data listPop(List *list);

void printList(List *list);

#endif