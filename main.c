#include <stdio.h>
#include <stdlib.h>

#include "c_acceleration.h"
extern int asm_acceleration(double vi, double vf, double t);

struct givens {
	double vi;
	double vf;
	double t;
} typedef givens;

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

	return 0;
}