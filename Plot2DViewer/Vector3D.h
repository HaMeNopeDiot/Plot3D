#ifndef VECTOR_3D_H
#define VECTOR_3D_H

#include <vector>

template <typename Cell = double>
class Vector3D
{
	std::vector<Cell> elements;
public:
	//Vector3D();
	Vector3D(int size = 3, Cell value = 0.0);
	
	std::vector<double> get_vector() { return elements; }
	int  get_size() { return elements.size(); }
	Cell get_elem(int pos) { return pos < elements.size() && pos > -1 ? elements[pos]: 0; }

	bool set_elem(int pos, Cell value);
	bool set_vector(std::vector<Cell> vec);

	Vector3D vector_multyply(Vector3D& B);
	double    scalar_multyply(Vector3D& B);

	double get_norm_of_vec() { return sqrt(scalar_multyply(*this)); }


	Vector3D& operator/(double value)
	{
		int size = elements.size();
		for (int i = 0; i < size; i++) {
			elements[i] = elements[i] * 1.0f / value;
		}
		return *this;
	}

	Vector3D& operator*(double value)
	{
		int size = elements.size();
		for (int i = 0; i < size; i++) {
			elements[i] *= value;
		}
		return *this;
	}

	double& operator[](int ind)
	{
		return elements[ind];
	}

};

//template<typename Cell>
//inline Vector3D<Cell>::Vector3D()
//{
//	std::vector<Cell> new_vector(3, 0);
//	elements = new_vector;
//}

template <typename Cell>
Vector3D<Cell>::Vector3D(int size, Cell value = 0.0)
{
	std::vector<Cell> new_vector(size, value);
	elements = new_vector;
}




template<typename Cell>
inline bool Vector3D<Cell>::set_elem(int pos, Cell value)
{
	if (pos > -1 && pos < elements.size()) {
		elements[i] = value;
		return true;
	} else {  
		return false;
	}
}

template<typename Cell>
inline bool Vector3D<Cell>::set_vector(std::vector<Cell> vec)
{
	if (vec.size() == elements.size()) {
		for (int i = 0; i < elements.size(); i++) {
			elements[i] = vec[i];
		}
		return true;
	} else {
		return false;
	}
}

template<typename Cell>
inline Vector3D<Cell> Vector3D<Cell>::vector_multyply(Vector3D& B)
{
	int size = elements.size();
	Vector3D result(size);
	for (int i = 0; i < size; i++) {
		result[i] = elements[(i + 1) % size] * B.get_elem((i + 2) % size) - B.get_elem((i + 1) % size) * elements[(i + 2) % size];
	}
	return result;
}

template<typename Cell>
inline double Vector3D<Cell>::scalar_multyply(Vector3D& B)
{
	int size = elements.size();
	double result = 0;
	for (int i = 0; i < size; i++) {
		result += elements[i] * B.get_elem(i);
	}
	return result;
}
#endif VECTOR_3D_H