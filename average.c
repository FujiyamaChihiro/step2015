#include <stdio.h>
int main(){
  double sec[10];
  double sum=0.0;
  double average;
  for(int i=0;i<10;i++){
    printf("Input the value : ");
    scanf("%lf",&sec[i]);
  }
  for(int i=0;i<10;i++){
    sum+=sec[i];
  }
  average=sum/10.0;
  printf(" average : %.6lf\n",average);
}
