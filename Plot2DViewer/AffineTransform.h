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

// Здесь требуется реализовать функции, возвращающие матрицы базовых АП:
// Identity() - тождественное АП;
// Rotation(t) - поворот на угол t;
// Rotation(c, s) - поворот на угол, косинус и синус которого пропорциональны величинам c и s;
// Scaling(kx, ky) - масштабирование;
// Mapping (различные виды отражений) - по желанию, для создания матриц отражения можно использовать функцию Scaling.

// В приведённом примере используется конструктор матрицы из списка.
// Приветствуются идеи о других способах создания матриц.
// Например, вызов функции, создающей единичную матрицу,
// с последующим переопределением некоторых её коэффициентов.

#endif AFFINE_TRANSFORM_H
