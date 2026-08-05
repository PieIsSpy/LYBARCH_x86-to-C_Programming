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
};

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

double silent_run(int y, givens* g) {
	int x;
	iteration_res* results = (iteration_res*)malloc(y * sizeof(iteration_res));

	for (int i = 0; i < y; i++) {
		x = i;
	}
	return x;
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
	iteration_res* results = (iteration_res*)malloc(y * sizeof(iteration_res));

	for (int i = 1; i <= 30; i++) {
		if (i == 30) {
			/*printf("displaying run 30:\n");
			verbose_run(y);*/
		}
		else {
			//printf("run %d result: %d\n", i, silent_run(y));
		}
	}

	free(vectors);
	free(results);

	return 0;
}