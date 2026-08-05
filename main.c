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
	clock_t t;
	iteration_res i;

	t = clock();
	i.asm_ans = asm_acceleration(g.vi, g.vf, g.t);
	t = clock() - t;
	i.asm_time = t;

	t = clock();
	i.c_ans = c_acceleration(g.vi, g.vf, g.t);
	t = clock() - t;
	i.c_time = t;

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

	return summarize_run(y, results);
}

void verbose_run(int y) {
	printf("| i |    vi    |    vf     |     t     | C Ans |  C Time  | Asm Ans | Asm Time | Correctness |\n");
	for (int i = 0; i < y; i++) {
		printf("%d\n", i + 1);
	}
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
		if (i == 29) {
			printf("Run 30 done\n");
		}
		else {
			printf("Run %d\n", i + 1);
			results[i] = silent_run(y, vectors);
			printf("Asm_avg: %lf, C_avg: %lf, Corrects: %d\n", results[i].asm_avg, results[i].c_avg, results[i].corrects);
		}
	}

	free(vectors);
	free(results);

	return 0;
}