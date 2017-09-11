#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

#define NTHREADS 4

long long num_steps = 1000000000;
double step;

typedef struct thread
{
	double sum;
	int i;
} td;

void *tfunc(void * args)
{
	td *tinfo = (td *) args;
	int start = tinfo->i * (num_steps / NTHREADS);
	int end = (tinfo->i + 1) * (num_steps / NTHREADS);
	//printf("thread %d, start %d, end %d\n", tinfo.i, start, end);
	int i;
	double x, sum = 0.0;
	for (i = start; i < end; i++)
	{
		x = (i + .5)*step;
		sum = sum + 4.0/(1.0+ x*x);
	}
	tinfo->sum = sum;
	//printf("thread %d, sum %lf\n", tinfo->i, tinfo->sum);
}

int main(int argc, char* argv[])
{
	long long start_ts;
	long long stop_ts;
	float elapsed_time;
	long lElapsedTime;
	struct timeval ts;

	double x, pi, sum=0.0;
	int i;
	pthread_t idhilos[NTHREADS];
	td threads[NTHREADS];

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial

	step = 1./(double)num_steps;

	for (i = 0; i < NTHREADS; i++)
	{
		threads[i].sum = 0.0;
		threads[i].i = i;
		pthread_create(&idhilos[i], NULL, tfunc, (void *) &threads[i]);
	}

	for(i = 0; i < NTHREADS; i++)
	{
		pthread_join(idhilos[i], NULL);
	}

	/*for(i = 0; i < NTHREADS; i++)
	{
		printf("thread %d sum %lf\n", threads[i].i, threads[i].sum);
	}*/

	pi = (threads[0].sum + threads[1].sum + threads[2].sum + threads[3].sum)*step;

	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final
	
	elapsed_time = (float) (stop_ts - start_ts)/1000000.0;
	printf("El valor de PI es %1.12f\n",pi);
	printf("Tiempo = %2.2f segundos\n",elapsed_time);
	return 0;
}