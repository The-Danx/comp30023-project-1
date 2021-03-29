#include <stdbool.h>

#ifndef LIST_H
#define LIST_H

typedef struct data Data;

typedef struct list List;

typedef struct node Node;

struct data
{
    int timeArrived;
    float processId;
    int executionTime;
    bool parallelisable;
    bool parallelised;
    bool complete;
    int waitingTime;
    int remainingTime;
    List *parallelisedProcesses;
    int exitTime;
};

struct list
{
    Node *head;
    int size;
    int remainingTime;
};

struct node
{
    Node *next;
    Node *parallelisedNext;
    Data data;
};

List *newList();

void freeList(List *list);

Node *newNode(Data data);

void freeNode(Node *node);

void listAppend(List *list, Data data);

void listAppendPointer(List *list, Node *node);

Data listPop(List *list);

void sortedInsert(List *list, Data data);

void printList(List *list, bool parallelisable);

void printNode(Node *node);

#endif