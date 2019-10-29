#ifndef _VECTOR_H
#define _VECTOR_H
#include <iostream>

class IndexOutofBounds
{
	public:
		IndexOutofBounds()
		{
			std::cout << "!!! Index Out of Bounds !!!" << std::endl;
		}
};

template <class T>
class Vector
{
	public:
		Vector(); // create an empty vector
		Vector(int size); // create a vector of size elements
		Vector(const Vector<T>& that); // copy ctor
		~Vector(); // destructor
		T& operator[](int index) throw(IndexOutofBounds); // return the specified element, throws exception when index < 0 or >= m_nSize
		int size(); // return the size of the vector
		int inflate(int addSize); //expand the storage to m_nSize + addSize, return the new size
		Vector<T>& operator = (const Vector<T>& that);
	private:
		T* m_pElements;
		int m_nSize;
};
#endif
