#pragma once

#include "model.h"

namespace build
{
	class Cube : public Model
	{
	public:
		GLfloat m_Length;
		GLfloat m_Width;
		GLfloat m_Height;

		Cube(GLfloat length, GLfloat width, GLfloat height, VECTOR3D center = VECTOR3D(0, 0, 0), VECTOR4D color = VECTOR4D(1, 1, 1, 1));
		~Cube();

		void draw() const override;
	};
}