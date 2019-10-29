#include "cube.h"

namespace build
{
	Cube::Cube(GLfloat length, GLfloat width, GLfloat height, VECTOR3D center, VECTOR4D color)
		: Model::Model(center, color), m_Length(length), m_Width(width), m_Height(height)
	{

	}

	Cube::~Cube()
	{

	}

	void Cube::draw() const
	{
		glColor4f(m_Color.x, m_Color.y, m_Color.z, m_Color.w);
		glTranslatef(m_Center.x, m_Center.y, m_Center.z);
		glScalef(m_Length, m_Width, m_Height);
		glutSolidCube(1.0f);

		Model::draw();
	}
}