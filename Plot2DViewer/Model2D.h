#ifndef MODEL_2D_H
#define MODEL_2D_H

#include <string>
#include <fstream>
#include <vector>
#include "Matrix.h"
#include <stdio.h>

#define CNT_DIMENSION 3

struct Color_model {
	int red = 0;
	int green = 0;
	int blue = 0;
};

class Model2D
{
private:
	Matrix<> InitalVertices;
	Matrix<> CumulativeAT;

	Matrix<> Vertices; //Вершины
	Matrix<int> Edges; //Ребра
public:
	Color_model color;
	double curve = 2;

	Model2D() : Vertices(), Edges() { color.red = 255; }
	Model2D(Matrix<> _Vertices, Matrix<int> _Edges) :
		Vertices(_Vertices), Edges(_Edges), InitalVertices(_Vertices), CumulativeAT(CNT_DIMENSION, 1.0) { color.red = 255; }
	Model2D(string path_vertix, string path_edges);
	
	Matrix<> GetVertices() { return Vertices; }
	Matrix<int> GetEdges() { return Edges; }
	vector<int> GetEdge(int _i);

	int GetCountEdges();
	int GetCountVertices() { return Vertices.get_count_of_cols(); }

	double GetVertexX(int num);
	double GetVertexY(int num);
	void Apply(Matrix<>);

	vector<double> Model2D::GetCenter();
};


double Model2D::GetVertexX(int num)
{
	return Vertices.get_cell(0, num) / Vertices.get_cell(2, num);
}



double Model2D::GetVertexY(int num)
{
	return Vertices.get_cell(1, num) / Vertices.get_cell(2, num);
}

void Model2D::Apply(Matrix<> A)
{
	CumulativeAT = A * CumulativeAT;
	Vertices = CumulativeAT * InitalVertices;
}

vector<int> Model2D::GetEdge(int _i)
{
	vector<int> para(2, 0);
	if (_i > GetCountEdges()) {
		return para;
	} else {  
		for (int i = 0; i < Edges.get_count_of_rows(); i++) {
			for (int j = i + 1; j < Edges.get_count_of_cols(); j++) {
				if (Edges.get_cell(i, j) != 0) {
					_i--;
				}
				if (_i == 0) {
					para[0] = i;
					para[1] = j;
					return para;
				}
			}
		}
	}
}

int Model2D::GetCountEdges()
{
	int count_of_links = 0;
	for (int i = 0; i < Edges.get_count_of_rows(); i++) {
		for (int j = i + 1; j < Edges.get_count_of_cols(); j++) {
			if (Edges.get_cell(i, j) != 0) {
				count_of_links++;
			}
		}
	}
	return count_of_links;	
}

vector<double> Model2D::GetCenter()
{
	vector<double> para(2, 0);
	vector<double> minimal(2, 0);
	vector<double> maximum(2, 0);
	minimal[0] = Vertices.get_cell(0, 0);
	minimal[1] = Vertices.get_cell(0, 1);

	maximum[0] = minimal[0];
	maximum[1] = minimal[1];


	for (int i = 1; i < Vertices.get_count_of_cols(); i++) {
		if (minimal[0] > Vertices.get_cell(0, i) / Vertices.get_cell(2, i))
			minimal[0] = Vertices.get_cell(0, i) / Vertices.get_cell(2, i);
		if (maximum[0] < Vertices.get_cell(0, i) / Vertices.get_cell(2, i))
			maximum[0] = Vertices.get_cell(0, i) / Vertices.get_cell(2, i);
		if (minimal[1] > Vertices.get_cell(1, i) / Vertices.get_cell(2, i))
			minimal[1] = Vertices.get_cell(1, i) / Vertices.get_cell(2, i);
		if (maximum[1] < Vertices.get_cell(1, i) / Vertices.get_cell(2, i))
			maximum[1] = Vertices.get_cell(1, i) / Vertices.get_cell(2, i);
		
	}
	para[0] = (minimal[0] + maximum[0]) / 2;
	para[1] = (minimal[1] + maximum[1]) / 2;
	return para;
}

Model2D::Model2D(string path_vertix, string path_edges)
{
	ifstream f_v(path_vertix);
	ifstream f_e(path_edges);

	if (!f_v.good() || !f_e.good()) {
		cout << "wrong path/no file\n";
	} else {
		vector<double> vert;
		double tmp;
		while (!f_v.eof()) {
			f_v >> tmp;
			vert.push_back(tmp);
		}
		int cnt_of_verts = vert.size() / CNT_DIMENSION;
		double **tmp_array = new double*[CNT_DIMENSION];
		for (int i = 0; i < CNT_DIMENSION; i++) {
			tmp_array[i] = new double[cnt_of_verts];
			for (int j = 0; j < cnt_of_verts; j++) {
				tmp_array[i][j] = vert[i * cnt_of_verts + j];
			}
		}
		

		int** tmp_edges = new int*[cnt_of_verts];
		for (int i = 0; i < cnt_of_verts; i++) {
			tmp_edges[i] = new int[cnt_of_verts];
			for (int j = 0; j < cnt_of_verts; j++) {
				f_e >> tmp_edges[i][j];
			}
		}


		Matrix<> tmp_ver(CNT_DIMENSION, cnt_of_verts, *tmp_array);
		Matrix<int> tmp_edg(cnt_of_verts , cnt_of_verts, *tmp_edges);
		

		Vertices = tmp_ver;
		InitalVertices = tmp_ver;
		CumulativeAT(CNT_DIMENSION, 1.0);
		Edges    = tmp_edg;


		for (int i = 0; i < cnt_of_verts; i++) {
			delete tmp_edges[i];
		}
		for (int i = 0; i < CNT_DIMENSION; i++) {
			delete[] tmp_array[i];
		}
		delete[] tmp_edges;
		delete[] tmp_array;

	}
	f_v.close();
	f_e.close();
}





#endif MODEL_2D_H
