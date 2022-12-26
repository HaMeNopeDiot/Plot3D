#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H

Matrix<> Translation(double x, double y)
{
	double T[9] = {
		1, 0, x,
		0, 1, y,
		0, 0, 1};
	return Matrix<>(3, T);
}

Matrix<> Identity()
{
	Matrix<> T(3, 1.0);
	return T;
}

Matrix<> Rotation(double c, double s)
{
	double T[9] = {
		c, -s, 0,
		s,  c, 0,
		0,  0, 1 };
	return Matrix<>(3, T);
}

Matrix<> Scalping(double kx, double ky)
{
	Matrix<> T(3, 1.0);
	T.Myltyply_column(0, kx);
	T.Myltyply_column(1, ky);
	return T;
}

Matrix<> Reflection_X()
{
	Matrix<> T(3, 1.0);
	T.Myltyply_column(1, -1);
	return T;
}

Matrix<> Reflection_Y()
{
	Matrix<> T(3, 1.0);
	T.Myltyply_column(0, -1.0);
	return T;
}

Matrix<> Reflection_O()
{
	Matrix<> T(3, 1.0);
	T.Myltyply_column(0, -1);
	T.Myltyply_column(1, -1);
	return T;
}

// ����� ��������� ����������� �������, ������������ ������� ������� ��:
// Identity() - ������������� ��;
// Rotation(t) - ������� �� ���� t;
// Rotation(c, s) - ������� �� ����, ������� � ����� �������� ��������������� ��������� c � s;
// Scaling(kx, ky) - ���������������;
// Mapping (��������� ���� ���������) - �� �������, ��� �������� ������ ��������� ����� ������������ ������� Scaling.

// � ���������� ������� ������������ ����������� ������� �� ������.
// �������������� ���� � ������ �������� �������� ������.
// ��������, ����� �������, ��������� ��������� �������,
// � ����������� ���������������� ��������� � �������������.

#endif AFFINE_TRANSFORM_H
