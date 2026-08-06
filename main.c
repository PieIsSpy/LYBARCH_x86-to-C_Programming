#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "c_acceleration.h"
extern int asm_acceleration(double vi, double vf, double t);

struct givens {
	double vi;
	double vf;
	double t;
} typedef givens;

struct iteration_result {
	int c_ans;
	int asm_ans;
} typedef iteration_result;

struct run_summary {
	double c_time;
	double asm_time;
	int corrects;
} typedef run_summary;

run_summary silent_run(int y, givens* g) {
	run_summary summary;
	iteration_result* results = (iteration_result*)malloc(y * sizeof(iteration_result));
	
	LARGE_INTEGER frequency, start, end;
	QueryPerformanceFrequency(&frequency);
	
	QueryPerformanceCounter(&start);
	for (int i = 0; i < y; i++) {
		results[i].c_ans = c_acceleration(g[i].vi, g[i].vf, g[i].t);
	}
	QueryPerformanceCounter(&end);
	summary.c_time = ((double)(end.QuadPart - start.QuadPart) * 1000.0) / frequency.QuadPart;

	QueryPerformanceCounter(&start);
	for (int i = 0; i < y; i++) {
		results[i].asm_ans = asm_acceleration(g[i].vi, g[i].vf, g[i].t);
	}
	QueryPerformanceCounter(&end);
	summary.asm_time = ((double)(end.QuadPart - start.QuadPart) * 1000.0) / frequency.QuadPart;

	summary.corrects = 0;
	for (int i = 0; i < y; i++) {
		if (results[i].c_ans == results[i].asm_ans) {
			summary.corrects++;
		}
	}

	free(results);

	return summary;
}

run_summary verbose_run(int y, givens* g) {
	run_summary summary;
	iteration_result* results = (iteration_result*)malloc(y * sizeof(iteration_result));

	LARGE_INTEGER frequency, start, end;
	QueryPerformanceFrequency(&frequency);

	QueryPerformanceCounter(&start);
	for (int i = 0; i < y; i++) {
		results[i].c_ans = c_acceleration(g[i].vi, g[i].vf, g[i].t);
	}
	QueryPerformanceCounter(&end);
	summary.c_time = ((double)(end.QuadPart - start.QuadPart) * 1000.0) / frequency.QuadPart;

	QueryPerformanceCounter(&start);
	for (int i = 0; i < y; i++) {
		results[i].asm_ans = asm_acceleration(g[i].vi, g[i].vf, g[i].t);
	}
	QueryPerformanceCounter(&end);
	summary.asm_time = ((double)(end.QuadPart - start.QuadPart) * 1000.0) / frequency.QuadPart;

	summary.corrects = 0;
	for (int i = 0; i < y; i++) {
		if (results[i].c_ans == results[i].asm_ans) {
			summary.corrects++;
		}
	}

	printf("%5s %5s %5s %5s %10s %10s %10s\n", "i", "vi", "vf", "t", "C_ans", "Asm_ans", "Check");
	for (int i = 0; i < y; i++) {
		printf("%5d %5.1f %5.1f %5.1f %10d %10d %10s\n",
			i + 1,
			g[i].vi,
			g[i].vf,
			g[i].t,
			results[i].c_ans,
			results[i].asm_ans,
			results[i].c_ans == results[i].asm_ans ? "PASS" : "FAIL");
	}

	free(results);
	return summary;
}

givens* initialize_vectors(int y) {
	givens* vectors = (givens*)malloc(y * sizeof(givens));

	if (vectors) {
		for (int i = 0; i < y; i++) {
			vectors[i].vi = (double)rand() / RAND_MAX * 100;
			vectors[i].vf = (double)rand() / RAND_MAX * 100;
			vectors[i].t = (double)rand() / RAND_MAX * 100;
		}
	}

	return vectors;
}

void display_summary(int i, run_summary summary) {
	printf("Run %d:\n", i + 1);
	printf("C time: %lf ms\n", summary.c_time);
	printf("Asm time: %lf ms\n", summary.asm_time);
	printf("Time difference (C to Asm): %lf ms\n", summary.c_time - summary.asm_time);
	printf("Corrects: %d\n", summary.corrects);
	printf("\n");
}

int main() {
	int y;
	srand(time(NULL));

	printf("Enter input size: ");
	scanf_s("%d", &y);

	// initialize inputs
	givens* vectors = initialize_vectors(y);

	for (int i = 0; i < 30; i++) {
		run_summary summary;
		if (i + 1 == 30) {
			summary = verbose_run(y, vectors);
		}
		else {
			summary = silent_run(y, vectors);
		}
		display_summary(i, summary);
	}

	free(vectors);

	return 0;
}