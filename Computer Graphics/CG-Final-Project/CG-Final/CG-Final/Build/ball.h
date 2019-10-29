#pragma once

#include "model.h"

namespace build
{
	class Ball : public Model
	{
	private:
		GLfloat m_R;
	public:
		Ball(GLfloat r, VECTOR3D center = VECTOR3D(0, 0, 0), VECTOR4D color = VECTOR4D(1, 1, 1, 1));
		~Ball();

		void draw() const override;
	};
}