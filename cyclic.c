#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h> // for timing
#include <pthread.h>

// Epsilon used to evaluate the equality of two floats
static float EPSILON = 0.0000001;

// Here we define the size of the array
static long nb = 100000000;

// Used in a multithread work. 
// Boolean used to see if a thread has already found the requested value in the array or not
static bool finished = 0;

/* We define the structure of the thread */
struct thread_data{
	// id of the thread
  unsigned int thread_id;
  // the array
  float *p;
  // Step used for the cyclic_routine
  int i_step;
  // The value we're looking for
  float val;
};

// Number of threads
#define NB_THREADS 4

// Variable to store the set of results
int rs[NB_THREADS];

// Variables for the threads
int rs[NB_THREADS];
struct thread_data thread_data_array[NB_THREADS];
unsigned int thread_i;
pthread_t thread_ptr[NB_THREADS];

/* Timing function */
double now(){
   struct timeval t; double f_t;
   gettimeofday(&t, NULL); 
   f_t = t.tv_usec; f_t = f_t/1000000.0; f_t +=t.tv_sec;
   return f_t; 
}

/* Cyclic find function 
It searches a value step by step, from 0 to i_end */
int cyclic_find(float *U, int i_step, int i_end, float val, int thread_id){
	int i;

	for(i=0; i < i_end/i_step+1; i++){
		// We look if another thread has already found the value
		if(finished == 1) {
				break;
		}
		if(fabs(U[i*i_step]-val) < EPSILON){
			finished = 1;
			printf("%d\n", i*i_step+thread_id);
			return 1;
		}
	}
    return 0;
}

/* Thread function 
This is the function that is called when a thread is launched */
void *thread_function(void *threadarg){
	// Local variables
	int id;
	// Shared variables correspondances
	float *p;
	int i_step;
	float val;
	// Association between shared variables and their correspondances
	struct thread_data *thread_pointer_data;
	thread_pointer_data = (struct thread_data *)threadarg; 
	// Shared variables
	id = thread_pointer_data->thread_id;
	p = thread_pointer_data->p;
	i_step = thread_pointer_data->i_step;
	val = thread_pointer_data->val;
	
	// Body of the thread
	rs[id] = cyclic_find(p, NB_THREADS, nb-1-id, val, id);	
	
	pthread_exit(NULL);
	return 0;
}

/* Multithread search function
The function that creates and launches the threads */
int search_thread(float *p, int n, float val){
  	int i, s;
	// Create and launch threads
	for(i=0;i<NB_THREADS;i++){
	  thread_i = i;
	  // Prepare data for this thread
	  thread_data_array[thread_i].thread_id = thread_i;
	  thread_data_array[thread_i].p = p+i;
	  thread_data_array[thread_i].val = val;
	  thread_data_array[thread_i].i_step = NB_THREADS;
	  // Create and launch this thread
	  pthread_create(&thread_ptr[thread_i], NULL, thread_function, (void *) &thread_data_array[thread_i]);
	}
	// Wait for every thread to complete
	for(i=0;i<NB_THREADS;i++)
  		pthread_join(thread_ptr[thread_i], NULL);

	for(i=0;i<NB_THREADS;i++)
		s = s || rs[i];
		
	return s;
}

int main(int argc, char *argv[]){
	printf("generating the array...\n");
	/* The following line is used in order to have a sort of random array
	If we comment this line, */
	srand((unsigned)time(0));
	
	// the array is allocated and then filled with random floats
	float *p;
	p = malloc(nb*sizeof(float));
	int i;

	// Filling the array
	for(i=0; i < nb; i++){
		p[i] = ((float)rand()/(float)RAND_MAX)+(rand()%100);
	}
	
	// The value to look for
	float val = 9.589720;
	
	// The time
	double t0,t1, t2, t3 ;
	
	printf("Searching with a multithread method...\n");
	
	t0=now(); i = search_thread(p, nb, val); t1=now(); 
	if(i==0)
		printf("Not found with multithread method\n");
	printf("Multi-Thread T = %6.4f\n",t1-t0);
	
	printf("Searching with a monothread method...\n");
	
	finished = 0;
	t2 = now();	 i = cyclic_find(p, 1, nb-1, val, 0); t3 = now();
	if(i==0)
		printf("Not found with mono-thread method\n");
	printf("Mono-Thread T = %6.4f\n",t3-t2);
	
	//Free the memory
	free(p);
}