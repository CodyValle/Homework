//author   not I
//date     many moons ago
//file  Maths.h
//description  simply a function that finds the larger of two floating-point numbers


#ifndef __MATHS__
#define __MATHS__

//finds the maximum of two numbers
//pre x0 and x1 are assigned
//post returns the larger of x0 and x1
//usage  if (max(a, b) > 0)
inline double
max(double x0, double x1);

//finds the maximum of two numbers
//pre x0 and x1 are assigned
//post returns the larger of x0 and x1
//usage  if (max(a, b) > 0)
inline double
max(double x0, double x1)
{
	return((x0 > x1) ? x0 : x1);
}

#endif
