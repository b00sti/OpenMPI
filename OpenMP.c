#include<stdio.h>
#include"my_timers.h"

//200 millions
#define NBR_OF_ELEMENTS 200000000

main(int argc, char** argv) {

  long i=0;
  double a[NBR_OF_ELEMENTS];
  double b[NBR_OF_ELEMENTS];
  double c[NBR_OF_ELEMENTS];

  omp_set_num_threads(argv[1]);

  printf("Preparing tab a... \n");
  start_time();
  //data preparing (tab a and b with random numbers)
  for( i=0; i<NBR_OF_ELEMENTS; i++){
    a[i]=1.0/rand();
  }
  stop_time();
  //print elapsed time
  print_time("Elapsed:");

  start_time();
  printf("Preparing tab b... \n");
  for( i=0; i<NBR_OF_ELEMENTS; i++){
    b[i]=1.0/rand();
  }
  stop_time();
  //print elapsed time
  print_time("Elapsed:");

  printf("\nAdding started... \n");
  //adding
  start_time();
    #pragma omp parallel for
  for( i=0; i<NBR_OF_ELEMENTS; i++) {
    c[i] = a[i] + b[i];
  }
  printf("Adding finished... \n");

  stop_time();
  //print elapsed time
  print_time("Adding time:");
}
