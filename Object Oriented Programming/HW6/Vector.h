#ifndef _VECTOR_H
#define _VECTOR_H

#include <cstring>

class IndexOutofBounds {
public:
	IndexOutofBounds() {
		std::cout << "!!! Index Out of Bounds !!!" << std::endl;
	}
};

template <class T>
class Vector {
public:
	Vector() {
		m_nSize = 0;
		m_pElements = NULL;
	} // create an empty vector

	Vector(int size) {
		m_nSize = size;
		m_pElements = new T[size];
	} // create a vector of size elements

	Vector(const Vector<T>& that) 
	{
		m_nSize = that.m_nSize;
		m_pElements = new T[that.m_nSize];
		std::memcpy(m_pElements, that.m_pElements, m_nSize * sizeof(T));
	} // copy ctor

	~Vector() {
		delete[] m_pElements;
	} // destructor

	T& operator[](int index) {
		if (index >= 0 && index < m_nSize)
		{
			return m_pElements[index];
		}
		else
		{
			throw IndexOutofBounds();
		}
	} // return the specified element, throws exception when index < 0 or >= m_nSize
	
	int size() {
		return m_nSize;
	} // return the size of the vector

	int inflate(int addSize) {
		int oldSize = m_nSize;
		T* temp = m_pElements;
		m_nSize += addSize;
		m_pElements = new T[m_nSize];
		std::memcpy(m_pElements, temp, oldSize * sizeof(T));
		delete[] temp;
		return m_nSize;
	} //expand the storage to m_nSize + addSize, return the new size

	Vector<T>& operator = (const Vector<T>& that) {
		delete[] m_pElements;
		m_nSize = that.m_nSize;
		m_pElements = new T[that.m_nSize];
		std::memcpy(m_pElements, that.m_pElements, m_nSize * sizeof(T));
		return *this;
	}
private:
	T* m_pElements;
	int m_nSize;
};
#endif
