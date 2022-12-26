#ifndef AFFINE_TRANSFORM3D_H
#define AFFINE_TRANSFORM3D_H
#include"Matrix.h"


Matrix<> Identity3D()
{
	double I[16] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };
	return Matrix<>(4, 4, I);
}

Matrix<> Translation(double x, double y, double z)
{
	double T[16] = {
		1,0,0,x,
		0,1,0,y,
		0,0,1,z,
		0,0,0,1 };
	return Matrix<>(4, 4, T);
}

Matrix<> RotationZ(double t)
{
	double	R[16] = {
		cos(t),	-sin(t), 0,0,
		sin(t),	 cos(t), 0,0,
		0,       0,      1,0,
		0,       0,      0,1 };
	return Matrix<>(4, 4, R);
}

Matrix<> RotationX(double t)
{
	double	R[16] = {
		1, 0,     0,      0,
		0, cos(t),-sin(t),0,
		0, sin(t), cos(t),0,
		0, 0,     0,      1 };
	return Matrix<>(4, 4, R);
}

Matrix<> RotationX(double c, double s)
{
	double	R[16] = {
		1, 0,     0,      0,
		0, c,    -s,0,
		0, c,     s,0,
		0, 0,     0,      1 };
	return Matrix<>(4, 4, R);
}

Matrix<> RotationY(double t)
{
	double	R[16] = {
		cos(t),  0, sin(t), 0,
		0,       1, 0,      0,
		-sin(t), 0, cos(t), 0,
		0,       0, 0,      1 };
	return Matrix<>(4, 4, R);
}

Matrix<> RotationY(double c, double s)
{
	double R[16] = {
		 c, 0, s, 0,
		 0, 1, 0, 0,
		-s, 0, c, 0,
		 0, 0, 0, 1
	};
	return Matrix<>(4, 4, R);
}

Matrix<> Scaling(double kx, double ky, double kz)
{
	double S[16] = {
		kx,0,0,0,
		0,ky,0,0,
		0,0,kz,0,
		0,0,0, 1 };
	return Matrix<>(4, 4, S);
}

Matrix<> MappingYZ()
{
	double M[16] = {
		-1,0,0, 0,
		 0,1,0, 0,
		 0,0,1, 0,
		 0,0,0, 1 };
	return Matrix<>(4, 4, M);
}

Matrix<> MappingZX()
{
	double M[16] = {
		 1, 0,0, 0,
		 0,-1,0, 0,
		 0, 0,1, 0,
		 0, 0,0, 1 };
	return Matrix<>(4, 4, M);
}

Matrix<> MappingXY()
{
	double M[16] = {
		 1,0, 0, 0,
		 0,1, 0, 0,
		 0,0,-1, 0,
		 0,0, 0, 1 };
	return Matrix<>(4, 4, M);
}

Matrix<> Mapping3Dx()
{
	double M[16] = {
		 1, 0, 0, 0,
		 0,-1, 0, 0,
		 0, 0,-1, 0,
		 0 ,0, 0, 1 };
	return Matrix<>(4, 4, M);
}

Matrix<> Mapping3Dy()
{
	double M[16] = {
		 -1,0, 0, 0,
		  0,1, 0, 0,
		  0,0,-1, 0,
		  0,0, 0, 1 };
	return Matrix<>(4, 4, M);
}

Matrix<> Mapping3Dz()
{
	double M[16] = {
		 -1, 0, 0, 0,
		  0,-1, 0, 0,
		  0, 0, 1, 0,
		  0, 0, 0, 1 };
	return Matrix<>(4, 4, M);
}

Matrix<> Mapping3D()
{
	double M[16] = {
		 -1, 0,  0, 0,
		  0,-1,  0, 0,
		  0, 0, -1, 0,
		  0, 0,  0, 1 };
	return Matrix<>(4, 4, M);
}




#endif AFFINE_TRANSFORM3D_H