#include <math.h>
#include "c_acceleration.h"

int c_acceleration(double vi, double vf, double t) {
	vi = vi * 1000.0 / 3600.0;
	vf = vf * 1000.0 / 3600.0;

	return round((vf - vi) / t);
}