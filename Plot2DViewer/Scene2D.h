#ifndef SCENE_2D_H
#define SCENE_2D_H

#include "Camera2D.h"

#include <ctime>
#include <vector>

class Scene2D : public Camera2D
{
private:
	typedef double (*Func)(double);
public:
	Model2D Model;
	Scene2D(double L, double R, double B, double T) : Camera2D(L, R, B, T)
	{
		/*int edges[] = {
			0, 1, 0, 1, 1,
			1, 0, 1, 0, 0,
			0, 1, 0, 1, 0,
			1, 0, 1, 0, 1,
			1, 0, 0, 1, 0
		};

		double vertices[] = {
			1, 1, 5, 5, 6,
			4, 1, 1, 4, 11,
			1, 1, 1, 1, 2
		};*/

		double vertices[] = {
			1, 1, 2, 2, 3, 3, 4, 4, 3, 3, 2, 2,
			2, 3, 3, 4, 4, 3, 3, 2, 2, 1, 1, 2,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
		};

		int edges[] = {
			0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
		};

		Matrix<> m_vertices(3, 12, vertices);
		Matrix<int> m_edges(12, 12, edges);

		Model2D tmp(m_vertices, m_edges);
		Model = tmp;
	}
	void Plot(HDC dc, Func f,  bool axes=true)
	{
		if (axes)
			Axes(dc);

		srand(time(0));
		
		Color color;
		color.red = 250;
		color.blue = 0;
		color.green = 0;

		HPEN redPen, edgePen;
		redPen = CreatePen(PS_SOLID, 2, RGB(color.red, color.green, color.blue));
		edgePen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		SelectObject(dc, redPen);

		int n = W;
		double t;

		MoveTo(tMin, f(tMin));

		
		MoveTo(tMin, f(tMin));

		for (int i = 1; i <= n; i++)
		{
			t = tMin + (tMax - tMin) * i / n;
			LineTo(dc, t, f(t));

		}

		SelectObject(dc, edgePen);
		MoveTo(tMin, tMin);
		LineTo(dc, tMin, tMax);
		LineTo(dc, tMax, tMax);
		LineTo(dc, tMax, tMin);
		LineTo(dc, tMin, tMin);
		// Построение графика функции f
		// Здесь передаваемая в качестве параметра функция f вызывается как обычная функция:
		// f(x);
	}
	void PlotParametrical(HDC dc, Func func_1, Func func_2 = NULL, bool axes = false)
	{
		if (axes)
			Axes_Polar(dc);

		HPEN redPen, edgePen;
		redPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
		edgePen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		SelectObject(dc, redPen);

		int n = W;
		double t;

		MoveTo(tMin, func_1(tMin));

		if (func_2)
			MoveTo(func_1(tMin), func_2(tMin));
		else
			MoveTo(tMin, func_1(tMin));

		for (int i = 1; i <= n; i++)
		{
			t = tMin + (tMax - tMin) * i * 1.0f / n;
			if (func_2)
				LineTo(dc, func_1(t), func_2(t));
			else
				LineTo(dc, t, func_1(t));

		}

		SelectObject(dc, edgePen);
		MoveTo(tMin, tMin);
		LineTo(dc, tMin, tMax);
		LineTo(dc, tMax, tMax);
		LineTo(dc, tMax, tMin);
		LineTo(dc, tMin, tMin);
		// Построение графика функции f
		// Здесь передаваемая в качестве параметра функция f вызывается как обычная функция:
		// f(x);
	}


	void Render(HDC dc, bool axes = true)
	{
		if (axes)
			Axes(dc);

		HPEN customPen, blackPen;
		customPen = CreatePen(PS_SOLID, Model.curve, RGB(Model.color.red, Model.color.green, Model.color.blue));
		blackPen = CreatePen(PS_SOLID, Model.curve, RGB(0, 0, 0));
		SelectObject(dc, customPen);

		for (int i = 1; i <= Model.GetCountEdges(); i++) {

			vector<int> para = Model.GetEdge(i);
			MoveTo(Model.GetVertexX(para[0]), Model.GetVertexY(para[0]));
			LineTo(dc, Model.GetVertexX(para[1]), Model.GetVertexY(para[1]));
		}

		SelectObject(dc, blackPen);
	}
};

#endif SCENE_2D_H
