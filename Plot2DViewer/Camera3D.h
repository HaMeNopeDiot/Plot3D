#ifndef CAMERA_3D_H
#define CAMERA_3D_H

#include "Camera2D.h"
#include "Vector3D.h"

class Camera3D : public Camera2D {
	//Position location;
	//Position target;
	//double fov;
	//double roll;
protected:

	Vector3D<> N, T, Ov;
	double F;

	Matrix<> WorldToView, ViewToProject, WorldToProject;
public:
	Camera3D() : Camera2D() {}
	Camera3D(double _L, double _R, double _B, double _T);

	void   set_F(double val) { F = val; }
	double get_F() { return F; }

	Vector3D<> getN()  { return N; }
	Vector3D<> getT()  { return T; }
	Vector3D<> getOv() { return Ov; }

	Vector3D<> getIv();
	Vector3D<> getKv();
	Vector3D<> getJv();

	void setN(vector<double>   _N);
	void setT(vector<double>   _T);
	void setOv(vector<double> _Ov);


	void UpdateCamera();
};

Camera3D::Camera3D(double _L, double _R, double _B, double _T) : Camera2D(_L, _R, _B, _T)
{
	std::vector<double> tmp0 = { 1, 1, 1 };
	std::vector<double> tmp1 = { 0, 0, 1 };
	std::vector<double> tmp2 = { 2, 1.25, 1 };

	Ov.set_vector(tmp0);
	T.set_vector(tmp1);
	N.set_vector(tmp2);

	set_F(5);
	UpdateCamera();
}

inline Vector3D<> Camera3D::getIv()
{
	Vector3D<> Iv(3);
	Iv = T.vector_multyply(N);
	Iv = Iv / Iv.get_norm_of_vec();
	return Iv;
}

inline Vector3D<> Camera3D::getKv()
{
	Vector3D<> Kv(3);
	Kv = N;
	Kv = Kv / Kv.get_norm_of_vec();
	return Kv;
}

inline Vector3D<> Camera3D::getJv()
{
	Vector3D<> Jv(3), Kv(3), Iv(3);

	Kv = getKv();
	Iv = getIv();
	Jv = Kv.vector_multyply(Iv);

	return Jv;
}

inline void Camera3D::setN(vector<double> _N)
{
	N.set_vector(_N);
}

inline void Camera3D::setT(vector<double> _T)
{
	T.set_vector(_T);
}

inline void Camera3D::setOv(vector<double> _Ov)
{
	Ov.set_vector(_Ov);
}

inline void Camera3D::UpdateCamera()
{
	Vector3D<> Iv(3), Jv(3), Kv(3);

	Iv = getIv();
	Jv = getJv();
	Kv = getKv();


	std::vector<double> WtoV = {
		Iv.get_elem(0), Iv.get_elem(1), Iv.get_elem(2), -Iv.scalar_multyply(Ov),
		Jv.get_elem(0), Jv.get_elem(1), Jv.get_elem(2), -Jv.scalar_multyply(Ov),
		Kv.get_elem(0), Kv.get_elem(1), Kv.get_elem(2), -Kv.scalar_multyply(Ov),
		0,              0,              0,               1
	};

	double Z = ( - 1.0f * 1.0f )/ F;
	std::vector<double> VtoP = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, Z, 1
	};

	WorldToView.set_matrix(WtoV, 4);
	ViewToProject.set_matrix(VtoP, 3, 4);

	WorldToProject = ViewToProject * WorldToView;
}

#endif CAMERA_3D_H

