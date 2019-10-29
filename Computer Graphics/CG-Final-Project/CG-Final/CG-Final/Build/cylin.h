#pragma once

#include "model.h"

namespace build
{
	class Cylin : public Model
	{
	private:
		GLfloat m_R;
		GLfloat m_Height;
	public:
		Cylin(GLfloat r, GLfloat h, VECTOR3D center = VECTOR3D(0, 0, 0), VECTOR4D color = VECTOR4D(1, 1, 1, 1));
		~Cylin();

		void draw() const override;
	};
}