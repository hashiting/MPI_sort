#include"mpi.h"
#include<time.h>
#include <stdlib.h>
#include <stdio.h>


#define SIZE 262144
#define range 1000000

int Exp2(int num)
{
  	if(num == 1) return 2;
    else if(num == 0) return 1;
    else{
        if(num%2 == 0){
            int temp = Exp2(num/2);
            return temp*temp;
        }
        else{
            int temp = Exp2(num/2);
            return temp*temp*2;
        }
    }
}


//Log2num
int Log2(int num)
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

int Is_sort(int *array){
	int i;
	for(i = 0;i < SIZE-1;i++){
		if(array[i+1] < array[i]){
			return -1;
		}
	}
	return 1;
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

/*
void MPI_Qsort(int *array,int begin,int end,int sqrt_num_process,int rank,int my_rank){
    int i;
	int temp_len;
  	int privot;
  	int len = -1;
  	int *temp;
  	MPI_Status status;
	if(sqrt_num_process == 0){
		if(my_rank == rank){
			quicksort(array,begin,end);
			return;
		}
	}
    if(my_rank == rank){
        privot = get_privot(array,begin,end);
        len = end - privot;
        MPI_Send(&len,1,MPI_INT,rank+Exp2(sqrt_num_process-1),my_rank,MPI_COMM_WORLD);
        if(len > 0){
            MPI_Send(array+privot+1,len,MPI_INT,rank+Exp2(sqrt_num_process-1),my_rank,MPI_COMM_WORLD);
        }
    }
    if(my_rank==rank+Exp2(sqrt_num_process-1))
  	{
  		MPI_Recv(&len,1,MPI_INT,rank,rank,MPI_COMM_WORLD,&status);
  		if(len!=0)
  		{
  			temp=(int *)malloc(len*sizeof(int));
  			MPI_Recv(temp,len,MPI_INT,rank,rank,MPI_COMM_WORLD,&status);
  		}
  	}
    //if(my_rank == rank){
	temp_len = privot - begin-1;
	MPI_Bcast(&temp_len,1,MPI_INT,rank,MPI_COMM_WORLD);
    //}
    if(temp_len > 0){
        MPI_Qsort(array,begin,privot-1,sqrt_num_process-1,rank,my_rank);
    }

    //if(my_rank == rank){
	temp_len = len;
	MPI_Bcast(&temp_len,1,MPI_INT,rank,MPI_COMM_WORLD);
    //}
    if(temp_len > 0){
        MPI_Qsort(temp,0,len-1,sqrt_num_process-1,rank+Exp2(sqrt_num_process-1),my_rank);
    }

    if((my_rank == rank+Exp2(sqrt_num_process-1)) && (len!=0))
  		MPI_Send(temp,len,MPI_INT,rank,rank+Exp2(sqrt_num_process-1),MPI_COMM_WORLD);
  	if((my_rank==rank) && (len!=0))
  		MPI_Recv(array+privot+1,len,MPI_INT,rank+Exp2(sqrt_num_process-1),rank+Exp2(sqrt_num_process-1),MPI_COMM_WORLD,&status);
    
}*/

void MPI_Qsort(int *array,int begin,int end,int sqrt_num_process,int rank,int my_rank)
{
	int temp_len = -1;
  	int privot;
  	int len = -1;
  	int *temp;
  	MPI_Status status;
	int next_rank = rank+Exp2(sqrt_num_process-1);
	if(sqrt_num_process==0)
  	{
  		if(my_rank==rank)
  		quicksort(array,begin,end);
  		return;
  	}
  	if(my_rank==rank)
  	{
  		privot=get_privot(array,begin,end);
  		len=end - privot;
  		MPI_Send(&len,1,MPI_INT,next_rank,my_rank,MPI_COMM_WORLD);
  		if(len!=0)
  			MPI_Send(array+privot+1,len,MPI_INT,next_rank,my_rank,MPI_COMM_WORLD);
  	}

  	if(my_rank==next_rank)
  	{
  		MPI_Recv(&len,1,MPI_INT,rank,rank,MPI_COMM_WORLD,&status);
  		if(len!=0)
  		{
  			temp=(int *)malloc(len*sizeof(int));
  			if(temp==0) printf("Malloc memory error!");
  				MPI_Recv(temp,len,MPI_INT,rank,rank,MPI_COMM_WORLD,&status);
  		}
  	}

  	temp_len=privot-1-begin;
  	MPI_Bcast(&temp_len,1,MPI_INT,rank,MPI_COMM_WORLD);
	//MPI_Barrier(MPI_COMM_WORLD);

  	if(temp_len>0)
  		MPI_Qsort(array,begin,privot-1,sqrt_num_process-1,rank,my_rank);

  	temp_len=len;
  	MPI_Bcast(&temp_len,1,MPI_INT,rank,MPI_COMM_WORLD);
	//MPI_Barrier(MPI_COMM_WORLD);

  	if(temp_len>0)
  		MPI_Qsort(temp,0,len-1,sqrt_num_process-1,next_rank,my_rank);

  	if((my_rank==next_rank) && (len!=0))
  		MPI_Send(temp,len,MPI_INT,rank,next_rank,MPI_COMM_WORLD);
  	if((my_rank==rank) && (len!=0))
  		MPI_Recv(array+privot+1,len,MPI_INT,next_rank,next_rank,MPI_COMM_WORLD,&status);
  }


int main(int argc,char** argv){
    int rank, num_process;
  	MPI_Status status;
  	MPI_Init(&argc,&argv);
  	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  	MPI_Comm_size(MPI_COMM_WORLD,&num_process);
    int *array;
	double start_time,end_time;
	double start_time1,end_time1;
    if(rank == 0){
		int *temp  = generate_random(SIZE);
		start_time1 = MPI_Wtime();
		quicksort(temp,0,SIZE-1);
		end_time1 = MPI_Wtime();
		printf("serial time for size of %d is: %fs\n",SIZE,end_time1-start_time1);
        array = generate_random(SIZE);
		/*int *temp  = generate_random(size);
		quicksort(temp,0,size-1);
		if(Is_sort(temp) == 1){
			printf("test sucessfully sort!\n");
		}
		else{
			printf("test Fail sort!\n");
		}
		free(temp);*/
		printf("begin MPI_sort\n");
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0){
		start_time = MPI_Wtime();
	}
    MPI_Qsort(array,0,SIZE-1,Log2(num_process),0,rank);
	if(rank == 0){
		end_time = MPI_Wtime();
		if(Is_sort(array) == 1){
			printf("sucessfully sort!\n");
		}
		else{
			printf("Fail sort!\n");
		}

		printf("parallel time for size of %d using %d process is: %fs\n",SIZE,num_process,end_time-start_time);
	}
    MPI_Finalize();
}
