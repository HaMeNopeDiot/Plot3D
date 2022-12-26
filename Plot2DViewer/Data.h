#ifndef DATA_H
#define DATA_H

#include <cmath>
# define M_PI           3.14159265358979323846  /* pi */

struct Position {
	double x, y, z;
};


double					// Исходные значения параметров L, R, B, T
	L = -M_PI,
	R = M_PI,
	B = -1.5,
	T = 1.5;

double
	tMin = -10,
	tMax = 10,
	a = 1,
	b = 2,
	p = 1;


double Parabola(double x, double multyply_x = 1, double bias_x = 0, double bias_y = 2)
{
	return pow(multyply_x * x + bias_x, 2) + bias_y;
}

double Parabola(double x)
{
	return pow(x, 2) + 2;
}

double Sinusoid(double x)
{
	return sin(x);
}

double Line(double x)
{
	return -x + 1;
}

double curve_x(double t)
{
	return t * (2 - t);
}

double curve_y(double t)
{
	return t * t * (2 - t);
}

double star_x(double t)
{
	return pow(cos(t), 3);
}

double star_y(double t)
{
	return pow(sin(t), 3);
}


#endif DATA_H
