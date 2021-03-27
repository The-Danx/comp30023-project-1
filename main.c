#include "list.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <limits.h>

#define MAX_PROCESSORS 1024

typedef struct processorData ProcessorData;

struct processorData
{
    List *processList;
    Node *minimumTimeProcess;
    int minimumTime;
    int minimumProcessId;
    int procRemaining;
    bool processAvailable;
    bool processRestart;
    int id;
    int procFinished;
};

void simulateProcessors(char *fileName, int processorCount);
struct processorData simulateProcessor(struct processorData processor, int time);
void printPerformanceStatistics(List *processList, int time);
List *parseFile(char *fileName);
float roundFloat(float num);

int globalProcRemaining;
int currentProcRemaining;

int main(int argc, char **argv)
{

    // Process command line arguments
    // int cflag = 0;
    char *fvalue;
    int pvalue;
    int index;
    int c;

    opterr = 0;

    while ((c = getopt(argc, argv, "f:p:c")) != -1)
        switch (c)
        {
        case 'f':
            fvalue = optarg;
            break;
        case 'p':
            pvalue = atoi(optarg);
            if (pvalue < 1 || pvalue > MAX_PROCESSORS)
            {
                fprintf(stderr, "%d, is an invalid number of processors\n", pvalue);
                return 1;
            }
            break;
        case 'c':
            // cflag = 1;
            break;
        case '?':
            if (optopt == 'c' || optopt == 'p')
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint(optopt))
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf(stderr,
                        "Unknown option character `\\x%x'.\n",
                        optopt);
            return 1;
        default:
            abort();
        }

    for (index = optind; index < argc; index++)
    {
        printf("Non-option argument %s\n", argv[index]);
    }

    simulateProcessors(fvalue, pvalue);

    return 0;
}

void simulateProcessors(char *fileName, int processorCount)
{
    List *processList = parseFile(fileName);
    // printf("Yeet");
    // processList = listInsertionSort(processList);
    // printf("Yeet");
    struct processorData processorDataArray[processorCount];
    for (int i = 0; i < processorCount; i++)
    {
        processorDataArray[i].processList = newList();
        processorDataArray[i].minimumTime = INT_MAX;
        processorDataArray[i].minimumProcessId = INT_MAX;
        processorDataArray[i].procRemaining = 0;
        processorDataArray[i].processAvailable = false;
        processorDataArray[i].processRestart = true;
        processorDataArray[i].id = i;
        processorDataArray[i].procFinished = -1;
    }

    int time = 0;
    globalProcRemaining = processList->size;

    while (globalProcRemaining > 0)
    {
        Node *node = processList->head;

        while (node != NULL)
        {
            // Find new processes
            if (node->data.timeArrived <= time && !node->data.complete)
            {
                int minimumProcessor;
                int minimumTime = INT_MAX;
                // printNode(node);
                // Add new process to minimum time remaing processor
                for (int i = 0; i < processorCount; i++)
                {
                    if (processorDataArray[i].processList->remainingTime < minimumTime)
                    {
                        // printf("%d\n", i);
                        minimumTime = processorDataArray[i].processList->remainingTime;
                        minimumProcessor = i;
                        // processorDataArray[i].procRemaining++;
                    }
                }
                listAppend(processorDataArray[minimumProcessor].processList, node->data);
                node->data.complete = true;
                processorDataArray[minimumProcessor].procRemaining++;
                currentProcRemaining++;
            }
            node = node->next;
        }

        for (int i = 0; i < processorCount; i++)
        {
            processorDataArray[i] = simulateProcessor(processorDataArray[i], time);
        }
        for (int i = 0; i < processorCount; i++)
        {
            if (processorDataArray[i].procFinished != -1)
            {
                printf("%d,FINISHED,pid=%d,proc_remaining=%d\n", time + 1, processorDataArray[i].procFinished, currentProcRemaining);
                processorDataArray[i].procFinished = -1;
            }
        }
        time++;
    }

    List *finishedProcessList = newList();
    for (int i = 0; i < processorCount; i++)
    {
        Node *node = processorDataArray[i].processList->head;
        while (node != NULL)
        {
            listAppend(finishedProcessList, node->data);
            node = node->next;
        }
    }

    printPerformanceStatistics(finishedProcessList, time);

    free(processList);
    return;
}

struct processorData simulateProcessor(struct processorData processor, int time)
{
    // printf("Pre min: %p ", processor.minimumTimeProcess);
    // printNode(processor.minimumTimeProcess);
    Node *node = processor.processList->head;
    while (node != NULL)
    {
        // printNode(node);
        // printf("%d", time);
        // Find minimum time remaing process
        if (node->data.timeArrived <= time && node->data.remainingTime <= processor.minimumTime && node->data.remainingTime > 0 && node != processor.minimumTimeProcess)
        {
            if (node->data.remainingTime == processor.minimumTime && node->data.processId > processor.minimumProcessId)
            {
                node = node->next;
                continue;
            }
            // printNode(node);
            processor.minimumTimeProcess = node;
            processor.minimumTime = node->data.remainingTime;
            processor.minimumProcessId = node->data.processId;
            processor.processAvailable = true;
            processor.processRestart = true;
        }
        node = node->next;
    }
    if (!processor.processAvailable)
    {
        return processor;
    }

    // Update process
    if (processor.processRestart)
    {
        printf("%d,RUNNING,pid=%d,remaining_time=%d,cpu=%d\n", time, processor.minimumTimeProcess->data.processId, processor.minimumTimeProcess->data.remainingTime, processor.id);
        processor.processRestart = false;
    }
    processor.minimumTimeProcess->data.remainingTime--;
    processor.processList->remainingTime--;
    processor.minimumTime = processor.minimumTimeProcess->data.remainingTime;
    if (processor.minimumTime == 0)
    {
        processor.procRemaining--;
        globalProcRemaining--;
        currentProcRemaining--;
        processor.procFinished = processor.minimumTimeProcess->data.processId;
        // printf("%d,FINISHED,pid=%d,proc_remaining=%d\n", time + 1, processor.minimumTimeProcess->data.processId, currentProcRemaining);
        processor.minimumTime = INT_MAX;
        processor.minimumProcessId = INT_MAX;
        processor.minimumTimeProcess->data.complete = true;
        processor.processAvailable = false;
        processor.minimumTimeProcess->data.waitingTime = time + 1 - processor.minimumTimeProcess->data.timeArrived - processor.minimumTimeProcess->data.executionTime;
        processor.processRestart = true;
    }
    // printf("Post min: %p ", processor.minimumTimeProcess);
    // printNode(processor.minimumTimeProcess);
    return processor;
}

void printPerformanceStatistics(List *processList, int time)
{
    int totalTurnaroundTime = 0;
    int turnaroundTime = 0;
    float timeOverhead[processList->size];
    float maxTimeOverhead = 0;
    Node *node = processList->head;
    int i = 0;
    while (node != NULL)
    {
        turnaroundTime = node->data.waitingTime + node->data.executionTime;
        totalTurnaroundTime += turnaroundTime;
        timeOverhead[i] = (float)turnaroundTime / (float)node->data.executionTime;
        if (timeOverhead[i] > maxTimeOverhead)
        {
            maxTimeOverhead = timeOverhead[i];
        }
        node = node->next;
        i++;
    }
    float total = 0;
    for (i = 0; i < processList->size; i++)
    {
        total += timeOverhead[i];
    }
    printf("Turnaround time %d\n", (totalTurnaroundTime + (processList->size - 1)) / processList->size);
    printf("Time overhead %g %g\n", roundFloat(maxTimeOverhead), roundFloat(total / processList->size));
    printf("Makespan %d\n", time);
    return;
}

List *parseFile(char *fileName)
{
    FILE *file = fopen(fileName, "r");

    if (file == NULL)
    {
        printf("File: %s does not exist.", fileName);
        exit(1);
    }

    // struct data listData;
    List *list = newList();

    int timeArrived;
    int processId;
    int executionTime;
    char parallelisableChar;
    while (fscanf(file, "%d %d %d %c", &timeArrived, &processId, &executionTime, &parallelisableChar) == 4)
    {
        struct data listData = {timeArrived, processId, executionTime, false, false, -1, executionTime};
        sortedInsert(list, listData);
    }
    return list;
}

float roundFloat(float num)
{
    float returnNum = (int)(num * 100 + 0.5);
    return (float)returnNum / 100;
}