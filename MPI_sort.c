#include "stdafx.h"
#include"mpi.h"
#include<time.h>
#include <stdlib.h>
#include <stdio.h>
#define size 16384
#define range 100000

int exp2(int num)
{
  	if(num == 1) return 2;
    else if(num == 0) return 1;
    else{
        if(num%2 == 0){
            int temp = exp2(num/2);
            return temp*temp;
        }
        else{
            int temp = exp2(num/2);
            return temp*temp*2;
        }
    }
}


//log2num
int log2(int num)
{
  	int i, j;
  	i=1;
  	j=2;
  	while(j<num)
  	{
  		j=j*2;
  		i++;
  	}
  	if(j>num)
  		i--;
  	return i;
}

void swap(int *a,int *b){
	int t = *a;
	*a = *b;
	*b = t;
}

int get_privot(int* array,int begin,int end){
	int privot = array[begin];
		int i = begin+1;
		int j = end;
		while(i <= j){
	     while((i <= end) && (array[i] <= privot))
	            i++;
	     while((j >= begin) && (array[j] > privot))
	            j--;
	     if(i < j)
	            swap(&array[i],&array[j]);
	  	}
	    swap(&array[begin],&array[j]);
	return j;
}

void quicksort(int* array,int begin,int end){
	if(begin >= end){
		return;
	}
	else{
		int privot = get_privot(array,begin,end);
	    quicksort(array,begin,privot-1);
	    quicksort(array,privot+1,end);
	}
}

int* generate_random(int num){
	srand(time(NULL) + rand());
	int *a = (int*)malloc(num*sizeof(int));
	int i = 0;
	for(i = 0;i < num;i++){
		a[i] = rand()%range;
	}
	return a;
}

void MPI_Qsort(int *array,int begin,int end,int sqrt_num_process,int rank,int my_rank){
    int i;
  	int privot;
  	int len = -1;
    int len_two = -1;
  	int *temp;
  	MPI_Status status;
    /*divide data and send to rank + 2^(sqrt_num_process-1)*/
    if(my_rank == rank){
        privot = get_privot(array,begin,end);
        len_two = end - privot;
        /*send len of latter array*/
        MPI_Send(&len_two,1,MPI_INT,rank+exp2(num_process-1),my_rank,MPI_COMM_WORLD);
        if(len_two > 0){
            MPI_Send(array+privot+1,len_two,MPI_INT,rank+exp2(num_process-1),my_rank,MPI_COMM_WORLD);
        }
    }
    /*id + 2^(sqrt_num_process-1) receive the array and len*/
    if(my_rank==rank+exp2(num_process-1))
  	{
  		MPI_Recv(&len_two,1,MPI_INT,rank,rank,MPI_COMM_WORLD,&status);
  		if(len_two!=0)
  		{
  			temp=(int *)malloc(len_two*sizeof(int));
  				MPI_Recv(temp,len_two,MPI_INT,rank,rank,MPI_COMM_WORLD,&status);
  		}
  	}
    if(my_rank == rank){
        len = privot - begin-1;
  	    MPI_Bcast(&len,1,MPI_INT,rank,MPI_COMM_WORLD);
    }
    /*Barrier*/
    if(len > 0){
        MPI_Qsort(array,begin,begin+len,sqrt_num_process-1,rank,my_rank);
    }

    if(my_rank == rank){
        len = len_two;
  	    MPI_Bcast(&len,1,MPI_INT,rank,MPI_COMM_WORLD);
    }

    if(len > 0){
        MPI_Qsort(temp,0,len-1,sqrt_num_process-1,rank+exp2(sqrt_num_process-1),my_rank);
    }

    if((my_rank == rank+exp2(num_process-1)) && (len_two!=0))
  		MPI_Send(temp,len_two,MPI_INT,rank,rank+exp2(sqrt_num_process-1),MPI_COMM_WORLD);
  	if((my_rank==rank) && (len_two!=0))
  		MPI_Recv(array+privot+1,len_two,MPI_INT,rank+exp2(sqrt_num_process-1),rank+exp2(sqrt_num_process-1),MPI_COMM_WORLD,&status);
    
}
int main(){
    int rank, num_process;
  	MPI_Status status;
  	MPI_Init(&argc,&argv);
  	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  	MPI_Comm_size(MPI_COMM_WORLD,&num_process);
    int *array;
    if(rank == 0){
        array = generate_random(size);
    }
    para_QuickSort(array,0,size-1,log2(num_process),0,rank);

    MPI_Finalize();
}
