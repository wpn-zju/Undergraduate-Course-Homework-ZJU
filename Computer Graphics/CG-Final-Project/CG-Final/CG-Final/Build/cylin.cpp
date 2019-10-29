#include "cylin.h"

namespace build
{
	Cylin::Cylin(GLfloat r, GLfloat h, VECTOR3D center, VECTOR4D color)
		: Model::Model(center, color), m_R(r), m_Height(h)
	{

	}

	Cylin::~Cylin()
	{

	}

	void Cylin::draw() const
	{
		glColor4f(m_Color.x, m_Color.y, m_Color.z, m_Color.w);
		GLUquadricObj *objCylinder = gluNewQuadric();
		glTranslatef(m_Center.x, m_Center.y, m_Center.z);
		glScalef(m_R, m_R, m_Height);
		drawCycle(VECTOR3D(0, 0, 1));
		drawCycle(VECTOR3D(0, 0, 0));
		gluCylinder(objCylinder, 1, 1, 1, 30, 30);

		Model::draw();
	}
}