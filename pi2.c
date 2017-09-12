#include <linux/sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

#define NTHREADS 4
#define STACK_SIZE (1024*1024)

long long num_steps = 1000000000;
double step;

typedef struct thread
{
	double sum;
	int i;	
} td;


int *tfunc(void *args)
{
	td *tinfo = (td *) args;
	int start = tinfo->i * (num_steps / NTHREADS);
	int end = (tinfo->i + 1) * (num_steps / NTHREADS);

	printf("Thread i %d\n", tinfo->i );

	int i;
	double x, sum = 0.0;
	for (i = start; i < end; i++)
	{
		x = (i + .5)*step;
		sum = sum + 4.0/(1.0+ x*x);
	}
	tinfo->sum = sum;
	printf("Thread sum %f\n", tinfo->sum );

	return 0;
}

int main(int argc, char* argv[])
{
	long long start_ts;
	long long stop_ts;
	float elapsed_time;
	long lElapsedTime;
	struct timeval ts;

	//Child stack initialization
	char *child_stack = (char*)malloc(sizeof(char)*STACK_SIZE);
	char *top_stack = child_stack + STACK_SIZE;

	//Thread arrays
	pid_t tid[NTHREADS];
	td threads[NTHREADS];

	double x, pi, sum=0.0;
	int i;

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial

	step = 1./(double)num_steps;

	for(int i = 0; i < NTHREADS; i++)
	{
		threads[i].sum = 0.0;
		threads[i].i = i;
		tid[i] = clone(tfunc, top_stack, SIGCHLD, (void*)&threads[i]);
	}

	
	for(int i = 0; i < NTHREADS; i++)
	{
		waitpid(tid[i], NULL, 0);
		printf("Salió de %d con suma %f\n", tid[i], threads[i].sum);
	}

	pi = (threads[0].sum + threads[1].sum + threads[2].sum + threads[3].sum)*step;

	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final
	
	elapsed_time = (float) (stop_ts - start_ts)/1000000.0;
	printf("El valor de PI es %1.12f\n",pi);
	printf("Tiempo = %2.2f segundos\n",elapsed_time);
	
	
	return 0;
}



