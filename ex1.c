#include <stdio.h>

struct process{
  int AT;
  int BT;
  int CT;
  int WT;
  int TAT;
};

void swap(struct process* a, struct process* b){
  struct process t = *a;
  *a = *b;
  *b = t;
}

void sort(struct process *list, int size){
  // bubble sort
  for (int i=0; i<size-1; i++){
    for (int k=0; k<size-i-1;k++){
      if (list[k].AT>list[k+1].AT)
        swap(&list[k], &list[k+1]);
    }
  }
}

void fifo(struct process *list, int size){
  sort(list, size);
  int clock = 1;
  int index = 0;
  while (index != size){
    if (list[index].AT<=clock){

      list[index].CT = clock + list[index].BT;
      list[index].TAT = list[index].CT - list[index].AT;
      list[index].WT = clock - list[index].AT;
      clock = list[index].CT;
      index++;
      
    } else clock++;
  }
  
}


int main(void) {

  FILE *fp;

  fp = fopen("new_input.csv", "r");

  struct process list[10];

  int a = 0, d = 0; //arrival time and duration
  int i=0; //index

  while (fscanf(fp, "%d,%d", &a, &d)!=EOF){
    struct process p = {a, d};
    list[i] = p;
    i++;
  }

  fifo(list, i);

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