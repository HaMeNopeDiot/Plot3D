#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;


template <typename Cell = double>
class Matrix
{
private:
	int size;
	int rows, cols;
	Cell **cells;
	void AllocateCells(int);
	void AllocateCells(int, int);
	void FreeCells();
public:
	Matrix() : size(0), cells(nullptr) {}	// Конструктор по умолчанию
	Matrix(const Matrix&);					// Конструктор копирования
	Matrix(int, Cell number = 0);							// Конструктор нулевой матрицы
	Matrix(int, Cell*);						// Конструктор матрицы из списка
	Matrix(int, int, Cell*);				// Конструктор матрицы из списка

	~Matrix();								// Деструктор


	int get_count_of_rows() { return rows; }
	int get_count_of_cols() { return cols; }

	int  get_size() { return size; }
	Cell get_cell(int i, int j);

	void set_cell(int i, int j, Cell val);
	bool set_matrix(vector<Cell>, int, int cols = -1);

	void Myltyply_column(int column, double multy);
	void Myltyply_row(int row      , double multy);

	Cell &operator()(int i, int j) { return cells[i-1][j-1]; }
	
	Matrix& operator = (const Matrix&);		// Перегрузка оператора присваивания
	Matrix  operator + (const Matrix&);		// Сложение матриц
	Matrix  operator - (const Matrix&);		// Вычитание матриц
	Matrix  operator * (const Matrix&);		// Умножение матриц

	friend istream& operator >> <> (istream&, Matrix&);			// Перегрузка оператора >> для ввода матрицы
	friend ostream& operator << <> (ostream&, const Matrix&);	// Перегрузка оператора << для вывода матрицы
};



template <typename Cell>
Matrix<Cell>::Matrix(const Matrix<Cell>& M)
{
	AllocateCells(M.rows, M.cols);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			cells[i][j] = M.cells[i][j];
}

template <typename Cell>
Matrix<Cell>::Matrix(int Size, Cell number = 0)
{
	AllocateCells(Size);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			i == j? cells[i][j] = number : cells[i][j] = 0;
		}
	}
	
		
}

template <typename Cell>
Matrix<Cell>::Matrix(int Size, Cell* list)
{
	AllocateCells(Size);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cells[i][j] = list[j + i * size];
		}
	}
}

template <typename Cell>
Matrix<Cell>::Matrix(int _rows, int _cols, Cell* list = nullptr)
{
	AllocateCells(_rows, _cols);
	if (list != nullptr) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				cells[i][j] = list[j + i * cols];
			}
		}
	}
	else {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				cells[i][j] = 0;
			}
		}
	}
}

template <typename Cell>
Matrix<Cell>::~Matrix()
{
	FreeCells();
}


template <typename Cell>
Cell Matrix<Cell>::get_cell(int i, int j)
{
	if (i < rows && j < cols) {
		return cells[i][j];
	}
	else {
		return -1;
	}
}

template<typename Cell>
void Matrix<Cell>::set_cell(int i, int j, Cell val)
{
	cells[i][j] = val;
}

template<typename Cell>
bool Matrix<Cell>::set_matrix(vector<Cell> vec, int rows, int cols)
{
	
	if (cols <= 0) {
		cols = rows;
	}
	AllocateCells(rows, cols);
	//Matrix<Cell> new_matrix(rows, cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cells[i][j] = vec[i * cols + j];
		}
	}

	return true;
}

template <typename Cell>
Matrix<Cell>& Matrix<Cell>::operator=(const Matrix& M)
{
	if ( rows != M.rows || cols != M.cols)
	{
		FreeCells();
		AllocateCells(M.rows, M.cols);
	}
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			cells[i][j] = M.cells[i][j];
	return *this;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator+(const Matrix& M)
{
	Matrix<Cell> res(*this);
	if (rows == M.rows && cols == M.cols)
	{
		for (int i=0; i<rows; i++)
		for (int j=0; j<cols; j++)
			res.cells[i][j] += M.cells[i][j];
	}
	return res;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator-(const Matrix& M)
{
	Matrix<Cell> res(*this);
	if (rows == M.rows && cols == M.cols)
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				res.cells[i][j] -= M.cells[i][j];
	}
	return res;
}

template <typename Cell>
Matrix<Cell> Matrix<Cell>::operator*(const Matrix& M)
{
	Matrix<Cell> res(*this);
	if (cols == M.rows)
	{
		Matrix<Cell> new_matrix(rows, M.cols, nullptr);
		for(int i = 0; i < rows; i++) {
			for (int j = 0; j < M.cols; j++) {
				for (int k = 0; k < cols; k++) {
					new_matrix.cells[i][j] += res.cells[i][k] * M.cells[k][j];
				}
			}
		}
		res = new_matrix;
	}
	return res;
}

template <typename Cell>
void Matrix<Cell>::AllocateCells(int Size)
{
	cells = new Cell*[Size];
	for (int i=0; i<Size; i++)
		cells[i] = new Cell[Size];
	size = Size;
	rows = Size;
	cols = Size;
}

template <typename Cell>
void Matrix<Cell>::AllocateCells(int _rows, int _cols)
{
	cells = new Cell*[_rows];
	for (int i = 0; i < _rows; i++)
		cells[i] = new Cell[_cols];
	size = -1;
	rows = _rows;
	cols = _cols;
}


template <typename Cell>
void Matrix<Cell>::FreeCells()
{
	for (int i = 0; i < rows; i++)
		delete[] cells[i];
	delete[] cells;
	size = 0;
	rows = 0;
	cols = 0;
}

template <typename Cell>
void Matrix<Cell>::Myltyply_column(int column, double multy)
{
	if (cols > column) {
		for (int i = 0; i < rows; i++) {
			cells[i][column] *= multy;
		}
	}
}

template <typename Cell>
void Matrix<Cell>::Myltyply_row(int row, double multy)
{
	if (rows > row) {
		for (int i = 0; i < cols; i++) {
			cells[row][i] *= multy;
		}
	}
}

template <typename Cell>
istream& operator >> (istream& fi, Matrix<Cell>& M)
{
	for (int i=0; i<M.rows; i++)
	for (int j=0; j<M.cols; j++)
		fi >> M.cells[i][j];
	return fi;
}

template <typename Cell>
ostream& operator << (ostream& fo, const Matrix<Cell>& M)
{
	for (int i=0; i<M.rows; i++)
	{
		fo << "|";
		for (int j=0; j<M.cols; j++)
			fo << setw(4) << M.cells[i][j] << "  ";
		fo << "|" << endl;
	}
	return fo;
}

#endif MATRIX_H
