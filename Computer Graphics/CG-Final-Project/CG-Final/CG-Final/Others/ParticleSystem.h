#pragma once

#include <vector>
#include "GL/glut.h"
#include "../Math/Maths.h"

struct Particle
{
	VECTOR3D position;
	VECTOR3D veclocity;
	VECTOR3D acceleration;
	float color[3];
	float age;
	float life;
	float size;
	bool died;
};

class ParticleSystem
{
public:
	ParticleSystem() {}
	ParticleSystem(int _count, float _gravity) { ptlCount = _count; gravity = _gravity; }
	void init();
	void simulate(float dt);
	void aging(float dt);
	void applyGravity();
	void kinematics(float dt);
	void render();
	~ParticleSystem() {}

private:
	int ptlCount;
	float gravity;
	GLUquadricObj* sphere;
	std::vector<Particle> particles;
};