#include "ball.h"

namespace build
{
	Ball::Ball(GLfloat r, VECTOR3D center, VECTOR4D color)
		: Model::Model(center, color), m_R(r)
	{

	}

	Ball::~Ball()
	{

	}

	void Ball::draw() const
	{
		glColor4f(m_Color.x, m_Color.y, m_Color.z, m_Color.w);
		glTranslatef(m_Center.x, m_Center.y, m_Center.z);
		glutSolidSphere(m_R, 30, 30);

		Model::draw();
	}
}