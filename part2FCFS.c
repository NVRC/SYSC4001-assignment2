#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PROCESSES 100


struct process{
    int pid;
    long unsigned int arrivalTime;
    int totalCPUTime;
    int ioFrequency;
    int ioDuration;
    int timeRan;
    int ioTrackerID;
};


int inputSize;
int readySize;
int runningSize;
int waitingSize;
int endSize;

char readyString[] = "Ready";
char runningString[] = "Running";
char waitingString[] = "Waiting";
char inputString[] = "New";
char endString[] = "End";

void parseInput(struct process processes[]){
  FILE *fp;
  int data, row, col, c, count, inc;
  int *array, capacity = 10;
  char ch;

  array=(int*)malloc(sizeof(int)*capacity);
  fp=fopen("input.txt","r");
  row=col=c=count=0;

  while(EOF!=(inc=fscanf(fp,"%d%c", &data, &ch)) && inc == 2){
      ++c;//COLUMN count

      if(capacity == count)
          array = (int*)realloc(array, sizeof(int)*(capacity*=2));
      array[count++] = data;

      if(ch == '\n'){
          ++row;

          if(col == 0){
              col = c;
          }
          else if(col != c){
              fprintf(stderr, "ERROR: format error of different Column of Row at %d\n", row);
              goto exit;
          }

          c = 0;
      }
      else if(ch != ','){
          fprintf(stderr, "ERROR: format error of different separator(%c) of Row at %d \n", ch, row);
          goto exit;
      }
  }
  { //Print results and create an array of processes
      int i,j;
      //int (*matrix)[col]=array;
      int numberStruct;
      //Testing the output of parseInput
      for(i=0;i<row;++i){
          for(j=0;j<col;++j){
              printf("%d ", array[i*col + j]);//matrix[i][j]
          }
              numberStruct++;
              printf("\n");
      }



      int temp[5], x=0;
      for(int y = 0; y < row; ++y){
        for(int z = 0; z < col; ++z){
          temp[z] = array[y*col +  z];
        }
        processes[x].pid = temp[0];
        processes[x].arrivalTime = temp[1];
        processes[x].totalCPUTime = temp[2];
        processes[x].ioFrequency = temp[3];
        processes[x].ioDuration = temp[4];
        x++;
      }
      inputSize = x;
  }

exit:
  fclose(fp);
  free(array);

}

/* Prints the details of a process array */
void printProcesses(struct process processesCopy[], int size, int front){
  printf(".........PRINTING NEW PROCESS ARRAY.......... \n");
  for(int i = front; i < size; i++){
    printf("PID: %d \n", processesCopy[i].pid);
    printf("Arrival Time:: %lu \n", processesCopy[i].arrivalTime);
    printf("Total CPU Time: %d \n", processesCopy[i].totalCPUTime);
    printf("I/O Frequency: %d \n", processesCopy[i].ioFrequency);
    printf("I/O Duration: %d \n \n", processesCopy[i].ioDuration);
  }
}

void printProcess(struct process process){
  printf("PID: %d \n", process.pid);
  printf("Arrival Time:: %lu \n", process.arrivalTime);
  printf("Total CPU Time: %d \n", process.totalCPUTime);
  printf("I/O Frequency: %d \n", process.ioFrequency);
  printf("I/O Duration: %d \n \n", process.ioDuration);
}

/*  Appends a process to a process queue
    queueInsert template:
    queueInsert(readyQueue, inputProcesses[0],&rearReady,&frontReady, &readySize);

*/
void queueInsert(struct process queue[], struct process processAdd, int *rearA, int *frontA, int *size){
  int rear = *rearA;
  int front = *frontA;
  if(rear == MAX_PROCESSES -1){
    printf("ERROR: Queue Overflow \n");
  }
  else{
    if(front == -1){
      //If queue is initially empty
      *frontA = 0;
    }
    (*rearA)++;
    rear++;
    (*size)++;
    queue[rear] = processAdd;
  }
}


/*  Deletes the first elemeent in a precess queue
    queueDelete template:
    queueDelete(readyQueue, &rearReady, &frontReady, &readySize);

*/
void queueDelete(struct process queue[], int *rearA, int *frontA, int *size){
  int rear = *rearA;
  int front = *frontA;
  if(front == -1 || front > rear){
    printf("ERROR: Queue Underflow \n");
  }
  else{
    (*frontA)++;
  }
}

/*
  prints to output.txt
  printOutput template:
  printOutput(outputFile, 3, inputProcesses[1], readyString, runningString);
*/
void printOutput(FILE *output, long unsigned int time, struct process someProcess, char oldState[], char newState[]){
    printf("|  %lu   |  %d   |  %s   |  %s  | \n", time, someProcess.pid, oldState, newState );
    fprintf(output, "|  %lu   |  %d   |  %s   |  %s  | \n", time, someProcess.pid, oldState, newState );
}



int main(){
  //parses input.txt
  struct process inputProcesses[MAX_PROCESSES]; //MAX NUMBER OF INPUT PROCESSES = 10
  parseInput(inputProcesses);
  //print parsed input info
  printProcesses(inputProcesses, inputSize, 0);


  //Setting up state queues
  struct process readyQueue[MAX_PROCESSES];
  struct process running;
  struct process waitingQueue[MAX_PROCESSES];
  struct process endQueue[MAX_PROCESSES];
  struct process invalidRunning;
  invalidRunning.pid = -1;
  invalidRunning.totalCPUTime = 1000;
  running = invalidRunning;

  int rearReady = -1;
  int frontReady = -1;

  int rearWaiting = -1;
  int frontWaiting = -1;
  int rearEnd = -1;
  int frontEnd = -1;



  long unsigned int time = 0;

  int modInputSize = inputSize;

  int timerIO = 0;
  int timerIOWait[inputSize];
  int ioWaitTracker = 0;
  int  skipRead = 0;



  //Setting up ouput to file
  FILE *outputFile;
  outputFile = fopen("output.txt", "w");
  fprintf(outputFile, "| Time of transition | PID   | Old State   | New State   | \n");
  printf("| Time of transition | PID   | Old State   | New State   | \n");
  /*
    Insert primary logic here
  */

  while(modInputSize > 0){
    for (int i = skipRead; i < inputSize; i++) {
      if(inputProcesses[i].arrivalTime == time){
        //if the processes arrivalTime has been reached add to it to the ready queue

        queueInsert(readyQueue, inputProcesses[i], &rearReady, &frontReady, &readySize);
        printOutput(outputFile, time, inputProcesses[i], inputString, readyString);


        skipRead++;
      }
    }

    if(running.totalCPUTime == running.timeRan){
      //Remove process from running
      queueInsert(endQueue, running, &rearEnd, &frontEnd, &endSize);
      printOutput(outputFile, time, running, runningString, endString);
      running = invalidRunning;


      modInputSize--;
      if(modInputSize == 0){ goto end;}
    }

    if(running.pid < 0){ //Check if there isn't a process running

      if(abs(rearReady - frontReady) >= 0){ //Check if a process is in the Ready Queue

          running = readyQueue[frontReady];
          printOutput(outputFile, time, running, readyString, runningString);
          //Remove the process from the ready Queue
          queueDelete(readyQueue, &rearReady, &frontReady, &readySize);
      }
    }



    //Check for I/O
    if(running.ioFrequency == 0){
      //NO IO
    }
    else if (running.ioFrequency == timerIO){
      //Transfer the process from the running queue to waiting if IO is procd
      queueInsert(waitingQueue, running, &rearWaiting, &frontWaiting, &waitingSize);
      printOutput(outputFile, time, running, runningString, waitingString);
      running = invalidRunning;
      //Set a unique ID for IO tracking
      timerIOWait[ioWaitTracker] = 0;
      waitingQueue[rearWaiting].ioTrackerID = ioWaitTracker;
      ioWaitTracker++;
      timerIO = 0;
    }

    //Iterate over all waiting processes
    for (int i = frontWaiting; frontWaiting < rearWaiting;i++) {
      //Check a process's IO is over
      if(waitingQueue[i].ioDuration == timerIOWait[waitingQueue[i].ioTrackerID]){
        //Transfer the process from the waiting queue to ready queue if IO is done
        queueInsert(readyQueue, waitingQueue[i], &rearReady, &frontReady, &readySize);
        printOutput(outputFile, time, waitingQueue[i], waitingString, readyString);
        queueDelete(waitingQueue, &rearWaiting, &frontWaiting, &waitingSize);

        timerIOWait[i] = 0;
      }
    }





    //Increment times
    running.timeRan++;
    //printf("PID : %d  TIME RAN:%d\n",runningQueue[frontRunning].pid, runningQueue[frontRunning].timeRan);
    time++;
        timerIO++;
    //Increment all IO timers
    for (int i = 0; frontWaiting < rearWaiting;i++) {
      timerIOWait[i]++;
    }
  }



  end:

  fclose(outputFile);
  return 0;
}
