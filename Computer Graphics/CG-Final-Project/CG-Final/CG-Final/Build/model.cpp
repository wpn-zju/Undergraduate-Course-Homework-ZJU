#include "model.h"

namespace build
{
	Model::Model(VECTOR3D center, VECTOR4D color)
		:m_Center(center), m_Color(color)
	{
		glPushMatrix();
	}

	Model::~Model()
	{

	}

	void Model::draw() const
	{
		glPopMatrix();
	}

	void Model::onTexture(GLuint texture)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	}

	void Model::offTexture()
	{
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_2D);
	}

	void Model::translate(VECTOR3D trans)
	{
		MATRIX4X4 m;
		m.SetTranslation(trans);
		VECTOR3D r = m * m_Center;
		m_Center.x = r.x;
		m_Center.y = r.y;
		m_Center.z = r.z;
	}

	void Model::scale(VECTOR3D scale)
	{
		glScalef(scale.x, scale.y, scale.z);
	}

	void Model::rotate(VECTOR4D rotate)
	{
		glRotatef(rotate.x, rotate.y, rotate.z, rotate.w);
	}
}