#ifndef SCENE_3D_H
#define SCENE_3D_H

#include <vector>
#include "Model3D.h"
#include "Camera3D.h"
#include "Light3D.h"
#include "Model2D.h"

class Scene3D : public Camera3D {
	//std::vector<Model3D> models;
public:
	Model3D model_3d;
	 
	Model3D XYZ;
	Camera3D camera;
	//std::vector<void> lighs;
	//textures

	

	Scene3D(double _L, double _R, double _B, double _T) : Camera3D(_L, _R, _B, _T)
	{
		/*double vert[] = { 1,1,1,1,0,0,0,0,
						  0,0,1,1,1,1,0,0,
						  0,1,0,1,0,1,0,1,
						  1,1,1,1,1,1,1,1 };*/
		
		// 8, 4
		double vertices[] = { 0, 1, 1, 0, 0, 1, 1, 0,
						  0, 0, 1, 1, 0, 0, 1, 1,
						  0, 0, 0, 0, 1, 1, 1, 1,
						  1, 1, 1, 1, 1, 1, 1, 1 };

		int facets[] = { 1, 4, 2,
					  1, 4, 3,
					  1, 4, 5,
					  1, 4, 8,
					  2, 3, 2,
					  5, 8, 5,
					  6, 7, 2,
					  6, 7, 3,
					  6, 7, 5,
					  6, 7, 8,
					  1, 2, 5,
					  6, 2, 5,
					  4, 3, 8,
					  7, 3, 8};


		/*int verg[] = { 1,2,3,
					   2,3,4,
					   1,3,7,
					   3,5,7,
					   5,6,7,
					   6,7,8,
					   3,4,6,
					   3,5,6,
					   1,2,7,
					   2,7,8,
					   3,4,5,
					   4,5,6 };*/



		


		double coord_Vertices[] = {0, 3,  0,  0,
						   0,  0, 3,  0,
						   0,  0,  0, 3,
						   1,  1,  1,  1 };
		

		int coord_Faces[] = { 1,2,1,
						  1,3,1,
						  1,4,1 };

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				coord_Faces[i * 3 + j]--;
			}
		}

		for (int i = 0; i < 14; i++) {
			for (int j = 0; j < 3; j++) {
				facets[i * 3 + j]--;
			}
		}

		Matrix<> Vertices(4, 8, vertices);
		Matrix<int> Faces(14, 3, facets);

		Matrix<> XYZVER(4, 4, coord_Vertices);
		Matrix<int> XYZVERG(3, 3, coord_Faces);

		XYZ.UpdateModel(XYZVER, XYZVERG);

		model_3d.UpdateModel(Vertices, Faces);
	}

	void Render3D(HDC dc, bool axes = true)
	{
		Model2D model2d(model_3d.Project(WorldToProject), model_3d.GetEdges());
		Model2D XYZ2D(XYZ.Project(WorldToProject), XYZ.GetEdges());

		HPEN redPen, blackPen;
		redPen = CreatePen(PS_SOLID, 1, RGB(model_3d.color.red, model_3d.color.green, model_3d.color.blue));
		//Rainbow(model_3d.color.red, model_3d.color.green, model_3d.color.blue);
		blackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		SelectObject(dc, redPen);

		for (int i = 1; i <= model2d.GetCountEdges(); i++)
		{

			vector<int> row = model2d.GetEdge(i);
			MoveTo(model2d.GetVertexX(row[0]), model2d.GetVertexY(row[0]));
			LineTo(dc, model2d.GetVertexX(row[1]), model2d.GetVertexY(row[1]));
		}
	

		SelectObject(dc, blackPen);

		if (axes) {
			int XYZ2D_size = XYZ2D.GetCountEdges();
			for (int i = 1; i <= XYZ2D_size; i++)
			{
				vector<int> row = XYZ2D.GetEdge(i);
				/*if (row[0] == 2 && row[1] == 3)
					continue;
				if (row[0] == 4 && row[1] == 3)
					continue;*/

				MoveTo(XYZ2D.GetVertexX(row[0]), XYZ2D.GetVertexY(row[0]));
				LineTo(dc, XYZ2D.GetVertexX(row[1]), XYZ2D.GetVertexY(row[1]));
			}
		}
	}


};


#endif SCENE_3D_H