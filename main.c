#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "c_acceleration.h"
extern int asm_acceleration(double vi, double vf, double t);

struct givens {
	double vi;
	double vf;
	double t;
} typedef givens;

struct iteration_res {
	int c_ans;
	double c_time;
	
	int asm_ans;
	double asm_time;
} typedef iteration_res;

struct run_summary {
	double c_avg;
	double asm_avg;
	int corrects;
} typedef run_summary;

iteration_res solve_iteration(givens g) {
	clock_t start, end;
	iteration_res i;

	start = clock();
	i.asm_ans = asm_acceleration(g.vi, g.vf, g.t);
	end = clock();
	i.asm_time = ((double)(end - start) / CLOCKS_PER_SEC) * 1000000.0;

	start = clock();
	i.c_ans = c_acceleration(g.vi, g.vf, g.t);
	end = clock();
	i.c_time = ((double)(end - start) / CLOCKS_PER_SEC) * 1000000.0;

	return i;
}

run_summary summarize_run(int y, iteration_res* results) {
	run_summary r;
	r.asm_avg = 0;
	r.c_avg = 0;
	r.corrects = 0;

	for (int i = 0; i < y; i++) {
		r.asm_avg += results[i].asm_time;
		r.c_avg += results[i].c_time;
		
		if (results[i].asm_ans == results[i].c_ans) {
			r.corrects++;
		}
	}

	r.asm_avg /= y;
	r.c_avg /= y;

	return r;
}

run_summary silent_run(int y, givens* g) {
	iteration_res* results = (iteration_res*)malloc(y * sizeof(iteration_res));

	if (results) {
		for (int i = 0; i < y; i++) {
			results[i] = solve_iteration(g[i]);
		}
	}

	run_summary summary =  summarize_run(y, results);
	free(results);
	return summary;
}

run_summary verbose_run(int y, givens* g) {
	iteration_res* results = (iteration_res*)malloc(y * sizeof(iteration_res));

	printf("Head of Iteration (Max 10)\n");
	printf("%5s %5s %5s %5s %10s %10s %10s %10s %10s\n", "i", "vi", "vf", "t", "C_ans", "C_time", "Asm_ans", "Asm_time", "Check");

	for (int i = 0; i < y; i++) {
		results[i] = solve_iteration(g[i]);

		if (i < 10) {
			printf("%5d %5.1f %5.1f %5.1f %10d %10.2f %10d %10.2f %10s\n",
				i + 1,
				g[i].vi,
				g[i].vf,
				g[i].t,
				results[i].c_ans,
				results[i].c_time,
				results[i].asm_ans,
				results[i].asm_time,
				(results[i].c_ans == results[i].asm_ans) ? "PASS" : "FAIL");
		}
	}

	run_summary summary = summarize_run(y, results);
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

int main() {
	int y;
	srand(time(NULL));

	printf("Enter input size: ");
	scanf_s("%d", &y);

	// initialize inputs
	givens* vectors = initialize_vectors(y);
	run_summary* results = (run_summary*)malloc(30 * sizeof(run_summary));

	for (int i = 0; i < 30; i++) {
		printf("Run %d\n", i + 1);
		if (i == 29) {
			results[i] = verbose_run(y, vectors);
		}
		else {
			results[i] = silent_run(y, vectors);
			printf("Asm_avg: %lf, C_avg: %lf, Corrects: %d\n", results[i].asm_avg, results[i].c_avg, results[i].corrects);
		}
	}

	free(vectors);
	free(results);

	return 0;
}