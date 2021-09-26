#include <stdio.h>
#include <stdlib.h>

struct process{
  int AT;
  int BT;
  int CT;
  int WT;
  int TAT;
  int ET; //execution time that remains until process is done
};

void swap(struct process* a, struct process* b){
  struct process t = *a;
  *a = *b;
  *b = t;
}

void sort(struct process *list, int size){
  //bubble sort
  for (int i=0; i<size-1; i++){
    for (int k=0; k<size-i-1;k++){
      if (list[k].AT>list[k+1].AT) swap(&list[k], &list[k+1]);
    }
  }
}

void rr(struct process *list, int size, int quantum){
  sort(list, size);
  int clock = list[0].AT;
  int index = 0; //index of the next process to consider
  int done = 0; //number of done processes
  int time_remains = 0; //time remains before next quantum
  while (done != size){

    if (list[index].CT==clock&&clock!=0) {//if there is no available process
      clock++;
      time_remains=0;
      }

    if (list[index].AT<=clock&&list[index].ET!=0){

      if (time_remains!=0){ //if we use remainded time

        if (list[index].ET<time_remains) { //if time interval is too big
          time_remains = time_remains - list[index].ET;
          clock+=list[index].ET;
          list[index].ET=0;
        } else {
          list[index].ET -= time_remains;
          clock+=time_remains;
          time_remains=0;
        }

      } else { //if we use standard quantum

        if (list[index].ET<quantum) { //if time interval is too big
          time_remains = quantum - list[index].ET;
          clock+=list[index].ET;
          list[index].ET=0;
        } else {
          list[index].ET -= quantum;
          clock+=quantum;
        }

      }

      if (list[index].ET==0){ //if process is done
        list[index].CT = clock;
        list[index].TAT = list[index].CT - list[index].AT;
        list[index].WT = list[index].TAT - list[index].BT;
        done++;
      }

    }

    index = (index+1)%size; //go to next process
  }
  
}


int main(int argc, char**argv) {

  if (argc!=2) {
    printf("Invalid number of arguments!\n");
    return -1;
  }

  int q = atoi(argv[1]);

  FILE *fp;

  fp = fopen("new_input.csv", "r");

  struct process list[10];

  char buffer[100];

  int a = 0, d = 0;
  int i=0;

  while (fscanf(fp, "%d,%d", &a, &d)!=EOF){
    struct process p = {.AT = a, .BT = d, .ET = d};
    list[i] = p;
    i++;
  }

  rr(list, i, q);

  printf("P #     AT     BT     CT    TAT     WT\n\n");

  double att = 0;
  double awt = 0;

  for(int k = 0; k<i; k++) {
    printf("P%2d %6d %6d %6d %6d %6d\n", k, list[k].AT, list[k].BT, list[k].CT, list[k].TAT, list[k].WT);
    att+=list[k].TAT;
    awt+=list[k].WT;
  }

  printf("\nAverage Turnaround Time: %f\n", att/i);
  printf("Average WT: %f\n", awt/i);

  return 0;
}