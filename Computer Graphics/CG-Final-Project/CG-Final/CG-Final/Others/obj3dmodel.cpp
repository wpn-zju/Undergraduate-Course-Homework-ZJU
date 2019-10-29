#include "obj3dmodel.h"

Obj3dmodel::Obj3dmodel(std::string filename)
{
	std::string line;
	std::fstream f;
	f.open(filename, std::ios::in);

	if (!f.is_open())
		std::cout << "Something Went Wrong When Opening Objfiles" << std::endl;

	while (!f.eof())
	{
		getline(f, line);
		std::vector<std::string> parameters;
		std::string tailMark = " ";
		std::string ans = "";

		line = line.append(tailMark);
		for (int i = 0; i < line.length(); i++)
		{
			char ch = line[i];
			if (ch != ' ')
				ans += ch;
			else
			{
				parameters.push_back(ans);
				ans = "";
			}
		}
		if (parameters[0] == "v")
		{
			std::vector<GLfloat>Point;
			for (int i = 1; i < 4; i++)
			{
				GLfloat xyz = atof(parameters[i].c_str());
				Point.push_back(xyz);
			}
			vSets.push_back(Point);
		}

		else if (parameters[0] == "f")
		{
			std::vector<GLint>vIndexSets;
			for (int i = 1; i < parameters.size(); i++)
			{
				std::string x = parameters[i];
				std::string ans = "";
				for (int j = 0; j < x.length(); j++)
				{
					char ch = x[j];
					if (ch != '/')
						ans += ch;
					else
						break;
				}
				GLint index = atof(ans.c_str());
				index--;
				vIndexSets.push_back(index);
			}
			fSets.push_back(vIndexSets);
		}
	}
	f.close();
}

void Obj3dmodel::Draw(VECTOR3D position, VECTOR4D rotation, VECTOR3D scale, GLuint texture)
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_PLANE);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_PLANE);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glTranslatef(position.x, position.y, position.z);
	glScalef(scale.x, scale.y, scale.z);
	glRotatef(rotation.x, rotation.y, rotation.z, rotation.w);
	glRotatef(90, 0, 1, 0);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < fSets.size(); i++)
	{
		GLfloat VN[3];

		GLfloat SV1[3];
		GLfloat SV2[3];
		GLfloat SV3[3];

		if ((fSets[i]).size() == 3)
		{
			GLint firstVertexIndex = (fSets[i])[0];
			GLint secondVertexIndex = (fSets[i])[1];
			GLint thirdVertexIndex = (fSets[i])[2];

			SV1[0] = (vSets[firstVertexIndex])[0];
			SV1[1] = (vSets[firstVertexIndex])[1];
			SV1[2] = (vSets[firstVertexIndex])[2];

			SV2[0] = (vSets[secondVertexIndex])[0];
			SV2[1] = (vSets[secondVertexIndex])[1];
			SV2[2] = (vSets[secondVertexIndex])[2];

			SV3[0] = (vSets[thirdVertexIndex])[0];
			SV3[1] = (vSets[thirdVertexIndex])[1];
			SV3[2] = (vSets[thirdVertexIndex])[2];

			GLfloat vec1[3], vec2[3], vec3[3];

			vec1[0] = SV1[0] - SV2[0];
			vec1[1] = SV1[1] - SV2[1];
			vec1[2] = SV1[2] - SV2[2];

			vec2[0] = SV1[0] - SV3[0];
			vec2[1] = SV1[1] - SV3[1];
			vec2[2] = SV1[2] - SV3[2];

			vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
			vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
			vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

			GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));

			VN[0] = vec3[0] / D;
			VN[1] = vec3[1] / D;
			VN[2] = vec3[2] / D;

			glNormal3f(VN[0], VN[1], VN[2]);

			glVertex3f(SV1[0], SV1[1], SV1[2]);
			glVertex3f(SV2[0], SV2[1], SV2[2]);
			glVertex3f(SV3[0], SV3[1], SV3[2]);
		}
	}
	glEnd();
	glBegin(GL_QUADS);
	for (int i = 0; i < fSets.size(); i++)
	{
		GLfloat VN[3];

		GLfloat SV1[3];
		GLfloat SV2[3];
		GLfloat SV3[3];
		GLfloat SV4[3];

		if ((fSets[i]).size() == 4)
		{
			GLint firstVertexIndex = (fSets[i])[0];
			GLint secondVertexIndex = (fSets[i])[1];
			GLint thirdVertexIndex = (fSets[i])[2];
			GLint forthVertexIndex = (fSets[i])[3];

			SV1[0] = (vSets[firstVertexIndex])[0];
			SV1[1] = (vSets[firstVertexIndex])[1];
			SV1[2] = (vSets[firstVertexIndex])[2];

			SV2[0] = (vSets[secondVertexIndex])[0];
			SV2[1] = (vSets[secondVertexIndex])[1];
			SV2[2] = (vSets[secondVertexIndex])[2];

			SV3[0] = (vSets[thirdVertexIndex])[0];
			SV3[1] = (vSets[thirdVertexIndex])[1];
			SV3[2] = (vSets[thirdVertexIndex])[2];

			SV4[0] = (vSets[forthVertexIndex])[0];
			SV4[1] = (vSets[forthVertexIndex])[1];
			SV4[2] = (vSets[forthVertexIndex])[2];

			GLfloat vec1[3], vec2[3], vec3[3];

			vec1[0] = SV1[0] - SV2[0];
			vec1[1] = SV1[1] - SV2[1];
			vec1[2] = SV1[2] - SV2[2];

			vec2[0] = SV1[0] - SV3[0];
			vec2[1] = SV1[1] - SV3[1];
			vec2[2] = SV1[2] - SV3[2];

			vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
			vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
			vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

			GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));

			VN[0] = vec3[0] / D;
			VN[1] = vec3[1] / D;
			VN[2] = vec3[2] / D;

			glNormal3f(VN[0], VN[1], VN[2]);

			glVertex3f(SV1[0], SV1[1], SV1[2]);
			glVertex3f(SV2[0], SV2[1], SV2[2]);
			glVertex3f(SV3[0], SV3[1], SV3[2]);
			glVertex3f(SV4[0], SV4[1], SV4[2]);
		}
	}
	glEnd();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}