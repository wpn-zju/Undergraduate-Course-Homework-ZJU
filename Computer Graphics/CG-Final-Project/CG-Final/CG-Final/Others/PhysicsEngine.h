#pragma once

#include <iostream>
#include <vector>
#include "../Math/Maths.h"

#define min(x,y) ((x) < (y) ? (x) : (y))
#define max(x,y) ((x) < (y) ? (y) : (x))
#define HeroHeight 1.0f				// 玩家视点到脚的高度
#define GravityAcceler -9.8f		// 重力加速度
#define MoveSpeed 15.0f				// 玩家移动速度
#define BoundaryGap 0.05f			// 碰撞间距
#define JumpInitialSpeed 8.0f		// 起跳初速度
#define JumpFactor 0.04f			// 跳起速度系数
#define GravityFactor 0.04f			// 下落速度系数

struct dot
{
	float x;
	float y;
	dot(float _x, float _y) :x(_x), y(_y) { }
};

class PhysicsEngine
{
public:
	PhysicsEngine();
	~PhysicsEngine();

	// 设置空间外部边缘
	void setSceneOuterBoundary(float x1, float z1, float x2, float z2);
	// 外部碰撞检测
	void outCollisionTest(VECTOR3D & cameraPos, VECTOR3D & targetPos);
	// 设置空间内部边缘
	void setSceneInnerBoundary(float x1, float y1, float z1, float x2, float y2, float z2);
	// 内部碰撞检测
	void inCollisionTest(VECTOR3D & cameraPos, VECTOR3D & targetPos);

	bool isJumping;
	// 按下space跳跃时调用
	void jumpAndUpdateVelocity();
	// 每帧绘制的时候更新摄像机垂直方向移动
	void updateCameraVertMovement(VECTOR3D & cameraPos, VECTOR3D & targetPos);

private:
	// 空间内部边缘碰撞检测（考虑高度）
	void inCollisionTestWithHeight(float x1, float y1, float z1, float x2, float y2, float z2, VECTOR3D & cameraPos, VECTOR3D & targetPos);
	// 空间内部边缘碰撞检测（不考虑高度，即XZ平面）
	void inCollisionTestXZ(float x1, float z1, float x2, float z2, VECTOR3D & cameraPos, VECTOR3D & targetPos);
	// 空间外部边缘碰撞检测
	void outCollisionTestXZ(float x1, float z1, float x2, float z2, VECTOR3D & cameraPos, VECTOR3D & targetPos);

	VECTOR3D velocity;						//垂直方向速度
	VECTOR3D gravity;						//重力加速度
	VECTOR3D accelerUp;						//方向向上的加速度

	VECTOR4D outerBoundary;
	std::vector<VECTOR3D> innerBoundaryMin;		//碰撞器小的x/y/z坐标
	std::vector<VECTOR3D> innerBoundaryMax;		//碰撞器大的x/y/z坐标
};