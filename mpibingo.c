#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/* Compute number of combinations of k items from population n */
int nCk (long fact[16], int n, int k)
{
  return fact[n] / (fact[n-k] * fact[k]);
}


int main(int argc, char *argv[])
{
  int rank,size, ierr;
  long fact[16]={1};
  int nballs[5]={0};
  int nneeded;
  double prob=1.0, reduced=1.0, tmpprob;

  MPI_Init (&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  for (int i=1; i<=15; i++) {
    fact[i] = fact[i-1]*i;
  }

  if (rank == 0) {
    printf("Enter number of balls called from each letter\n");
    printf("Example: 13 12 10 10 14\n");
    scanf("%d %d %d %d %d",&nballs[0],&nballs[1],&nballs[2],&nballs[3],&nballs[4]);
  }


  for (int i=0;i<5;i++) {
    if (nballs[i] < 0 || nballs[i] > 15) {
      printf("Error: numbers must be in range [0,15]\n");
      MPI_Abort(MPI_COMM_WORLD, 1);
    }
  }
	   
  MPI_Bcast(nballs, 5, MPI_INT, 0, MPI_COMM_WORLD);

  prob = 1.0;
  /* Do the work */
  for (int i=rank; i<5; i+=size) {
    nneeded = (i!=2) ? 5 : 4; //N col has a free space
    if (nballs[i] < nneeded) {
      prob *= 0;
    } else {
      tmpprob = 1.0*nCk(fact,15-nneeded,nballs[i]-nneeded)/nCk(fact,15,nballs[i]);

      prob *= tmpprob;
    }
    printf("rank=%d, col=%d, prob=%f\n",rank, i,tmpprob);
  }

  ierr = MPI_Reduce(&prob,&reduced,1,MPI_DOUBLE,MPI_PROD,0,MPI_COMM_WORLD);
  if (ierr!=0) {printf("Error"); exit(1);}

  if (rank == 0) {
    printf("probability of coverall bingo = %f\n", reduced);
  }

  MPI_Finalize();
  return(0);
}
