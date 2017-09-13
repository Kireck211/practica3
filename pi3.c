#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define NTHREADS 4
#define SHMSIZE sizeof(double)*100

long long num_steps = 1000000000;
double step;

int main(int argc, char* argv[])
{
	long long start_ts;
	long long stop_ts;
	float elapsed_time;
	long lElapsedTime;
	struct timeval ts;

	double x, pi, sum=0.0;
	int i, status;

	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial

	step = 1./(double)num_steps;
	double *shm;

	int pids[NTHREADS], shmid, key = 1000;

	int proc = 1;
	char casted_proc[10];
	char casted_key[10];
	char casted_index[10];
	

	for (i = 0; i < NTHREADS; i++)
	{
		sprintf(casted_proc, "%d", proc);
		sprintf(casted_key, "%d", (key+i));
		sprintf(casted_index, "%d", i);
		//printf("thread :%d\n", i);
		pids[i] = fork();

		if (pids[i] == 0)
		{
			execlp("taskset", "taskset", casted_proc, "./pi3_child", casted_key, casted_index, NULL);
		}
		proc<<=1;
	}

	for (i = 0; i < NTHREADS; i++)
	{
		wait(&status);
	}

	for (i = 0; i < NTHREADS; i++)
	{
		if ((shmid = shmget(key+i, SHMSIZE, 0666)) < 0)
		{
			perror("shmget pi3");
			exit(1);
		}

		if ((shm = shmat(shmid, NULL, 0)) == (double *) -1)
		{
			perror("shmat pi3");
			exit(2);
		}

		double child_sum = *shm;
		sum = sum + child_sum;
	}

	pi = sum*step;

	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final
	
	elapsed_time = (float) (stop_ts - start_ts)/1000000.0;
	printf("El valor de PI es %1.12f\n",pi);
	printf("Tiempo = %2.2f segundos\n",elapsed_time);
	return 0;
}