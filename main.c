#include <stdio.h>
#include "c_acceleration.h"

extern int asm_acceleration(double vi, double vf, double t);

int main() {
	double vi;
	double vf;
	double t;

	printf("Vi (in km/h): ");
	scanf_s("%lf", &vi);
	printf("Vf (in km/h): ");
	scanf_s("%lf", &vf);
	printf("t (in s): ");
	scanf_s("%lf", &t);

	printf("acceleration: %d", c_acceleration(vi, vf, t));

	return 0;
}