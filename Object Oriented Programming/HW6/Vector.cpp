#include <iostream>
#include "Vector.h"
using namespace std;

template <class T>
Vector<T> :: Vector()
{
	m_nSize = 0; 
	m_pElements = NULL;
}

template <class T>
Vector<T> :: Vector(int size)
{
	m_nSize = size;
	m_pElements = new T[size];
}

template <class T>
Vector<T> :: Vector(const Vector<T>& that)
{
	m_nSize = that.m_nSize;
	m_pElements = new T[that.m_nSize];
	memcpy(m_pElements, that.m_pElements, m_nSize * sizeof(T));
}

template <class T>
Vector<T> :: ~Vector()
{
	delete[] m_pElements;
}

template <class T>
T& Vector<T> :: operator[](int index) throw(IndexOutofBounds)
{
	if(index >= 0 && index < m_nSize)
	{
		return m_pElements[index];
	}
	else
	{
		throw(IndexOutofBounds());
	}
}

template <class T>
int Vector<T> :: size()
{
	return m_nSize;
}

template <class T>
int Vector<T> :: inflate(int addSize)
{
	int oldSize = m_nSize;
	T* temp = m_pElements;
	m_nSize += addSize;
	m_pElements = new T[m_nSize];
	memcpy(m_pElements, temp, oldSize * sizeof(T)) ;
	delete[] temp;
	return m_nSize; 
}

template <class T>
Vector<T>& Vector<T> :: operator = (const Vector<T>& that)
{
	delete[] m_pElements;
	m_nSize = that.m_nSize;
	m_pElements = new T[that.m_nSize];
	memcpy(m_pElements, that.m_pElements, m_nSize * sizeof(T));
	return *this;
}
