#include <stdio.h>

extern int asm_acceleration(double vi, double vf, double t);

int main() {
	double vi;
	double vf;
	double t;

	printf("Vi: ");
	scanf_s("%lf", &vi);
	printf("Vf: ");
	scanf_s("%lf", &vf);
	printf("t: ");
	scanf_s("%lf", &t);

	return 0;
}