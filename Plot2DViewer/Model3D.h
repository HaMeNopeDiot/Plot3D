#ifndef MODEL_3D_H
#define MODEL_3D_H

#include <fstream>
#include "Matrix.h"
#include "AffineTeansform3D.h"
#include "Vector3D.h"
#include "Data.h"

class Model3D {
	Matrix<> Vertices;
	Matrix<int> Edges;
	Matrix<int> Facets;

	Matrix<> ProjectedVertices;
	

	Matrix<> InitialVertices;
	Matrix<> ComulativeAT;
public:
	Color_model color;


	Model3D() : Vertices(), InitialVertices(), ProjectedVertices(), Facets(), Edges(), ComulativeAT(Identity3D()) { color.red = 255; }
	Model3D(const Matrix<> _Vertices, const Matrix<int> _Facets) : Vertices(_Vertices), Facets(_Facets), InitialVertices(_Vertices) { GetEdges(); color.red = 255;
	} // - конструктор создания модели по заданным карте вершин и карте граней.
	Model3D(string, string); // - конструктор создания модели по именам  файлов, в которых лежат карт а вершин и карта граней.
	void SetEdges(); // - создание карты рёбер по заданной карте граней;
	void UpdateModel(Matrix<> _vertices, Matrix<int> _edges);

	Matrix<> GetVertices();
	Matrix<int> GetFacets();
	Matrix<int> GetEdges();

	vector<int> GetEdge3D(int);
	vector<double> GetCenter3D();

	int GetCountEdges() {    return Edges.get_count_of_cols(); }
	int GetCountVertices() { return Vertices.get_count_of_cols(); }
	int GetCountFacets() { return Facets.get_count_of_rows(); }
	
	double GetVertexX(int);
	double GetVertexY(int); // - получение координаты вершины модели с заданным номером
	double GetVertexZ(int);

	void Apply3D(Matrix<>); // - применение к модели аффинного преобразования, заданного своей матрицей
	Matrix<> Project(Matrix<>); // - проецирование модели
};



inline Model3D::Model3D(string path_vertices, string path_edges)
{
	
}

vector<double> Model3D::GetCenter3D()
{
	vector<double> para(3, 0);
	vector<double> minimal(3, 0);
	vector<double> maximum(3, 0);
	minimal[0] = Vertices.get_cell(0, 0);
	minimal[1] = Vertices.get_cell(0, 1);
	minimal[2] = Vertices.get_cell(0, 2);

	maximum[0] = minimal[0];
	maximum[1] = minimal[1];
	maximum[2] = maximum[2];


	for (int i = 1; i < Vertices.get_count_of_cols(); i++) {
		if (minimal[0] > Vertices.get_cell(0, i) / Vertices.get_cell(3, i))
			minimal[0] = Vertices.get_cell(0, i) / Vertices.get_cell(3, i);
		if (maximum[0] < Vertices.get_cell(0, i) / Vertices.get_cell(3, i))
			maximum[0] = Vertices.get_cell(0, i) / Vertices.get_cell(3, i);
		if (minimal[1] > Vertices.get_cell(1, i) / Vertices.get_cell(3, i))
			minimal[1] = Vertices.get_cell(1, i) / Vertices.get_cell(3, i);
		if (maximum[1] < Vertices.get_cell(1, i) / Vertices.get_cell(3, i))
			maximum[2] = Vertices.get_cell(1, i) / Vertices.get_cell(3, i);
		if (minimal[2] > Vertices.get_cell(2, i) / Vertices.get_cell(3, i))
			minimal[2] = Vertices.get_cell(2, i) / Vertices.get_cell(3, i);
		if (maximum[2] < Vertices.get_cell(2, i) / Vertices.get_cell(3, i))
			maximum[2] = Vertices.get_cell(2, i) / Vertices.get_cell(3, i);

	}
	para[0] = (minimal[0] + maximum[0]) / 2;
	para[1] = (minimal[1] + maximum[1]) / 2;
	para[2] = (minimal[2] + maximum[2]) / 2;
	return para;
}

inline void Model3D::SetEdges()
{
	int ver_size = Vertices.get_size();
	GetEdges();
}

inline void Model3D::UpdateModel(Matrix<> _vertices, Matrix<int> _facets)
{
	Vertices = _vertices;
	InitialVertices = Vertices;
	ComulativeAT = Identity3D();
	
	Facets = _facets;
}



Matrix<> Model3D::GetVertices()
{
	return Vertices;
}

inline Matrix<int> Model3D::GetFacets()
{
	return Facets;
}

inline Matrix<int> Model3D::GetEdges()
{
	Matrix<int> new_Edges(GetCountVertices());

	int size_facets = GetCountFacets();
	for (int i = 0; i < size_facets; i++) {
		std::vector<int> poses(3, 0);
		for (int j = 0; j < 3; j++) {
			poses[j] = Facets.get_cell(i, j);
		}
		
		new_Edges.set_cell(poses[0], poses[1], 1);
		new_Edges.set_cell(poses[1], poses[2], 1);
		new_Edges.set_cell(poses[0], poses[2], 1);

		new_Edges.set_cell(poses[1], poses[0], 1);
		new_Edges.set_cell(poses[2], poses[1], 1);
		new_Edges.set_cell(poses[2], poses[0], 1);
	}

	Edges = new_Edges;
	return Edges;
}

inline vector<int> Model3D::GetEdge3D(int pos)
{
	vector<int> para(2, 0);
	if (pos > GetCountEdges()) {
		return para;
	}
	else {
		for (int i = 0; i < Edges.get_count_of_rows(); i++) {
			for (int j = i + 1; j < Edges.get_count_of_cols(); j++) {
				if (Edges.get_cell(i, j) != 0) {
					pos--;
				}
				if (pos == 0) {
					para[0] = i;
					para[1] = j;
					return para;
				}
			}
		}
	}
}

inline double Model3D::GetVertexX(int pos)
{
	return Vertices.get_cell(0, pos) * 1.0f / Vertices.get_cell(3, pos);
}

inline double Model3D::GetVertexY(int pos)
{
	return Vertices.get_cell(1, pos) * 1.0f / Vertices.get_cell(3, pos);
}

inline double Model3D::GetVertexZ(int pos)
{
	return Vertices.get_cell(2, pos) * 1.0f / Vertices.get_cell(3, pos);
}

inline void Model3D::Apply3D(Matrix<> A)
{
	ComulativeAT = A * ComulativeAT;
	Vertices = ComulativeAT * InitialVertices;
}

inline Matrix<> Model3D::Project(Matrix<> P)
{
	Matrix<> NewVertices;
	NewVertices = P * Vertices;

	return NewVertices;
}


#endif MODEL_3D_H