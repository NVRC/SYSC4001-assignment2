#include<stdio.h>
#include <stdlib.h>
#include <string.h>

struct process{
    int pid;
    int arrivalTime;
    int totalCPUTime;
    int ioFrequency;
    int ioDuration;
};

int numberOfRows;

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



      int temp[5], x;
      for(int y = 0; y < sizeof(array); y+=5){
        for(int z = 0; z < 5; z++){
          temp[z] = array[z+y];
        }
        processes[x].pid = temp[0];
        processes[x].arrivalTime = temp[1];
        processes[x].totalCPUTime = temp[2];
        processes[x].ioFrequency = temp[3];
        processes[x].ioDuration = temp[4];
        x++;
      }
      numberOfRows = x-1;
  }

exit:
  fclose(fp);
  free(array);

}

void printProcesses(struct process processesCopy[]){
  for(int i = 0; i < numberOfRows; i++){
    printf("PID: %d \n", processesCopy[i].pid);
    printf("Arrival Time:: %d \n", processesCopy[i].arrivalTime);
    printf("Total CPU Time: %d \n", processesCopy[i].totalCPUTime);
    printf("I/O Frequency: %d \n", processesCopy[i].ioFrequency);
    printf("I/O Duration: %d \n \n", processesCopy[i].ioDuration);
  }
}

int main(){

  struct process inputProcesses[10];
  parseInput(inputProcesses);
  printProcesses(inputProcesses);
  return 0;
}
