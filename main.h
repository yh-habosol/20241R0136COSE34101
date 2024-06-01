#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESSES 100
#define MAX_QUEUE_SIZE 100
#define PID_MIN 1000
#define PID_MAX 9999


typedef struct {
    int pid;
    int arrivalTime;

    int cpuBurstTime;
    int ioBurstTime;
    int ioBurstTiming;
    
    int priority;   

    int waitingStartTime;    
    int waitingTime;
    int turnaroundTime;
    
    int cpuRemainingTime;
    int ioRemainingTime;

} Process;


extern Process processes[MAX_PROCESSES];
extern Process originalProcesses[MAX_PROCESSES];
extern int processMap[PID_MAX + 1];
extern int processCount;


//scheduling algorithm
void fcfs();
void nonPreemptiveSJF();
void preemptiveSJF();
void nonPreemptivePriority();
void preemptivePriority();
void roundRobin(int timeQuantum);
void ioBurst(int queue[], int *front, int *rear);



//process 
void createProcess(int processNum);
void resetProcesses();
void printProcesses();
void evaluate();
void ganttChart(int chart[], int size);




//queue 
void enqueue(int queue[], int *front, int *rear, int pid);
int dequeue(int queue[], int *front, int *rear);





#endif // MAIN_H