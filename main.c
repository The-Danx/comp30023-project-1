#include "list.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <limits.h>

#define MAX_PROCESSORS 1024

void simulateProcessor(char *fileName, int processorCount);
List *parseFile(char *fileName);
bool isProcessesDone(List *list);
float roundFloat(float num);

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
        printf("Non-option argument %s\n", argv[index]);

    simulateProcessor(fvalue, pvalue);

    return 0;
}

void simulateProcessor(char *fileName, int processorCount)
{
    List *processList = parseFile(fileName);

    int time = 0;
    Node *minimumTimeProcess;
    int minimumTime = INT_MAX;
    int minimumProcessId = INT_MAX;
    int procRemaining = processList->size;
    bool processAvailable = false;
    bool processRestart = true;
    while (!isProcessesDone(processList))
    {
        Node *node = processList->head;
        while (node != NULL)
        {
            // Find minimum time remaing process
            if (node->data.timeArrived <= time && node->data.remainingTime <= minimumTime && node->data.remainingTime > 0 && node != minimumTimeProcess)
            {
                if (node->data.remainingTime == minimumTime && node->data.processId > minimumProcessId)
                {
                    node = node->next;
                    continue;
                }
                minimumTimeProcess = node;
                minimumTime = node->data.remainingTime;
                minimumProcessId = node->data.processId;
                processAvailable = true;
                processRestart = true;
            }
            node = node->next;
        }
        if (!processAvailable)
        {
            time++;
            continue;
        }

        // Update process
        if (processRestart)
        {
            printf("%d,RUNNING,pid=%d,remaining_time=%d,cpu=%d\n", time, minimumTimeProcess->data.processId, minimumTimeProcess->data.remainingTime, 0);
            processRestart = false;
        }
        // printf("%d,RUNNING,pid=%d,remaining_time=%d,cpu=%d\n", time, minimumTimeProcess->data.processId, minimumTimeProcess->data.remainingTime, 0);
        minimumTimeProcess->data.remainingTime--;
        minimumTime = minimumTimeProcess->data.remainingTime;
        if (minimumTime == 0)
        {
            procRemaining--;
            printf("%d,FINISHED,pid=%d,proc_remaining=%d\n", time + 1, minimumTimeProcess->data.processId, procRemaining);
            minimumTime = INT_MAX;
            minimumProcessId = INT_MAX;
            minimumTimeProcess->data.complete = true;
            processAvailable = false;
            minimumTimeProcess->data.waitingTime = time + 1 - minimumTimeProcess->data.timeArrived - minimumTimeProcess->data.executionTime;
            processRestart = true;
        }

        time++;
    }

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

    free(processList);
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
        listAppend(list, listData);
    }

    return list;
}

bool isProcessesDone(List *list)
{
    if (list->size > 0)
    {
        Node *node = list->head;
        while (node != NULL)
        {
            if (node->data.complete == false)
            {
                return false;
            }
            node = node->next;
        }
    }
    return true;
}

float roundFloat(float num)
{
    float returnNum = (int)(num * 100 + 0.5);
    return (float)returnNum / 100;
}