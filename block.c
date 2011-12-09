#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h> // for timing
#include <pthread.h>

static float EPSILON = 0.000001;
static int nb = 100000000;
static bool finished = 0;

struct thread_data{
  unsigned int thread_id;
  float *p;
  int i_start;
  int i_end;
  float val;
};

#define NB_THREADS 4
int rs[NB_THREADS];
struct thread_data thread_data_array[NB_THREADS];
unsigned int thread_i;
pthread_t thread_ptr[NB_THREADS];

double now(){
   struct timeval t; double f_t;
   gettimeofday(&t, NULL); 
   f_t = t.tv_usec; f_t = f_t/1000000.0; f_t +=t.tv_sec;
   return f_t; 
}

int block_find(float *U, int i_start, int i_end, float val){
	int i;
	for(i=i_start; i < i_end + 1; i++){
		if(finished == 1) {
			//printf("received break instruction");
			break;
		}
		if(fabs(U[i]-val) < EPSILON){
			printf("%d, u(i)=%f\n", i, U[i]);
			finished = 1;
			return 1;
		}
		//printf("Some thread looking for %d\n", i);
	}
    return 0;
}

void *thread_function(void *threadarg){
 /* Local variables */
  int id;
 /* Shared variables correspondances */
  float *p;
  int i_start, i_end;
  float val;
 /* Association between shared variables and their correspondances */
  struct thread_data *thread_pointer_data;
  thread_pointer_data = (struct thread_data *)threadarg; 
  /* Shared variables */
  id = thread_pointer_data->thread_id;
  p = thread_pointer_data->p;
  i_start = thread_pointer_data->i_start;
  i_end = thread_pointer_data->i_end;
  val = thread_pointer_data->val;
 
  /* Body of the thread */
  rs[id] = block_find(p, i_start, i_end, val);
  //printf("Thread %d : i_start=%d, i_end=%d\n", id, i_start, i_end);

  pthread_exit(NULL);
  return 0;
}

int search_thread(float *p, int n, float val){
  	int i, s;
  	s = 0;
/*-------------------------------------------------------------*/
/* We prepare and call the thread version of this code portion */
/*-------------------------------------------------------------*/
/* Create and launch threads */
	for(i=0;i<NB_THREADS;i++){
	  thread_i = i;
	  /* Prepare data for this thread */
	  thread_data_array[thread_i].thread_id = thread_i;
	  thread_data_array[thread_i].p = p;
	  thread_data_array[thread_i].val = val;
	  thread_data_array[thread_i].i_start = i*(n/NB_THREADS);
	  thread_data_array[thread_i].i_end = (i+1)*(n/NB_THREADS)-1;
	  /* Create and launch this thread */
	  pthread_create(&thread_ptr[thread_i], NULL, thread_function, (void *) &thread_data_array[thread_i]);
	}
/* Wait for every thread to complete  */
	for(i=0;i<NB_THREADS;i++){
  		pthread_join(thread_ptr[thread_i], NULL);
	}
	return 1;
}

int main(int argc, char *argv[]){
	printf("float rand: %f\n", (float)rand());
	float *p;
	p = malloc(nb*sizeof(float));
	int i;
	double t0,t1, t2, t3 ;
	//srand((unsigned)time(0));
	for(i=0; i < nb; i++){
		//p[i] = ((float)rand()/(float)RAND_MAX)*(1000000)+10;
		p[i] = (float)((rand() << 15 + rand()) & ((1 << 24) - 1)) / (1 << 24);
		if(i>nb-5)
			printf("%f\n", p[i]);
	}
	float val = 0.265625;
	t0=now(); search_thread(p, nb, val); t1=now(); 
	printf("Multi-Thread T = %6.4f\n",t1-t0);
	finished = 0;
	t2 = now(); block_find(p, 0, nb-1, val); t3 = now();
	printf("Mono-Thread T = %6.4f\n",t3-t2);
}