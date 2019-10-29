#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine()
{
	velocity = VECTOR3D(0.f, 0.f, 0.f);
	gravity = VECTOR3D(0.f, GravityAcceler, 0.f);
	accelerUp = VECTOR3D(0.f, 0.f, 0.f);

	isJumping = false;
}

PhysicsEngine::~PhysicsEngine()
{

}

void PhysicsEngine::setSceneOuterBoundary(float x1, float z1, float x2, float z2)
{
	outerBoundary = VECTOR4D(x1, z1, x2, z2);
}

void PhysicsEngine::setSceneInnerBoundary(float x1, float y1, float z1, float x2, float y2, float z2)
{
	VECTOR3D key(x1 - BoundaryGap, y1 - BoundaryGap, z1 - BoundaryGap);
	VECTOR3D value(x2 + BoundaryGap, y2 + BoundaryGap, z2 + BoundaryGap);

	innerBoundaryMin.push_back(key);
	innerBoundaryMax.push_back(value);
}

void PhysicsEngine::outCollisionTest(VECTOR3D & cameraPos, VECTOR3D & targetPos)
{
	outCollisionTestXZ(outerBoundary.x, outerBoundary.y, outerBoundary.z, outerBoundary.w, cameraPos, targetPos);
}

void PhysicsEngine::outCollisionTestXZ(float x1, float z1, float x2, float z2, VECTOR3D & cameraPos, VECTOR3D & targetPos)
{
	if (x1 < 0)
		x1 += 1;
	else x1 -= 1;

	if (x2 < 0)
		x2 += 1;
	else x2 -= 1;

	if (z1 < 0)
		z1 += 1;
	else z1 -= 1;

	if (z2 < 0)
		z2 += 1;
	else z2 -= 1;

	if (cameraPos.x < x1)
		cameraPos.x = x1;
	if (cameraPos.x > x2)
		cameraPos.x = x2;
	if (cameraPos.z < z1)
		cameraPos.z = z1;
	if (cameraPos.z > z2)
		cameraPos.z = z2;
}

void PhysicsEngine::jumpAndUpdateVelocity()
{
	velocity += VECTOR3D(0.f, JumpInitialSpeed, 0.f);
	accelerUp.y = 0.f;
}

// 判断在xz平面，相机位置是否位于碰撞体内部
bool insideTheCollider(VECTOR3D _cameraPos, VECTOR3D _innerMin, VECTOR3D _innerMax)
{
	float camX = _cameraPos.x;
	float camZ = _cameraPos.z;
	float minX = _innerMin.x;
	float minZ = _innerMin.z;
	float maxX = _innerMax.x;
	float maxZ = _innerMax.z;

	if (minX <= camX && camX <= maxX && minZ <= camZ && camZ <= maxZ)
		return true;
	else
		return false;
}

void PhysicsEngine::updateCameraVertMovement(VECTOR3D & cameraPos, VECTOR3D & targetPos)
{
	VECTOR3D acceleration = gravity + accelerUp;
	velocity.x += acceleration.x * GravityFactor;
	velocity.y += acceleration.y * GravityFactor;
	velocity.z += acceleration.z * GravityFactor;
	cameraPos.x += velocity.x * JumpFactor;
	cameraPos.y += velocity.y * JumpFactor;
	cameraPos.z += velocity.z * JumpFactor;
	targetPos.x += velocity.x * JumpFactor;
	targetPos.y += velocity.y * JumpFactor;
	targetPos.z += velocity.z * JumpFactor;

	// 检测所有碰撞体
	for (int i = 0; i < innerBoundaryMin.size(); i++)
	{
		// 如果在XZ平面进入碰撞体所在区域
		if (insideTheCollider(cameraPos, innerBoundaryMin[i], innerBoundaryMax[i]))
		{
			if (cameraPos.y - HeroHeight <= innerBoundaryMax[i].y&& cameraPos.y >= innerBoundaryMax[i].y)
			{
				// 脚接触到碰撞体顶部
				isJumping = false;
				accelerUp.y = -GravityAcceler;
				velocity.y = 0.f;
				cameraPos.y = innerBoundaryMax[i].y + HeroHeight;
				break;
			}

			if (cameraPos.y >= innerBoundaryMin[i].y &&cameraPos.y - HeroHeight <= innerBoundaryMin[i].y)
			{
				// 头接触到碰撞体底部
				velocity.y = 0.f;
				cameraPos.y = innerBoundaryMin[i].y;
				break;
			}
		}
		else
			accelerUp.y = 0.f;	// 重力
	}
}

void PhysicsEngine::inCollisionTest(VECTOR3D & cameraPos, VECTOR3D & targetPos)
{
	// 后面可以在这里添加：预处理，排除当前肯定不会产生碰撞的物体
	for (int i = 0; i < innerBoundaryMin.size(); i++)
		inCollisionTestWithHeight(innerBoundaryMin[i].x, innerBoundaryMin[i].y, innerBoundaryMin[i].z,
			innerBoundaryMax[i].x, innerBoundaryMax[i].y, innerBoundaryMax[i].z, cameraPos, targetPos);
}

void PhysicsEngine::inCollisionTestWithHeight(float x1, float y1, float z1, float x2, float y2, float z2, VECTOR3D & cameraPos, VECTOR3D & targetPos)
{
	// 当身体处于碰撞体垂直区域范围内，才进行XZ平面的碰撞检测
	if (cameraPos.y - HeroHeight > y1 && cameraPos.y - HeroHeight < y2)
		inCollisionTestXZ(x1, z1, x2, z2, cameraPos, targetPos);
}
double Direction(dot pi, dot pj, dot pk)
{
	return (pk.x - pi.x) * (pj.y - pi.y) - (pj.x - pi.x) * (pk.y - pi.y);
}

bool OnSegment(dot pi, dot pj, dot pk)
{
	if ((min(pi.x, pj.x) <= pk.x) && (pk.x <= max(pi.x, pj.x))
		&& (min(pi.y, pj.y) <= pk.y) && (pk.y <= max(pi.y, pj.y)))
		return true;
	else return false;
}

// 检测线段相交快速算法
bool SegmentIntersect(dot p1, dot p2, dot p3, dot p4)
{
	int d1, d2, d3, d4;
	d1 = Direction(p3, p4, p1);
	d2 = Direction(p3, p4, p2);
	d3 = Direction(p1, p2, p3);
	d4 = Direction(p1, p2, p4);
	if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2>0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4>0)))
		return true;
	else if (d1 == 0 && OnSegment(p3, p4, p1))
		return true;
	else if (d2 == 0 && OnSegment(p3, p4, p2))
		return true;
	else if (d3 == 0 && OnSegment(p1, p2, p3))
		return true;
	else if (d4 == 0 && OnSegment(p1, p2, p4))
		return true;
	else
		return false;
}

void PhysicsEngine::inCollisionTestXZ(float x1, float z1, float x2, float z2, VECTOR3D & cameraPos, VECTOR3D & targetPos)
{
	if (cameraPos.z > z1 && cameraPos.z < z1 + 0.3f && cameraPos.x > x1 && cameraPos.x < x2)
		cameraPos.z = z1 - 0.3f;
	else if (cameraPos.z < z2 && cameraPos.z > z2 - 0.3f && cameraPos.x > x1 && cameraPos.x < x2)
		cameraPos.z = z2 + 0.3f;
	else if (cameraPos.x > x1 && cameraPos.x < x1 + 0.3f && cameraPos.z > z1 && cameraPos.z < z2)
		cameraPos.x = x1 - 0.3f;
	else if (cameraPos.x < x2 && cameraPos.x > x2 - 0.3f && cameraPos.z > z1 && cameraPos.z < z2)
		cameraPos.x = x1 + 0.3f;
}