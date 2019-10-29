#include "texture.h"

namespace build
{
	Texture::Texture()
	{

	}

	Texture::~Texture()
	{

	}

	void Texture::load(const char *filename, GLuint* texture, int width, int height)
	{
		int pixellength;

		FILE* pfile = fopen(filename, "rb");
		if (pfile == 0) exit(0);

		fseek(pfile, 0x0012, SEEK_SET);
		fread(&m_Width, sizeof(m_Width), 1, pfile);
		fread(&m_Height, sizeof(m_Height), 1, pfile);

		pixellength = (m_Width) * 3;
		while (pixellength % 4 != 0)pixellength++;
		pixellength *= m_Height;

		m_Pixeldata = (GLubyte*)malloc(pixellength);
		if (m_Pixeldata == 0) exit(0);

		fseek(pfile, 54, SEEK_SET);
		fread(m_Pixeldata, pixellength, 1, pfile);

		if (width != 0 && height != 0)
		{
			pixellength = (width) * 3;
			while (pixellength % 4 != 0)pixellength++;
			pixellength *= height;

			GLubyte* pixeldata = (GLubyte*)malloc(pixellength);
			if (pixellength == 0) exit(0);

			gluScaleImage(GL_RGB, m_Width, m_Height, GL_UNSIGNED_BYTE, m_Pixeldata, width, height, GL_UNSIGNED_BYTE, pixeldata);
			m_Pixeldata = pixeldata;
			m_Width = width;
			m_Height = height;
		}

		fclose(pfile);
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_Pixeldata);
		return;
	}

	void Texture::setWrapS(GLuint mode)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
	}

	void Texture::setWrapT(GLuint mode)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
	}

	void Texture::setMagFilter(GLuint mode)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
	}

	void Texture::setMinFilter(GLuint mode)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
	}
}