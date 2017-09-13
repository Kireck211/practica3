#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

#define NTHREADS 4
#define SHMSIZE sizeof(double)

long long num_steps = 1000000000;
double step;

int main(int argc, char * argv[])
{
	int index, shmid, key, i, start, end;
	double x, sum = 0.0, *shm, *double_pointer;
	sscanf(argv[1], "%d", &key);
	sscanf(argv[2], "%d", &index);

	if((shmid = shmget(key, SHMSIZE, IPC_CREAT | 0666)) < 0)
	{
		perror("shmget pi3_child");
		exit(1);
	}

	if((shm = (double *)shmat(shmid, NULL, 0)) == (double *) -1)
	{
		perror("shmat pi3_child");
		exit(2);
	}

	double step = 1./(double)num_steps;

	start = index * (num_steps / NTHREADS);
	end = (index + 1) * (num_steps / NTHREADS);

	for (i = start; i < end; i++)
	{
		x = (i + .5)*step;
		sum = sum + 4.0/(1.0 + x*x);
	}
	*double_pointer = sum;

	return 0;
}