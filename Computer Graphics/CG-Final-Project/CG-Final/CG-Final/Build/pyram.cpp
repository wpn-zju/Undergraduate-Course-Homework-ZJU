#include "pyram.h"

namespace build
{
	Pyram::Pyram(GLfloat r, GLfloat h, VECTOR3D center, VECTOR4D color)
		: Model::Model(center, color), m_R(r), m_Height(h)
	{

	}

	Pyram::~Pyram()
	{

	}

	void Pyram::draw() const
	{
		glColor4f(m_Color.x, m_Color.y, m_Color.z, m_Color.w);
		glTranslatef(m_Center.x, m_Center.y, m_Center.z);
		glScalef(m_R, m_R, m_Height);
		drawCycle(VECTOR3D(0, 0, 0));
		glutSolidCone(1, 1, 30, 30);

		Model::draw();
	}
}