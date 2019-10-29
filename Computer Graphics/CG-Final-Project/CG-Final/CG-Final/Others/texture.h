#pragma once

#include <cstdio>
#include "GL/glut.h"
#include "../Math/Maths.h"

namespace build
{
	class Texture
	{
	private:
		int m_Width;
		int m_Height;
		GLubyte* m_Pixeldata;
	public:
		Texture();
		~Texture();

		void load(const char* filename, GLuint* texture, int width, int height);
		void setWrapS(GLuint mode);
		void setWrapT(GLuint mode);
		void setMagFilter(GLuint mode);
		void setMinFilter(GLuint mode);
	};
}