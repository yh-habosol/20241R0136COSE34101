#include "main.h"


void enqueue(int queue[], int *front, int *rear, int pid) {
    queue[*rear] = pid;
    *rear = (*rear + 1) % (MAX_QUEUE_SIZE + 1);
}

int dequeue(int queue[], int *front, int *rear) {
    int pid = queue[*front];
    *front = (*front + 1) % (MAX_QUEUE_SIZE + 1);
    return pid;
}