#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

List *newList()
{
    List *list = malloc(sizeof(*list));
    assert(list);

    list->head = NULL;
    list->size = 0;
    list->remainingTime = 0;

    return list;
}

void freeList(List *list)
{
    while (list->size > 0)
    {
        listPop(list);
    }

    free(list);
}

Node *newNode(Data data)
{
    Node *node = malloc(sizeof(*node));
    assert(node);

    node->next = NULL;

    if (data.parallelisable)
    {
        data.parallelisedProcesses = newList();
    }
    node->data = data;

    return node;
}

void freeNode(Node *node)
{
    free(node);
}

void listAppend(List *list, Data data)
{
    Node *new = newNode(data);

    if (list->size > 0)
    {
        Node *oldBottom = list->head;
        while (oldBottom->next != NULL)
        {
            oldBottom = oldBottom->next;
        }
        oldBottom->next = new;
    }
    else
    {
        list->head = new;
    }

    list->size++;
    list->remainingTime = list->remainingTime + new->data.executionTime;
}

void listAppendPointer(List *list, Node *node)
{
    if (list->size > 0)
    {
        Node *oldBottom = list->head;
        while (oldBottom->parallelisedNext != NULL)
        {
            oldBottom = oldBottom->parallelisedNext;
        }
        oldBottom->parallelisedNext = node;
    }
    else
    {
        list->head = node;
    }

    list->size++;
    list->remainingTime = list->remainingTime + node->data.executionTime;
}

Data listPop(List *list)
{
    if (list->size == 0)
    {
        printf("Can't pop from empty list.");
        exit(1);
    }

    Node *oldHead = list->head;
    Data data = oldHead->data;

    if (list->size == 1)
    {
        list->head = NULL;
    }
    else
    {
        list->head = oldHead->next;
    }

    list->size--;
    list->remainingTime = list->remainingTime - oldHead->data.remainingTime;
    free(oldHead);
    return data;
}

void sortedInsert(List *list, Data data)
{
    Node *node = newNode(data);
    if (list->size == 0 || (node->data.timeArrived < list->head->data.timeArrived) || (node->data.timeArrived == list->head->data.timeArrived && node->data.executionTime < list->head->data.executionTime) || (node->data.timeArrived < list->head->data.timeArrived) || (node->data.timeArrived == list->head->data.timeArrived && node->data.executionTime == list->head->data.executionTime && node->data.processId < list->head->data.processId))
    {
        node->next = list->head;
        list->head = node;
        list->size++;
        list->remainingTime = list->remainingTime + node->data.executionTime;
        return;
    }

    Node *currentNode = list->head;
    while (currentNode->next != NULL)
    {
        if ((node->data.timeArrived < currentNode->next->data.timeArrived) || (node->data.timeArrived == currentNode->next->data.timeArrived && node->data.executionTime < currentNode->next->data.executionTime) || (node->data.timeArrived < currentNode->next->data.timeArrived) || (node->data.timeArrived == currentNode->next->data.timeArrived && node->data.executionTime == currentNode->next->data.executionTime && node->data.processId < currentNode->next->data.processId))
        {
            node->next = currentNode->next;
            currentNode->next = node;
            list->size++;
            list->remainingTime = list->remainingTime + node->data.executionTime;
            return;
        }
        currentNode = currentNode->next;
    }
    node->next = currentNode->next;
    currentNode->next = node;
    list->size++;
    list->remainingTime = list->remainingTime + node->data.executionTime;
    return;
}

void printList(List *list, bool parallelisable)
{
    if (parallelisable)
    {
        if (list->size > 0)
        {
            Node *node = list->head;
            while (node != NULL)
            {
                printNode(node);
                node = node->parallelisedNext;
            }
        }
    }
    else
    {
        if (list->size > 0)
        {
            Node *node = list->head;
            while (node != NULL)
            {
                printNode(node);
                node = node->next;
            }
        }
    }
    return;
}

void printNode(Node *node)
{
    struct data listData = node->data;
    printf("%d %f %d %d %d %d %d\n", listData.timeArrived, listData.processId, listData.executionTime, listData.parallelisable, listData.complete, listData.waitingTime, listData.remainingTime);
    return;
}