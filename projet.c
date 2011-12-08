#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static float EPSILON = 0.000001;
static int n = 16;

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

int cyclic_find(float *U, int i_step, int i_end, float val){
	int i;
	for(i=0; i < i_end/i_step+1; i++){
		printf("%d ", i);
		printf("i*i_step=%d ", i*i_step);
		printf("U[i*i_step]=%f\n", U[i*i_step]);
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
	//srand((unsigned)time(0));
	for(i=0; i < n; i++){
		p[i] = (float)rand()/(float)RAND_MAX;
	}
	for(i=0; i < n; i++){
		printf("%f\n", p[i]);
	}
	float val = 0.523072;
	int res = block_find(p, 0, n-1, val);
	int res2 = cyclic_find(p, 5, n-1, val);
}