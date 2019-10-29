#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include "GL/glut.h"
#include "../Math/Maths.h"

class Obj3dmodel
{
private:
	std::vector <std::vector<GLfloat >> vSets;
	std::vector<std::vector<GLint>>fSets;
public:
	Obj3dmodel(std::string filename);
	void Draw(VECTOR3D position, VECTOR4D rotation, VECTOR3D scale, GLuint texture);
};