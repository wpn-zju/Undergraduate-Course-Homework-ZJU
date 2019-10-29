#include "material.h"

namespace build
{
	Material::Material()
	{

	}

	Material::~Material()
	{

	}

	void Material::setAmbient(VECTOR4D color) const
	{
		GLfloat ambient[] = { color.x, color.y, color.z, color.w };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	}

	void Material::setDiffuse(VECTOR4D color) const
	{
		GLfloat diffuse[] = { color.x, color.y, color.z, color.w };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	}

	void Material::setSpecular(VECTOR4D color) const
	{
		GLfloat specular[] = { color.x, color.y, color.z, color.w };
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	}

	void Material::setShininess(GLfloat arg) const
	{
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, arg);
	}

	void Material::setEmission(VECTOR4D color) const
	{
		GLfloat emission[] = { color.x, color.y, color.z, color.w };
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
	}
}