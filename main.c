#include <stdio.h>
#include <stdlib.h>
#include "c_acceleration.h"

extern int asm_acceleration(double vi, double vf, double t);

struct givens {
	double vi;
	double vf;
	double t;
} typedef givens;

int main() {
	int n = 0;

	scanf("%d", &n);

	givens* vectors = (givens*)malloc(n * sizeof(givens));

	for (int i = 0; i < n; i++) {
		scanf("%lf", &vectors[i].vi);
		scanf("%lf", &vectors[i].vf);
		scanf("%lf", &vectors[i].t);
	}

	for (int i = 0; i < n; i++) {
		printf("%d\n", c_acceleration(vectors[i].vi, vectors[i].vf, vectors[i].t));
	}
	
	free(vectors);

	return 0;
}