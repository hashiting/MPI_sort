#include <stdio.h>
#include <stdlib.h> 

void swap(int *a,int *b){
	int t = *a;
	*a = *b;
	*b = t;
}

void quicksort(int* array,int begin,int end){
	
	if(begin >= end){
		return;
	}
	else{
		int privot = array[begin];
		int i = begin+1;
		int j = end;
		while(i <= j)
      {
         while((i <= end) && (array[i] <= privot))
                i++;
         while((j >= begin) && (array[j] > privot))
                j--;
         if( i < j)
                swap(&array[i],&array[j]);
      }
      
      swap(&array[begin],&array[j]);
      quicksort(array,begin,j-1);
      quicksort(array,j+1,end);
	}
}

int main(){
	int *a = (int *)malloc(6*sizeof(int));
	a[0] = 22;a[1] = 10;a[2] = 1;a[3] = 3;a[4] = 11;a[5] = 0; 
	quicksort(a,0,5);
	int i = 0;
	for(i=0;i < 6;i++){
		printf("%d ",a[i]);
	}
}
