#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PROCESSES 10


struct process{
    int pid;
    int arrivalTime;
    int totalCPUTime;
    int ioFrequency;
    int ioDuration;
};

int inputSize;
int readySize;
int runningSize;
int waitingSize;

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
void printProcesses(struct process processesCopy[], int size){
  printf(".........PRINTING NEW PROCESS ARRAY.......... \n");
  for(int i = 0; i < size; i++){
    printf("PID: %d \n", processesCopy[i].pid);
    printf("Arrival Time:: %d \n", processesCopy[i].arrivalTime);
    printf("Total CPU Time: %d \n", processesCopy[i].totalCPUTime);
    printf("I/O Frequency: %d \n", processesCopy[i].ioFrequency);
    printf("I/O Duration: %d \n \n", processesCopy[i].ioDuration);
  }
}

/* Appends a process to a process queue */
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


/* Deletes the first elemeent in a precess queue */
void queueDelete(struct process queue[], int *rearA, int *frontA, int *size){
  int rear = *rearA;
  int front = *frontA;
  if(front == -1 || front > rear){
    printf("ERROR: Queue Underflow \n");
  }
  else{
    (*frontA)++;
    (*size)--;
    //Needs logic to remove first element of Queue
    for(int i = 0; i < *size; i++ ){
      queue[i] = queue[i + 1];
    }
  }
}



int main(){
  //parses input.txt
  struct process inputProcesses[MAX_PROCESSES]; //MAX NUMBER OF INPUT PROCESSES = 10
  parseInput(inputProcesses);
  printProcesses(inputProcesses, inputSize);


  //Setting up state queues
  struct process readyQueue[MAX_PROCESSES];
  struct process runningQueue[MAX_PROCESSES];
  struct process waitingQueue[MAX_PROCESSES];
  int rearReady = -1;
  int frontReady = -1;
  int rearRunning = -1;
  int frontRunning = -1;
  int rearWaiting = -1;
  int frontWaiting = -1;

  queueInsert(readyQueue, inputProcesses[0],&rearReady,&frontReady, &readySize);
  queueInsert(readyQueue, inputProcesses[1],&rearReady,&frontReady, &readySize);
  printProcesses(readyQueue,readySize);
  queueDelete(readyQueue, &rearReady, &frontReady, &readySize);
  printProcesses(readyQueue,readySize);


  /*

  //Setting up ouput to file
  FILE *outputFile;
  outputFile = fopen("output.txt", "w");
  fprintf(outputFile, "| Time of transition | PID   | Old State   | New State   | \n");
  fprintf(outputFile, "Starting PID: %d \n", inputProcesses[0].pid);

  clock_t uptime = clock() / (CLOCKS_PER_SEC / 1000);

  */

  return 0;
}
