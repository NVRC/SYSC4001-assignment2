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

void parseInput(){
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
  {   //check print
      int i,j;
      //int (*matrix)[col]=array;
      int numberStruct;
      //Testing the output of parseInput
      for(i=0;i<row;++i){
          for(j=0;j<col;++j)
              printf("%d ", array[i*col + j]);//matrix[i][j]
              numberStruct++;
              printf("\n");
      }

      struct process processes[numberStruct];

      int temp[5], x;
      for(int y = 0; y < sizeof(array); y+=5){
        for(int z = 0; z < 5; z++){
          temp[z] = array[z+y];
        }
        processes[x] = {temp[0],temp[1],temp[2],temp[3],temp[4]};
        x++;
      }
  }
exit:
  fclose(fp);
  free(array);
}

int main(){
  parseInput();
  return 0;
}
