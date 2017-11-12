#define _GNU_SOURCE
#ifndef __USE_GNU
#define __USE_GNU
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

#define NUM_JOBS 100
#define PRINT_OUTPUT

typedef struct job_struct {
	unsigned int number_of_entries;
	float *x_arr;
	float *y_arr;
}_job;

_job* job_arr[NUM_JOBS];
pthread_mutex_t mutex_pop;
unsigned long job_arr_index;

void regression(_job *job) {
	unsigned int n, i;
	float sumx, sumsq, sumxsq, sumy, sumxy, x, y, a0, a1, denom;
	sumx = sumsq = sumxsq = sumy = sumxy = x = y = a0 = a1 = denom = 0;

	n = job->number_of_entries;
	for (i=0; i<n; i++) {
		x = job->x_arr[i];
		y = job->y_arr[i];
		sumx += x;
		sumxsq += pow(x, 2);
		sumy += y;
		sumxy += x*y;
	}
	denom = n * sumxsq - pow(sumx, 2);
	a0 = (sumy * sumxsq - sumx * sumxy) / denom;
	a1 = (n * sumxy - sumx * sumy) / denom;
#ifdef PRINT_OUTPUT
	printf("y = %fx + %f\n", a1, a0);
#endif
}

int set_job(_job *j) {
	int i;
	j->number_of_entries = abs(2+rand()%1000);
	j->x_arr = (float*)malloc(j->number_of_entries*sizeof(float));
	j->y_arr = (float*)malloc(j->number_of_entries*sizeof(float));
	for (i=0; i<j->number_of_entries; i++) {
		j->x_arr[i] = rand()%1000;
		j->y_arr[i] = rand()%1000;
	}
	return 0;
}

int push_jobs (void) {
	_job *job;
	unsigned int i;
	for (i=0; i<NUM_JOBS; i++) {
		job = (_job*)malloc(sizeof(_job));
		set_job(job);
		job_arr[i] = job;
	}
	return 0;
}

_job* pop_job (thread_id) {
	_job* value = NULL;
	pthread_mutex_lock(&mutex_pop);
	if (job_arr_index < NUM_JOBS) {
		value = job_arr[job_arr_index++];
	}
	pthread_mutex_unlock(&mutex_pop);
	return value;
}


typedef struct thread_worker_arg_list_struct {
	int arg1;	  					// the proc_num
} thread_worker_arg_list;



void* thread_worker(void *thread_worker_args) {
	// thread definitions:
	cpu_set_t set;
	int thread_id;
	_job *popped_job;

	thread_worker_arg_list *args = (thread_worker_arg_list*) thread_worker_args;
	thread_id = (int)args->arg1;

	CPU_ZERO(&set);
	// set thread with thread_id to cpu set
	CPU_SET(thread_id, &set);
	if (sched_setaffinity(syscall(__NR_gettid), sizeof(cpu_set_t), &set)) {
		exit(1);
	}

	while ((popped_job = pop_job(thread_id))) {
		// process job
		regression(popped_job);
	}

	return NULL;
}

int main() {
	pthread_t *thrs;
	thread_worker_arg_list *thread_worker_args;
	unsigned int number_of_cpus = 0;
	// mutex init:
	pthread_attr_t attr;
	int i;

	pthread_mutex_init(&mutex_pop, NULL);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	push_jobs();

	// Getting number of CPUs
	number_of_cpus = (int)sysconf(_SC_NPROCESSORS_ONLN);
	// Getting threads:
	thrs = malloc(sizeof(pthread_t) * number_of_cpus);

	printf( "Starting %d threads...\n", number_of_cpus );

	for (i = 0; i < number_of_cpus; i++) {
		thread_worker_args = (thread_worker_arg_list*)malloc(sizeof(thread_worker_arg_list));
		thread_worker_args->arg1 = i;
		if (pthread_create(&thrs[i], NULL, thread_worker, (void *)thread_worker_args)) {
			exit(1);
		}
	}

	for (i = 0; i < number_of_cpus; i++) {
		if (pthread_join(thrs[i], NULL)) {
			printf("join error\n");
			exit(1);
		}
	}

	free(thrs);
	free(thread_worker_args);
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&mutex_pop);
	printf("OK\n");
	return 0;
}
