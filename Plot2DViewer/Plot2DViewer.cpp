#include <windows.h>
#include <windowsx.h>
#include "Matrix.h"
#include "AffineTransform.h"
#include "Model2D.h"
#include "Data.h"
//#include "Scene2D.h"
#include "Scene3D.h"


#include <vector>
#define X_SIZE 512
#define Y_SIZE 512



/*
Вариант 9. Полярная система координат. 
Параметрический способ задания функции. 
Первый способ связи мировых и экранных координатных систем (при помощи параметров L, R, B, T). 
Масштабирование с сохранением положения боковых границ окна.
*/



LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);						// прототип оконной процедуры
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)		// основная процедура
{
	// Первая составляющая часть основной процедуры - создание окна: сначала описывается оконный класс wc, затем создаётся окно hWnd
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;						// имя оконной процедуры, закрепленной за данным классом
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;						// идентификатор приложения, содержащий адрес начала выделенной ему области памяти
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = 0;							// меню в оконном классе отсутствует
	wc.lpszClassName = (LPCSTR)"MainWindowClass";	// имя оконного класса, используемое при создании экземпляров окна
	RegisterClass(&wc);								// регистрация класса wc

	HWND hWnd = CreateWindow(						// hWnd - дескриптор, идентифицирующий окно; функция создания окна заполняет дескриптор hWnd ненулевым значением
		(LPCSTR)"MainWindowClass",					// имя оконного класса
		(LPCSTR)"Plot2D Viewer",					// заголовок окна
		WS_OVERLAPPEDWINDOW,						// стиль окна
		X_SIZE / 2, X_SIZE / 2, Y_SIZE, Y_SIZE,							// координаты на экране левого верхнего угла окна, его ширина и высота
		nullptr,nullptr,hInstance,nullptr);

	HMENU hMenu = CreateMenu();
	HMENU hPopMenuFile1 = CreatePopupMenu();
	//HMENU hPopMenuFile2 = CreatePopupMenu();
	//HMENU hPopMenuFile3 = CreatePopupMenu();

	HMENU hPopMenuFile100 = CreatePopupMenu();

	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hPopMenuFile1, "&File");
	{
		AppendMenu(hPopMenuFile1, MF_STRING | MF_POPUP, (UINT)hPopMenuFile100, "&New Func");
		{
			AppendMenu(hPopMenuFile100, MF_STRING, 101, "Sight");
			AppendMenu(hPopMenuFile100, MF_STRING, 102, "Box");
		}
	}

	SetMenu(hWnd, hMenu);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	// Вторая составляющая часть основной процедуры - основной цикл обработки системных сообщений, который ожидает сообщения и рассылает их соответствующим окнам
	MSG msg;
	while(GetMessage(&msg,nullptr,0,0))				// функция GetMessage выбирает из очереди сообщение и заносит его в структуру msg
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);						// функция DispatchMessage оповещает систему о необходимости вызова оконной процедуры
	}

	return 0;
}

// В основном модуле объявляется только одна глобальная переменная - создаётся объект класса Scene2D
// Все дальнейшие действия осуществляются посредством обращения к методам, реализованным в этом классе
Scene3D scene(L, R, B, T);

//std::vector<Func> func_vect;

LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)		// 
{
	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);
	ScreenToClient(hWnd, &pt);
	switch (msg)
	{
	case WM_PAINT:
	{
		HDC dc = GetDC(hWnd);
		scene.Clear(dc);
		scene.UpdateCamera();
		scene.Render3D(dc);
		ReleaseDC(hWnd, dc);
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	case WM_SIZE:
	{
		HDC dc = GetDC(hWnd);
		scene.SetResolution(dc);
		ReleaseDC(hWnd, dc);
		InvalidateRect(hWnd, nullptr, false);
		return 0;
	}

	case WM_LBUTTONDOWN:
	{

		scene.StartDragging(pt.x, pt.y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		scene.Right_Start_Dragging(pt.x, pt.y);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		if (scene.IsDragging())
		{
			scene.Drag(pt.x, pt.y);
			InvalidateRect(hWnd, nullptr, false);
		}
		/*if (scene.IsRightDragging())
		{
			scene.getOv();
			scene.getT();
			scene.getN();

			Vector3D<> Ov = scene.getOv();
			Vector3D<> T = scene.getT();
			Vector3D<> N = scene.getN();
			vector<double> coordinatesOV = Ov.get_vector();
			vector<double> coordinatesT = T.get_vector();
			vector<double> coordinatesN = N.get_vector();
			
			coordinatesOV[0] += pt.x;
			coordinatesOV[1] += pt.y;

			scene.setOv(coordinatesOV);
			scene.setT(coordinatesT);
			scene.setN(coordinatesN);
			
		}*/
		return 0;
	}
	case WM_LBUTTONUP:
	{
		scene.StopDragging();
		return 0;
	}
	case WM_RBUTTONUP:
	{
		scene.Right_StopDragging();
		return 0;
	}
	case WM_KEYDOWN:
	{

		if (::GetKeyState(VK_LCONTROL) & 0x8000) {
			if (::GetKeyState(VK_OEM_PLUS) & 0x8000) {
				scene.model_3d.Apply3D(Scaling(1.2, 1.2, 1.2));
			}
			if (::GetKeyState(VK_OEM_MINUS) & 0x8000) {
				scene.model_3d.Apply3D(Scaling(0.8, 0.8, 0.8));
			}

			/*                      =Transfer=                          */
			if (::GetKeyState(VK_LEFT) & 0x8000) {
				scene.model_3d.Apply3D(Translation(-0.5, 0, 0));
			}
			if (::GetKeyState(VK_UP) & 0x8000) {
				scene.model_3d.Apply3D(Translation(0, 0.5, 0));
			}
			if (::GetKeyState(VK_RIGHT) & 0x8000) {
				scene.model_3d.Apply3D(Translation(0.5, 0, 0));
			}
			if (::GetKeyState(VK_DOWN) & 0x8000) {
				scene.model_3d.Apply3D(Translation(0, -0.5, 0));
			}
			if (::GetKeyState(0x57) & 0x8000) {
				scene.model_3d.Apply3D(Translation(0, 0, 0.5));
			}

			if (::GetKeyState(0x53) & 0x8000) {
				scene.model_3d.Apply3D(Translation(0, 0, -0.5));
			}
		}

		if (::GetKeyState(0x58) & 0x8000) {
			scene.model_3d.Apply3D(RotationX(0.17));
		}
		
		if (::GetKeyState(0x59) & 0x8000) {
			scene.model_3d.Apply3D(RotationY(0.17));
		}

		if (::GetKeyState(0x5A) & 0x8000) {
			scene.model_3d.Apply3D(RotationZ(0.17));
		}

		if (::GetKeyState(0x4D) & 0x8000) {
			scene.model_3d.Apply3D(Mapping3D());
		}


		if (::GetKeyState(VK_SPACE) & 0x8000) {
			
						CHOOSECOLOR cc;		// структура стандартного диалогового окна
						static COLORREF acrCustClr[16];	// массив пользовательских цветов
						HBRUSH hbrush;		// дескриптор кисти
						static DWORD rgbCurrent;		// выбор начального цвета
			
						ZeroMemory(&cc, sizeof(CHOOSECOLOR));
						cc.lStructSize = sizeof(CHOOSECOLOR);
						cc.hwndOwner = hWnd;
						cc.lpCustColors = (LPDWORD)acrCustClr;
						cc.rgbResult = rgbCurrent;
			
						cc.Flags = CC_FULLOPEN | CC_RGBINIT;
			
						if (ChooseColor(&cc) == TRUE)
						{
							hbrush = CreateSolidBrush(cc.rgbResult);
							rgbCurrent = cc.rgbResult;
						}
			
						scene.model_3d.color.red = GetRValue(rgbCurrent);
						scene.model_3d.color.blue = GetBValue(rgbCurrent);
						scene.model_3d.color.green = GetGValue(rgbCurrent);
					}


		if (::GetKeyState(0x31) & 0x8000) {
			vector<double> coordinatesOv = { 1,2,8 };
			scene.setOv(coordinatesOv);

			vector<double> coordinatesT = { 0,1,0 };
			scene.setT(coordinatesT);

			vector<double> coordinatesN = { 0,0,1 };
			scene.setN(coordinatesN);
		}

		if (::GetKeyState(0x32) & 0x8000) {
			vector<double> coordinatesOv = { 4,4,4 };
			scene.setOv(coordinatesOv);

			vector<double> coordinatesT = { 0,1,0 };
			scene.setT(coordinatesT);

			vector<double> coordinatesN = { 1,1,1 };
			scene.setN(coordinatesN);
		}
		if (::GetKeyState(0x33) & 0x8000) {
			vector<double> coordinatesOv = { 1,2,2 };
			scene.setOv(coordinatesOv);

			vector<double> coordinatesT = { 0,1,0 };
			scene.setT(coordinatesT);

			vector<double> coordinatesN = { 1,0,0 };
			scene.setN(coordinatesN);
		}


		/*
		


		case 0x31:
		{
			vector<double> coordinatesOv = { 3,3,8 };
			scene.setOv(coordinatesOv);

			vector<double> coordinatesT = { 0,1,0 };
			scene.setT(coordinatesT);

			vector<double> coordinatesN = { 0,0,1 };
			scene.setN(coordinatesN);
			break;
		}

		case 0x32:
		{
			vector<double> coordinatesOv = { 4,4,4 };
			scene.setOv(coordinatesOv);

			vector<double> coordinatesT = { 0,1,0 };
			scene.setT(coordinatesT);

			vector<double> coordinatesN = { 1,1,1 };
			scene.setN(coordinatesN);
			break;
		}

		case 0x33:
		{
			vector<double> coordinatesOv = { 3,3,4 };
			scene.setOv(coordinatesOv);

			vector<double> coordinatesT = { 0,1,0 };
			scene.setT(coordinatesT);

			vector<double> coordinatesN = { 1,0,0 };
			scene.setN(coordinatesN);
			break;
		}

		case 0x34://4 -  x
		{
			scene.model_3d.Apply3D(Mapping3Dx());
			break;
		}

		case 0x35://5- y
		{
			scene.model_3d.Apply3D(Mapping3Dy());
			break;
		}

		case 0x36://6- z
		{
			scene.model_3d.Apply3D(Mapping3Dz());
			break;
		}

		case 0x37://7- xy
		{
			scene.model_3d.Apply3D(MappingXY());
			break;
		}

		case 0x38://8- zx
		{
			scene.model_3d.Apply3D(MappingZX());
			break;
		}

		case 0x39://9- yz
		{
			scene.model_3d.Apply3D(MappingYZ());
			break;
		}
		
		*/


		if (::GetKeyState(VK_TAB) & 0x8000) {
			if (::GetKeyState(VK_OEM_PLUS) & 0x8000) {
				vector<double> point = scene.model_3d.GetCenter3D();

				double cosin = point[0] / (sqrt(point[0] * point[0] + point[1] * point[1]));
				double sinus = point[1] / (sqrt(point[0] * point[0] + point[1] * point[1]));
				scene.model_3d.Apply3D(Translation(-0.5, -0.5, -0.5));
				scene.model_3d.Apply3D(RotationY(cosin, -sinus));
				scene.model_3d.Apply3D(RotationX(cosin, -sinus));
				scene.model_3d.Apply3D(RotationY(cosin, sinus));
				scene.model_3d.Apply3D(Translation(0.5, 0.5, 0.5));
				//vector<double> point = scene.Model.GetCenter();
				/*scene.Model.Apply(Translation(-point[1], -point[0]));
				scene.Model.Apply(Scalping(1.1, 1.1));
				scene.Model.Apply(Translation(point[0], point[1]));*/
			}
			if (::GetKeyState(VK_OEM_MINUS) & 0x8000) {
				/*vector<double> point = scene.Model.GetCenter();
				scene.Model.Apply(Translation(-point[1], -point[0]));
				scene.Model.Apply(Scalping(0.9, 0.9));
				scene.Model.Apply(Translation(point[0], point[1]));*/
			}
		}
		InvalidateRect(hWnd, nullptr, false);
		return 0;
	}


	case WM_MOUSEWHEEL:
	{
		/*if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			scene.IncreaseSize(pt.x, pt.y, 1.2);
		else
			scene.DecreaseSize(pt.x, pt.y, 1.2);
		InvalidateRect(hWnd, nullptr, false);
		return DefWindowProc(hWnd, msg, wParam, lParam);*/
		double F = scene.get_F();
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			F *= 1.1;
			scene.set_F(F);
		}
		else
		{
			F *= 0.9;
			scene.set_F(F);
		}
		InvalidateRect(hWnd, nullptr, false);
		return DefWindowProc(hWnd, msg, wParam, lParam);

	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	default:
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	}
	return 0;
}


//POINT point;
//point.x = GET_X_LPARAM(lParam);
//point.y = GET_Y_LPARAM(lParam);
//ScreenToClient(hWnd, &point);
//switch (msg)
//{
//case WM_PAINT:
//{
//	HDC dc = GetDC(hWnd);
//	scene.Clear(dc);				// Вызов реализованного в классе Camera2D метода, отвечающего за очистку рабочей области окна hWnd							
//	//scene.Plot(dc, Sinusoid);         // Вызов реализованного в классе Scene2D метода, отвечающего за отрисовку графика синусоиды
//	//scene.Plot(dc, Parabola);
//	//scene.PlotParametrical(dc, star_x, star_y);
//	//scene.PlotParametrical(dc, curve_x, curve_y);
//	scene.Plot(dc, Line, false);
//	scene.Render(dc);
//	ReleaseDC(hWnd, dc);
//	return DefWindowProc(hWnd, msg, wParam, lParam);
//}
//
//case WM_SIZE:
//{
//	HDC dc = GetDC(hWnd);
//	scene.SetResolution(dc);
//	ReleaseDC(hWnd, dc);
//	InvalidateRect(hWnd, nullptr, false);
//	return 0;
//}
//
//case WM_LBUTTONDOWN:
//{
//	scene.StartDragging(point.x, point.y);
//	return 0;
//}
//case WM_MOUSEMOVE:
//{
//	if (scene.IsDragging())
//	{
//		scene.Drag(point.x, point.y);
//		InvalidateRect(hWnd, nullptr, false);
//	}
//	return 0;
//}
//case WM_LBUTTONUP:
//{
//	scene.StopDragging();
//	return 0;
//}
//
//case WM_KEYDOWN:
//{
//
//	PAINTSTRUCT ps;
//	HDC dc = BeginPaint(hWnd, &ps);
//	if (::GetKeyState(VK_LCONTROL) & 0x8000) {
//		if (::GetKeyState(VK_OEM_PLUS) & 0x8000) {
//			scene.Resize(point.x, point.y, 1.2);
//		}
//		if (::GetKeyState(VK_OEM_MINUS) & 0x8000) {
//			scene.Resize(point.x, point.y, 0.8);
//		}
//
//		/*                      =Transfer=                          */
//		if (::GetKeyState(VK_LEFT) & 0x8000) {
//			scene.Model.Apply(Translation(-0.5, 0));
//		}
//		if (::GetKeyState(VK_UP) & 0x8000) {
//			scene.Model.Apply(Translation(0, 0.5));
//		}
//		if (::GetKeyState(VK_RIGHT) & 0x8000) {
//			scene.Model.Apply(Translation(0.5, 0));
//		}
//		if (::GetKeyState(VK_DOWN) & 0x8000) {
//			scene.Model.Apply(Translation(0, -0.5));
//		}
//
//	}
//	else {
//		/*                      =Moving=                          */
//		if (::GetKeyState(VK_LEFT) & 0x8000) {
//			scene.Move(4, 0);
//		}
//		if (::GetKeyState(VK_UP) & 0x8000) {
//			scene.Move(0, 4);
//		}
//		if (::GetKeyState(VK_RIGHT) & 0x8000) {
//			scene.Move(-4, 0);
//		}
//		if (::GetKeyState(VK_DOWN) & 0x8000) {
//			scene.Move(0, -4);
//		}
//
//		/*                      =Color=                          */
//		if (::GetKeyState(VK_SPACE) & 0x8000) {
//
//			CHOOSECOLOR cc;		// структура стандартного диалогового окна
//			static COLORREF acrCustClr[16];	// массив пользовательских цветов
//			HBRUSH hbrush;		// дескриптор кисти
//			static DWORD rgbCurrent;		// выбор начального цвета
//
//			ZeroMemory(&cc, sizeof(CHOOSECOLOR));
//			cc.lStructSize = sizeof(CHOOSECOLOR);
//			cc.hwndOwner = hWnd;
//			cc.lpCustColors = (LPDWORD)acrCustClr;
//			cc.rgbResult = rgbCurrent;
//
//			cc.Flags = CC_FULLOPEN | CC_RGBINIT;
//
//			if (ChooseColor(&cc) == TRUE)
//			{
//				hbrush = CreateSolidBrush(cc.rgbResult);
//				rgbCurrent = cc.rgbResult;
//			}
//
//			scene.Model.color.red = GetRValue(rgbCurrent);
//			scene.Model.color.blue = GetBValue(rgbCurrent);
//			scene.Model.color.green = GetGValue(rgbCurrent);
//		}
//
//
//
//
//	}
//
//	if (::GetKeyState(0x52) & 0x8000) {
//		int random = 1 + rand() % 4;
//		switch (random)
//		{
//		case 1:
//			scene.Model.Apply(Translation(-1, 0));
//			break;
//		case 2:
//			scene.Model.Apply(Translation(0, 1));
//			break;
//		case 3:
//			scene.Model.Apply(Translation(1, 0));
//			break;
//		case 4:
//			scene.Model.Apply(Translation(0, -1));
//			break;
//		}
//
//	}
//
//	if (::GetKeyState(VK_RCONTROL) & 0x8000) {
//		if (::GetKeyState(VK_OEM_PLUS) & 0x8000) {
//			scene.Model.curve += 0.1;
//		}
//		if (::GetKeyState(VK_OEM_MINUS) & 0x8000) {
//			scene.Model.curve -= 0.1;
//		}
//	}
//
//	if (::GetKeyState(VK_LSHIFT) & 0x8000) {
//		if (::GetKeyState(VK_OEM_PLUS) & 0x8000) {
//			scene.Model.Apply(Rotation(0.99984769515, 0.01745240643));
//		}
//		if (::GetKeyState(VK_OEM_MINUS) & 0x8000) {
//			scene.Model.Apply(Rotation(0.99984769515, -0.01745240643));
//		}
//	}
//
//	if (::GetKeyState(VK_RSHIFT) & 0x8000) {
//		if (::GetKeyState(VK_OEM_PLUS) & 0x8000) {
//			scene.Model.Apply(Scalping(1.1, 1.1));
//		}
//		if (::GetKeyState(VK_OEM_MINUS) & 0x8000) {
//			scene.Model.Apply(Scalping(0.9, 0.9));
//		}
//	}
//
//	if (::GetKeyState(VK_TAB) & 0x8000) {
//		if (::GetKeyState(VK_OEM_PLUS) & 0x8000) {
//			vector<double> point = scene.Model.GetCenter();
//
//			double cosin = point[0] / (sqrt(point[0] * point[0] + point[1] * point[1]));
//			double sinus = point[1] / (sqrt(point[0] * point[0] + point[1] * point[1]));
//			scene.Model.Apply(Translation(-0.5, -0.5));
//			scene.Model.Apply(Rotation(cosin, -sinus));
//			scene.Model.Apply(Reflection_Y());
//			scene.Model.Apply(Rotation(cosin, sinus));
//			scene.Model.Apply(Translation(0.5, 0.5));
//			//vector<double> point = scene.Model.GetCenter();
//			/*scene.Model.Apply(Translation(-point[1], -point[0]));
//			scene.Model.Apply(Scalping(1.1, 1.1));
//			scene.Model.Apply(Translation(point[0], point[1]));*/
//		}
//		if (::GetKeyState(VK_OEM_MINUS) & 0x8000) {
//			/*vector<double> point = scene.Model.GetCenter();
//			scene.Model.Apply(Translation(-point[1], -point[0]));
//			scene.Model.Apply(Scalping(0.9, 0.9));
//			scene.Model.Apply(Translation(point[0], point[1]));*/
//		}
//	}
//
//	EndPaint(hWnd, &ps);
//	InvalidateRect(hWnd, nullptr, false);
//
//
//	return DefWindowProc(hWnd, msg, wParam, lParam);
//}
//case WM_MOUSEWHEEL:
//{
//	int wheel = GET_WHEEL_DELTA_WPARAM(wParam);
//	if (wheel > 0)
//		scene.Resize(point.x, point.y, 1.2);
//	else
//		scene.Resize(point.x, point.y, 0.8);
//
//
//
//	InvalidateRect(hWnd, nullptr, false);
//	return DefWindowProc(hWnd, msg, wParam, lParam);
//}
//
//case WM_DESTROY:
//{
//	PostQuitMessage(0);
//	return 0;
//}
//case WM_COMMAND:
//{
//	//switch (LOWORD(wParam)) {
//	//case IDM_NEW_SIGHT:
//	//{
//	//	PAINTSTRUCT ps;
//	//	HDC dc = BeginPaint(hWnd, &ps);
//	//	obj_array.push_back(new Sight(30, X_SIZE / 2, Y_SIZE / 2));
//	//	EndPaint(hWnd, &ps);
//	//	InvalidateRect(hWnd, NULL, TRUE);
//	//	break;
//	//}
//	//case IDM_NEW_BOX:
//	//{
//	//	PAINTSTRUCT ps;
//	//	HDC dc = BeginPaint(hWnd, &ps);
//	//	obj_array.push_back(new Box(30, X_SIZE / 2, Y_SIZE / 2));
//	//	EndPaint(hWnd, &ps);
//	//	InvalidateRect(hWnd, NULL, TRUE);
//	//	break;
//	//}
//	//case IDM_EXIT:
//	//{
//	//	break;
//	//	// Process other menu commands. 
//	//}
//	//default:
//	//{
//	//	break;
//	//	return 0;
//	//}
//	//}
//}
//default:
//{
//
//	return DefWindowProc(hWnd, msg, wParam, lParam);
//}
//}
//return 0;