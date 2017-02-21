#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include"my_timers.h"

#define DOMAIN_MIN 0.0
#define DOMAIN_MAX 1000.0
#define DOMAIN_LEN 200000000
#define MASTER 0

void get_full_domain(double **domaina, double **domainb, double **domainc, long long int *domain_length) {
  long long int i;
  *domain_length = DOMAIN_LEN;
  double step = (DOMAIN_MAX-DOMAIN_MIN)/((double) *domain_length);

  *domaina = (double*) malloc((*domain_length)*sizeof(double));
  *domainb = (double*) malloc((*domain_length)*sizeof(double));
  *domainc = (double*) malloc((*domain_length)*sizeof(double));

  for (i=0;i<(*domain_length);i++){
    (*domaina)[i] = DOMAIN_MIN + step*((double)i);
    (*domainb)[i] = DOMAIN_MIN + step*((double)i);
    (*domainc)[i] = DOMAIN_MIN + step*((double)i);
  }
}

void main(int argc, char** argv){
// arrays a, b, c
  double * a;
  double * b;
  double * c;
  
  // total nuber of processes
  int total_proc;
  
  // rank of each process
  int rank;
  
  // elements per process
  long long int n_per_proc;
  

  long long int i, n;

  MPI_Init(&argc, &argv);

  MPI_Comm_size (MPI_COMM_WORLD, &total_proc);
  MPI_Comm_rank (MPI_COMM_WORLD,&rank);

  double * ap;
  double * bp;
  double * cp;

  /*
   * Root obtains full domain and broadcasts its length.
   */
  if (rank == MASTER) {
      
    get_full_domain(&a, &b, &c, &n);
    MPI_Bcast (&n, 1, MPI_LONG_LONG_INT, MASTER, MPI_COMM_WORLD);
    n_per_proc = n/total_proc;
    
    printf ("\nThe number of elements per processor:  %d ", n_per_proc);
    if(n%total_proc != 0) {
      n_per_proc+=1;
      for(i=0;i<(n_per_proc*total_proc - n);i++) {
          a[n+i] = 0.0;
          b[n+i] = 0.0;
        }
    }

    printf ("\nThe number of array elements:  %d ", n);
    printf ("\nThe number of processors:  %d ", total_proc);
    
    ap = (double *) malloc(sizeof(double)*n_per_proc);
    bp = (double *) malloc(sizeof(double)*n_per_proc);
    cp = (double *) malloc(sizeof(double)*n_per_proc);
    
    MPI_Bcast (&n_per_proc, 1, MPI_LONG_LONG_INT, MASTER, MPI_COMM_WORLD);
    
    start_time();
    printf ("\nAdding started...");
    
    MPI_Scatter(a, n_per_proc, MPI_DOUBLE, ap, n_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(b, n_per_proc, MPI_DOUBLE, bp, n_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    for(i=0;i<n_per_proc;i++) {
      cp[i] = ap[i]+bp[i];
    }
    
    MPI_Gather(cp, n_per_proc, MPI_DOUBLE, c, n_per_proc, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
    stop_time();
   
    printf ("\nAdding finished.");
    
    print_time("\nAdding time: ");

  } else {

    MPI_Bcast (&n, 1, MPI_LONG_LONG_INT, MASTER, MPI_COMM_WORLD);
    MPI_Bcast (&n_per_proc, 1, MPI_LONG_LONG_INT, MASTER, MPI_COMM_WORLD);
    ap = (double *) malloc(sizeof(double)*n_per_proc);
    bp = (double *) malloc(sizeof(double)*n_per_proc);
    cp = (double *) malloc(sizeof(double)*n_per_proc);
    MPI_Scatter(a, n_per_proc, MPI_DOUBLE, ap, n_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    //Recieving Scattered a
    MPI_Scatter(b, n_per_proc, MPI_DOUBLE, bp, n_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    //Recieving Scattered b
    for(i=0;i<n_per_proc;i++) {
        cp[i] = ap[i]+bp[i];
      }
    MPI_Gather(cp, n_per_proc, MPI_DOUBLE, c, n_per_proc, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
  }

  MPI_Finalize();
}
