#pragma once

#include "model.h"

namespace build
{
	class Pyram : public Model
	{
	private:
		GLfloat m_R;
		GLfloat m_Height;
	public:
		Pyram(GLfloat r, GLfloat h, VECTOR3D center = VECTOR3D(0, 0, 0), VECTOR4D color = VECTOR4D(1, 1, 1, 1));
		~Pyram();

		void draw() const override;
	};
}