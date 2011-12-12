#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Epsilon used to evaluate the equality of two floats
static float EPSILON = 0.000001;

// Here we define the size of the array
static int n = 10000000;

// Block_Find method
int block_find(float *U, int i_start, int i_end, float val){
	int i;
	for(i=i_start; i < i_end + 1; i++){
		if(fabs(U[i]-val) < EPSILON){
			printf("%d\n", i);
			return 1;
		}
	}
    return 0;
}

// Cyclic_find method
int cyclic_find(float *U, int i_step, int i_end, float val){
	int i;
	for(i=0; i < i_end/i_step+1; i++){
		if(fabs(U[i*i_step]-val) < EPSILON){
			printf("%d\n", i);
			return 1;
		}
	}
    return 0;
}

int main(int argc, char *argv[]){
	float *p;
	p = malloc(n*sizeof(float));
	int i;
	
	/* The following line is used in order to have a sort of random array
	If we comment this line, the array will be the same at every launch of the program*/
	srand((unsigned)time(0));
	
	// the array is allocated and then filled with random floats
	for(i=0; i < n; i++){
		p[i] = ((float)rand()/(float)RAND_MAX)+(rand()%100);
	}
	
	// The value we're looking for
	float val = 0.523072;
	printf("Searching with block_find\n");
	int res = block_find(p, 0, n-1, val);
	if(res==0)
		printf("Not found with block method\n");
	printf("Searching with cyclic_find\n");
	int res2 = cyclic_find(p, 1, n-1, val);
	if(res2==0)
		printf("Not found with cyclic method\n");
}