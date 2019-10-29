#pragma once

#include "GL/glut.h"
#include "../Math/Maths.h"

namespace build
{
	inline VECTOR2D getCylinPoint(GLfloat s)
	{
		GLfloat x = cos(2 * M_PI*s);
		GLfloat y = sin(2 * M_PI*s);
		return VECTOR2D(x, y);
	}

	inline void drawCycle(VECTOR3D pos)
	{
		GLuint num = 30;
		GLfloat step = 1 / (GLfloat)num;
		GLfloat s = 0;
		for (int i = 0; i < num; i++)
		{
			glBegin(GL_TRIANGLES);
			glVertex3f(pos.x, pos.y, pos.z);
			VECTOR2D p1 = getCylinPoint(s);
			glVertex3f(pos.x + p1.x, pos.y + p1.y, pos.z);
			VECTOR2D p2 = getCylinPoint(s - step);
			glVertex3f(pos.x + p2.x, pos.y + p2.y, pos.z);
			s -= step;
			glEnd();
		}
	}

	class Model
	{
	public:
		VECTOR3D m_Center;
		VECTOR4D m_Color;
		Model(VECTOR3D center, VECTOR4D color);
		~Model();

		virtual void draw() const;
		virtual void onTexture(GLuint texture);
		virtual void offTexture();
		virtual void translate(VECTOR3D trans);
		virtual void scale(VECTOR3D scale);
		virtual void rotate(VECTOR4D rotate);
	};
}