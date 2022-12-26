#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <windows.h>

struct Color {
	int red = 0;
	int green = 0;
	int blue = 0;
};


class Camera2D
{
protected:
	double L, R, B, T;					// ������� ���������� ������ ������� ������� ����
	int W, H;							// ���������� ������� ������� ����
	int WorldToScreenX(double Xw)		// ������� �� ������� ��������� � �������� (��� ��������)
	{
		return (int)(((Xw - L) * 1.0f / (R - L)) * W);
	}
	int WorldToScreenY(double Yw)		// ������� �� ������� ��������� � �������� (��� ��������)
	{
		return (int)(((T - Yw) * 1.0f / (T - B)) * H);
	}
	double ScreenToWorldX(int Xs)		// ������� �� �������� ��������� � ������� (��� ��������)
	{
		return L + (R - L) * ((Xs + 0.5) * 1.0f / W);
	}
	double ScreenToWorldY(int Ys)		// ������� �� �������� ��������� � ������� (��� ��������)
	{
		return T - (T - B) * ((Ys + 0.5) * 1.0f / H);
	}
private:
	double posX, posY;					// ������� ������������ ������� � ������� ����������� (��� ������� MoveTo � LineTo)
	bool isDragging = false;
	bool RisDragging = false;
	double previousX = 0, previousY = 0;
public:
	Camera2D() {}
	Camera2D(double L, double R, double B, double T) : L(L), R(R), B(B), T(T) {}
	void Clear(HDC dc)
	{
		Rectangle(dc, 0, 0, W, H); // ������� ������� ������� ����
	}
	void SetResolution(HDC dc)
	{
		RECT r;
		GetClientRect(WindowFromDC(dc), &r);

		W = r.right - 1;
		H = r.bottom - 1;

		double res = double(W) / double(H);

		/*double B_new = (B + T) * 1.0f / 2 - (R - L) * 1.0f / 2 * res;
		double T_new = (B + T) * 1.0f / 2 + (R - L) * 1.0f / 2 * res;
		B = B_new;
		T = T_new;*/

		double new_L = (R + L) / 2 - ((T - B) / 2) * res;
		double new_R = (R + L) / 2 + ((T - B) / 2) * res;
		L = new_L;
		R = new_R;

		// ������ ��������� ���������� ��� ��������� �������� ����
		// � ��� �������� �������� ������� W, H, � ����� ������������� �������� ���������� L, R, B, T ����� �������, ����� ���������� ���������� �������� �� ������������ ����
	}

	void MoveTo(double X, double Y)
	{
		posX = X;
		posY = Y;
		// ����������� ������������ ������� (posX, posY)
		// �������� ��������, ��� �� ��������� � ������� �����������
	}

	void LineTo(HDC dc, double X, double Y)
	{
		MoveToEx(dc, WorldToScreenX(posX), WorldToScreenY(posY), NULL);
		::LineTo(dc, WorldToScreenX(X), WorldToScreenY(Y));
		MoveTo(X, Y);
		// ��������� ����� �� ������� ������� ������������ ������� � ����� (X, Y) � ��� ����������� � ��� �����
		// �������� ��������, ��� �� ��������� � ������� �����������
		// ��� ��������� ����� ����� ���� ������������ WinApi �������
		// ::MoveToEx(dc, Xs, Ys, nullptr) � ::LineTo(dc, Xs, Ys)
	}

	void Axes(HDC dc)
	{
		// ��������� ������������ ����
		HPEN gridPen, blackPen;
		gridPen = CreatePen(PS_SOLID, 1, RGB(60, 60, 60));
		blackPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		SelectObject(dc, gridPen);


		for (int i = L; i <= R; i++)
		{
			MoveTo(i, 0);
			LineTo(dc, i, T);
			MoveTo(i, 0);
			LineTo(dc, i, B);
		}

		for (int i = B; i <= T; i++)
		{
			MoveTo(0, i);
			LineTo(dc, L, i);
			MoveTo(0, i);
			LineTo(dc, R, i);
		}

		SelectObject(dc, blackPen);

		MoveTo(0, B);
		LineTo(dc, 0, T);

		MoveTo(L, 0);
		LineTo(dc, R, 0);
	}

	void Axes_Polar(HDC dc)
	{
		// ��������� ������������ ����
		HPEN gridPen, blackPen;
		gridPen = CreatePen(PS_SOLID, 1, RGB(60, 60, 60));
		blackPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		SelectObject(dc, gridPen);


		for (int i = tMax; i > 0; i--)
		{
			MoveTo(R, T);
			Ellipse(dc, (R - L) / 2 - i, (R - L) / 2 - i, (R - L) / 2 + i, (R - L) / 2 + i);
		}

	}

	void Move(double X, double Y)
	{
		L = L - (R - L) * X * 1.0f / W;
		R = R - (R - L) * X * 1.0f / W;
		B = B + (T - B) * Y * 1.0f / H;
		T = T + (T - B) * Y * 1.0f / H;
	}
	void StopDragging()
	{
		isDragging = false;
	}
	void Right_StopDragging()
	{
		RisDragging = false;
	}

	bool IsDragging()
	{
		return isDragging;
	}

	bool IsRightDragging()
	{
		return RisDragging;
	}

	void Resize(int X, int Y, double multyply)
	{

		double Xw = ScreenToWorldX(X);
		double Yw = ScreenToWorldY(Y);

		L = Xw - (Xw - L) * 1.0f / multyply;
		R = Xw + (R - Xw) * 1.0f / multyply;
		B = Yw - (Yw - B) * 1.0f / multyply;
		T = Yw + (T - Yw) * 1.0f / multyply;
	}

	void Drag(int X, int Y, double digit = 4)
	{
		L = L + (R - L) * 1.0f / W * (previousX - ScreenToWorldX(X)) * digit;
		R = R + (R - L) * 1.0f / W * (previousX - ScreenToWorldX(X)) * digit;

		B = B + (T - B) * 1.0f / H * (previousY - ScreenToWorldY(Y)) * digit;
		T = T + (T - B) * 1.0f / H * (previousY - ScreenToWorldY(Y)) * digit;
	}

	void StartDragging(int X, int Y)
	{
		previousX = ScreenToWorldX(X);
		previousY = ScreenToWorldY(Y);
		isDragging = true;

	}
	
	void Right_Start_Dragging(int X, int Y)
	{
		previousX = ScreenToWorldX(X);
		previousY = ScreenToWorldY(Y);
		RisDragging = true;
	}

};

#endif CAMERA_2D_H
