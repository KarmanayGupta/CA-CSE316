#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
pthread_mutex_t resourcesshouldbelocked;
pthread_cond_t preconditionstobefollowed;
bool safe_sequence_hiest();
void *final_function_bankersalgorithm(void *);
int numberofresources, numberofprocesses, *totalnumberofresources, **resourcestobeallocated, **maxresreq, **resourcesneeded, *sequencesafe;
int ran_processes = 0;
int main(int argc, char **argv)
{
    printf("***WELCOME TO THE ALGORITHM***");
    printf("***REGARDS***");
    printf("***KARMANAY GUPTA***");
    printf("***K21FG***");
    printf("***RK21FGA07***");
    srand(time(NULL));
    printf("\nPLEASE ENTER THE NUMBER OF PROCESSES YOU WANT TO EXECUTE  --> ");
    scanf("%d", &numberofprocesses);
    printf("\nPLEASE ENTER NAME OF PROCESSES THAT YOU WANTED TO EXECUTE --> ");
    scanf("%d", &numberofresources);
    totalnumberofresources = (int *)malloc(numberofresources * sizeof(*totalnumberofresources));
    printf("\nPLEASE ENTER THE NUMBER OF RESOURCES YOU WANT TO ALLOCATE ");
    printf("\nENTER THE VALUES IN THE FORM OF int space int space int   --> ");
    for (int i = 0; i < numberofresources; i++)
    {
        scanf("%d", &totalnumberofresources[i]);
    }
    resourcestobeallocated = (int **)malloc(numberofprocesses * sizeof(*resourcestobeallocated));
    for (int i = 0; i < numberofprocesses; i++)
    {
        resourcestobeallocated[i] = (int *)malloc(numberofresources * sizeof(**resourcestobeallocated));
    }

    maxresreq = (int **)malloc(numberofprocesses * sizeof(*maxresreq));
    for (int i = 0; i < numberofprocesses; i++)
    {
        maxresreq[i] = (int *)malloc(numberofresources * sizeof(**maxresreq));
    }
    printf("\n");
    for (int i = 0; i < numberofprocesses; i++)
    {
        printf("\nThe number of resources allocated to processes are as follows --> %d ", i + 1);
        for (int j = 0; j < numberofresources; j++)
        {
            scanf("%d", &resourcestobeallocated[i][j]);
        }
    }
    printf("\n");
    for (int i = 0; i < numberofprocesses; i++)
    {
        printf("\nThe total number of maximum resources required by processes are --> %d ", i + 1);
        for (int j = 0; j < numberofresources; j++)
        {
            scanf("%d", &maxresreq[i][j]);
        }
    }
    printf("\n");
    resourcesneeded = (int **)malloc(numberofprocesses * sizeof(*resourcesneeded));
    for (int i = 0; i < numberofprocesses; i++)
        resourcesneeded[i] = (int *)malloc(numberofresources * sizeof(**resourcesneeded));
    for (int i = 0; i < numberofprocesses; i++)
    {
        for (int j = 0; j < numberofresources; j++)
        {
            resourcesneeded[i][j] = maxresreq[i][j] - resourcestobeallocated[i][j];
        }
    }
    sequencesafe = (int *)malloc(numberofprocesses * sizeof(*sequencesafe));
    for (int i = 0; i < numberofprocesses; i++)
        sequencesafe[i] = -1;

    if (!safe_sequence_hiest())
    {
        printf("\nWARNING!!!!!!!!Unsafe State!!!!!!!\n");
        exit(-1);
    }

    printf("\n\nSafe Stete Verified!!!!!!! : ");
    for (int i = 0; i < numberofprocesses; i++)
    {
        printf("%-3d", sequencesafe[i] + 1);
    }
    printf("\nPROCESS UNDER EXECUTION!!!!\n");
    sleep(1);
    pthread_t processes[numberofprocesses];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int processNumber[numberofprocesses];
    for (int i = 0; i < numberofprocesses; i++)
        processNumber[i] = i;
    for (int i = 0; i < numberofprocesses; i++)
        pthread_create(&processes[i], &attr, final_function_bankersalgorithm, (void *)(&processNumber[i]));
    for (int i = 0; i < numberofprocesses; i++)
        pthread_join(processes[i], NULL);
    printf("\nAll Processes Finished\n");
    free(totalnumberofresources);
    for (int i = 0; i < numberofprocesses; i++)
    {
        free(resourcestobeallocated[i]);
        free(maxresreq[i]);
        free(resourcesneeded[i]);
    }
    free(resourcestobeallocated);
    free(maxresreq);
    free(resourcesneeded);
    free(sequencesafe);
}

bool safe_sequence_hiest()
{
    int tempRes[numberofresources];
    for (int i = 0; i < numberofresources; i++)
        tempRes[i] = totalnumberofresources[i];

    bool finished[numberofprocesses];
    for (int i = 0; i < numberofprocesses; i++)
        finished[i] = false;
    int nfinished = 0;
    while (nfinished < numberofprocesses)
    {
        bool safe = false;

        for (int i = 0; i < numberofprocesses; i++)
        {
            if (!finished[i])
            {
                bool possible = true;

                for (int j = 0; j < numberofresources; j++)
                    if (resourcesneeded[i][j] > tempRes[j])
                    {
                        possible = false;
                        break;
                    }

                if (possible)
                {
                    for (int j = 0; j < numberofresources; j++)
                        tempRes[j] += resourcestobeallocated[i][j];
                    sequencesafe[nfinished] = i;
                    finished[i] = true;
                    ++nfinished;
                    safe = true;
                }
            }
        }

        if (!safe)
        {
            for (int k = 0; k < numberofprocesses; k++)
                sequencesafe[k] = -1;
            return false;
        }
    }
    return true;
}

void *final_function_bankersalgorithm(void *arg)
{
    int p = *((int *)arg);
    pthread_mutex_lock(&resourcesshouldbelocked);
    while (p != sequencesafe[ran_processes])
    {
        pthread_cond_wait(&preconditionstobefollowed, &resourcesshouldbelocked);
    }
    printf("\n~~~~ Process %d", p + 1);
    printf("\n\tRESOURCES ALLOCATED --> ");
    for (int i = 0; i < numberofresources; i++)
    {
        printf("%3d", resourcestobeallocated[p][i]);
    }
    printf("\n\tRESOURCES NEEDED    -->");
    for (int i = 0; i < numberofresources; i++)
    {
        printf("%3d", resourcesneeded[p][i]);
    }
    printf("\n\tRESOURCES AVAILABLE --> ");
    for (int i = 0; i < numberofresources; i++)
    {
        printf("%3d", totalnumberofresources[i]);
    }
    printf("\n");
    sleep(1);
    printf("\tRESOURCES ALLOCATED SUCCESSFULLY");
    printf("\n");
    sleep(1);
    printf("\tPROCESS COMPLETED");
    printf("\n");
    sleep(1);
    printf("\tUSED RESOURCES RELEASED!");
    for (int i = 0; i < numberofresources; i++)
    {
        totalnumberofresources[i] += resourcestobeallocated[p][i];
    }
    printf("\n\tRESOURCES NOW AVAILABLE TO BE USED AGAIN --> ");
    for (int i = 0; i < numberofresources; i++)
    {
        printf("%3d", totalnumberofresources[i]);
    }
    printf("\n\n");
    sleep(1);
    printf("***THANKS FOR USING THE ALGORITHM***");
    printf("***REGARDS***");
    printf("***KARMANAY GUPTA***");
    ran_processes++;
}
