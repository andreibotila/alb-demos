/**
 * Copyright (C) 2016 NXP Semiconductors
 *
 * SPDX-License-Identifier:     BSD-3-Clause
 */

#define _GNU_SOURCE
#include "functions.h"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <sched.h>
#include <errno.h>

int make_matrix(char path[], int matrix[MAX_SIZE][MAX_SIZE], int *size)
{
	int i, j;
	FILE *f;

	f = fopen(path, "r+");
	if (!f)
		return -1;

	fscanf(f, "%d", size);
	for (i = 0; i < *size; i++)
		for (j = 0; j < *size; j++)
			fscanf(f, "%d", &matrix[i][j]);
	return 0;
}

void display_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size)
{
	int i, j;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++)
			printf(" %d ", matrix[i][j]);
		printf("\n");
	}

}

void multi_matrix(int matrix[MAX_SIZE][MAX_SIZE], int size, int loop_no)
{
	int i, j, k, m;
	int matrix_result[MAX_SIZE][MAX_SIZE];

	for (m = 0; m < loop_no; m++) {
		for (i = 0; i < size; i++)
			for (j = 0; j < size; j++) {
				matrix_result[i][j] = 0;
				for (k = 0; k < size; k++)
					matrix_result[i][j] +=
					    (matrix[i][k] * matrix[k][j]);
			}

		for (i = 0; i < size; i++)
			for (j = 0; j < size; j++)
				matrix[i][j] = matrix_result[i][j];
	}
}

void *thread_multi_matrix(void *params)
{
	struct parameter *p = (struct parameter *)params;

	cpu_set_t cpuset;
	/* the CPU we want to use */
	CPU_ZERO(&cpuset);
	/* clears the cpuset */
	CPU_SET(p->id_thread, &cpuset);
	/**
	 * cpu affinity for the calling thread 
	 * first parameter is the pid, 0 = calling thread
	 * second parameter is the size of your cpuset
	 * third param is the cpuset in which your thread will be
	 * placed. Each bit represents a CPU
	 */
	sched_setaffinity(0, sizeof(cpuset), &cpuset);
	multi_matrix(p->matrix, p->size, p->loop_no);

	return NULL;
}
