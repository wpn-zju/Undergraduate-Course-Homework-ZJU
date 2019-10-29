#include <cstdio>
#include <vector>
#include "../GLEE/GLee.h"
#include "GL/glut.h"
#include "Math/Maths.h"
#include "main.h"
#include "Build/ball.h"
#include "Build/cube.h"
#include "Build/cylin.h"
#include "Build/pyram.h"
#include "Others/material.h"
#include "Others/texture.h"
#include "Others/obj3dmodel.h"
#include "Others/ParticleSystem.h"
#include "Others/PhysicsEngine.h"

#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
#define SKYBOX_SIZE 600.0f
#define SENSITIVITY 0.2f
#define BMP_Header_Length 54

//Camera & light positions
VECTOR3D cameraPosition(-4.0f, 1.1f, 0.0f);
VECTOR3D cameraTarget(0.0f, 0.0f, 0.0f);
VECTOR3D lightPosition(-5.0f, 5.0f, -5.0f);

//Size of shadow map
const int shadowMapSize = 512;

//Textures
GLuint ground;
GLuint door;
GLuint wall;
GLuint sUp;
GLuint sDown;
GLuint sLeft;
GLuint sRight;
GLuint sFront;
GLuint sBack;
GLuint shadowMapTexture;

// .obj Model
Obj3dmodel desk("CG-Final\\Image\\computerdesk.obj");

GLUnurbsObj *nurbs;

// Physics
PhysicsEngine* physicsEngine;

// Particle System
ParticleSystem particleSystem(15, GravityAcceler / 5.0f);

// Control
int selectId = 0;
bool wPressed = false;
bool sPressed = false;
bool aPressed = false;
bool dPressed = false;
GLfloat roll = 0, pitch = 0, yaw = 0;
GLfloat cameraScale = 1.0f;
std::vector<VECTOR3D> boxPosition;
std::vector<VECTOR3D> boxScale;
static int boxNum = 0;
int count1 = 0;
int count2 = 0;
int count3 = 0;
int count4 = 0;
GLfloat lightPower = 1.0f;
int windowWidth, windowHeight;

// Matrices
MATRIX4X4 lightProjectionMatrix, lightViewMatrix;
MATRIX4X4 cameraProjectionMatrix, cameraViewMatrix;

void InitSingleBoxCollider(VECTOR3D pos, VECTOR3D scalar) 
{
	boxPosition.push_back(pos);
	boxScale.push_back(scalar);
	boxNum++;
}

void InitBoxCollidersProperty() {
	InitSingleBoxCollider(VECTOR3D(14, 1, 9), VECTOR3D(12, 2, 2));
	InitSingleBoxCollider(VECTOR3D(19, 3, 4), VECTOR3D(2, 2, 8));
	InitSingleBoxCollider(VECTOR3D(19, 5, -5), VECTOR3D(2, 2, 10));
	InitSingleBoxCollider(VECTOR3D(11, 3, -9), VECTOR3D(14, 2, 2));
	InitSingleBoxCollider(VECTOR3D(-2, 5, -9), VECTOR3D(12, 2, 2));
	InitSingleBoxCollider(VECTOR3D(-14, 3, -9), VECTOR3D(12, 2, 2));
	InitSingleBoxCollider(VECTOR3D(-19, 5, -5), VECTOR3D(2, 2, 10));
	InitSingleBoxCollider(VECTOR3D(-19, 7, 4), VECTOR3D(2, 2, 8));
	InitSingleBoxCollider(VECTOR3D(-12, 5, 9), VECTOR3D(12, 2, 2));
	InitSingleBoxCollider(VECTOR3D(1, 3, 9), VECTOR3D(14, 2, 2));
	InitSingleBoxCollider(VECTOR3D(0, 4, -10.3f), VECTOR3D(40, 8, 0.6f));
	InitSingleBoxCollider(VECTOR3D(5, 7, 10.3f), VECTOR3D(30, 2, 0.6f));
	InitSingleBoxCollider(VECTOR3D(0, 3, 10.3f), VECTOR3D(40, 6, 0.6f));
	InitSingleBoxCollider(VECTOR3D(-16, 7, 10.3f), VECTOR3D(8, 2, 0.6f));
	InitSingleBoxCollider(VECTOR3D(20.3f, 4, 0), VECTOR3D(0.6f, 8, 20));
	InitSingleBoxCollider(VECTOR3D(-20.3f, 4, 0), VECTOR3D(0.6f, 8, 20));
}

void SetBoxColliderBoundary() 
{
	for (int i = 0; i < boxNum; ++i) 
	{
		physicsEngine->setSceneInnerBoundary
		(
			boxPosition[i].x - boxScale[i].x / 2.f,
			boxPosition[i].y - boxScale[i].y / 2.f,
			boxPosition[i].z - boxScale[i].z / 2.f,
			boxPosition[i].x + boxScale[i].x / 2.f,
			boxPosition[i].y + boxScale[i].y / 2.f,
			boxPosition[i].z + boxScale[i].z / 2.f
		);
	}
}

// Called for initiation
bool Init(void)
{
	// Check for necessary extensions
	if (!GLEE_ARB_depth_texture || !GLEE_ARB_shadow)
	{
		printf("I require ARB_depth_texture and ARB_shadow extensionsn\n");
		return false;
	}

	physicsEngine = new PhysicsEngine();

	particleSystem.init();

	// Load identity modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Shading states
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Depth states
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	// We use glScale when drawing the scene
	glEnable(GL_NORMALIZE);

	// Create the shadow map texture
	glGenTextures(1, &shadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize, 0,
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Use the color as the ambient and diffuse material
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	// White specular material color, shininess 16
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);

	// Calculate & save matrices
	glPushMatrix();

	glLoadIdentity();
	gluPerspective(45.0f, (float)windowWidth / windowHeight, 0.1f, 1000.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);

	glLoadIdentity();
	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
		cameraTarget.x, cameraTarget.y, cameraTarget.z,
		0.0f, 1.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraViewMatrix);

	glLoadIdentity();
	gluPerspective(45.0f, 1.0f, 1.0f, 100.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);

	glLoadIdentity();
	gluLookAt(lightPosition.x, lightPosition.y, lightPosition.z,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);

	glPopMatrix();

	build::Texture tex = build::Texture();
	tex.load("CG-Final\\Image\\floor16.bmp", &ground, 512, 512);
	tex.setWrapS(GL_REPEAT);
	tex.setWrapT(GL_REPEAT);
	tex.setMagFilter(GL_LINEAR);
	tex.setMinFilter(GL_LINEAR);
	tex.load("CG-Final\\Image\\door.bmp", &door, 512, 512);
	tex.setWrapS(GL_REPEAT);
	tex.setWrapT(GL_REPEAT);
	tex.setMagFilter(GL_LINEAR);
	tex.setMinFilter(GL_LINEAR);
	tex.load("CG-Final\\Image\\wall.bmp", &wall, 512, 512);
	tex.setWrapS(GL_REPEAT);
	tex.setWrapT(GL_REPEAT);
	tex.setMagFilter(GL_LINEAR);
	tex.setMinFilter(GL_LINEAR);

	// Added Skybox
	tex.load("CG-Final\\Image\\Skybox\\SkyBox2_up.bmp", &sUp, 2048, 2048);
	tex.setWrapS(GL_CLAMP_TO_EDGE);
	tex.setWrapT(GL_CLAMP_TO_EDGE);
	tex.setMagFilter(GL_LINEAR);
	tex.setMinFilter(GL_NEAREST);
	tex.load("CG-Final\\Image\\Skybox\\SkyBox2_down.bmp", &sDown, 2048, 2048);
	tex.setWrapS(GL_CLAMP_TO_EDGE);
	tex.setWrapT(GL_CLAMP_TO_EDGE);
	tex.setMagFilter(GL_LINEAR);
	tex.setMinFilter(GL_NEAREST);
	tex.load("CG-Final\\Image\\Skybox\\SkyBox2_left.bmp", &sLeft, 2048, 2048);
	tex.setWrapS(GL_CLAMP_TO_EDGE);
	tex.setWrapT(GL_CLAMP_TO_EDGE);
	tex.setMagFilter(GL_LINEAR);
	tex.setMinFilter(GL_NEAREST);
	tex.load("CG-Final\\Image\\Skybox\\SkyBox2_right.bmp", &sRight, 2048, 2048);
	tex.setWrapS(GL_CLAMP_TO_EDGE);
	tex.setWrapT(GL_CLAMP_TO_EDGE);
	tex.setMagFilter(GL_LINEAR);
	tex.setMinFilter(GL_NEAREST);
	tex.load("CG-Final\\Image\\Skybox\\SkyBox2_front.bmp", &sFront, 2048, 2048);
	tex.setWrapS(GL_CLAMP_TO_EDGE);
	tex.setWrapT(GL_CLAMP_TO_EDGE);
	tex.setMagFilter(GL_LINEAR);
	tex.setMinFilter(GL_NEAREST);
	tex.load("CG-Final\\Image\\Skybox\\SkyBox2_back.bmp", &sBack, 2048, 2048);
	tex.setWrapS(GL_CLAMP_TO_EDGE);
	tex.setWrapT(GL_CLAMP_TO_EDGE);
	tex.setMagFilter(GL_LINEAR);
	tex.setMinFilter(GL_NEAREST);

	physicsEngine->setSceneOuterBoundary(-32, -32, 32, 32);
	physicsEngine->setSceneInnerBoundary(-32, 10, -32, 32, 0, 32);
	InitBoxCollidersProperty();
	SetBoxColliderBoundary();

	return true;
}

void DrawRect(GLuint texture) 
{
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glBindTexture(GL_TEXTURE_2D, texture);
	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat point[4][2] = { { x1,y1 },{ x1,y2 },{ x2,y2 },{ x2,y1 } };
	int dir[4][2] = { { 1,0 },{ 1,1 },{ 0,1 },{ 0,0 } };
	glBegin(GL_QUADS);
	for (int i = 0; i < 4; i++) 
	{
		glTexCoord2iv(dir[i]);
		glVertex2fv(point[i]);
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void DrawSkybox()
{
	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);

	// Up
	glPushMatrix();
	glTranslatef(0.0f, SKYBOX_SIZE / 2.0f, 0.0f);
	glRotatef(270, 1, 0, 0);

	glScalef(SKYBOX_SIZE, SKYBOX_SIZE, 1);
	DrawRect(sUp);
	glPopMatrix();

	// Down  
	glPushMatrix();
	glTranslatef(0.0f, -SKYBOX_SIZE / 2.0f, 0.0f);
	glRotatef(90, 1, 0, 0);

	glScalef(SKYBOX_SIZE, SKYBOX_SIZE, 1);
	DrawRect(sDown);
	glPopMatrix();

	// Left
	glPushMatrix();
	glTranslatef(-SKYBOX_SIZE / 2.0f, 0.0f, 0.0f);
	glRotatef(270, 0, 1, 0);

	glScalef(SKYBOX_SIZE, SKYBOX_SIZE, 1);
	DrawRect(sLeft);
	glPopMatrix();

	// Right
	glPushMatrix();
	glTranslatef(SKYBOX_SIZE / 2.0f, 0.0f, 0.0f);
	glRotatef(90, 0, 1, 0);

	glScalef(SKYBOX_SIZE, SKYBOX_SIZE, 1);
	DrawRect(sRight);
	glPopMatrix();

	// Front
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, SKYBOX_SIZE / 2.0);

	glScalef(SKYBOX_SIZE, SKYBOX_SIZE, 1);
	DrawRect(sFront);
	glPopMatrix();

	// Back
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -SKYBOX_SIZE / 2.0f);
	glRotatef(180, 0, 1, 0);

	glScalef(SKYBOX_SIZE, SKYBOX_SIZE, 1);
	DrawRect(sBack);
	glPopMatrix();

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
}

// Called to draw scene
void Display()
{
	glActiveTexture(GL_TEXTURE1);

	glEnable(GL_CULL_FACE);

	// First pass - from light's point of view
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(lightProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(lightViewMatrix);

	// Use viewport the same size as the shadow map
	glViewport(0, 0, shadowMapSize, shadowMapSize);

	// Draw back faces into the shadow map
	glCullFace(GL_FRONT);

	// Disable color writes, and use flat shading for speed
	glShadeModel(GL_FLAT);
	glColorMask(0.05, 0.05, 0.05, 0);

	// Draw the scene
	DrawScene();

	// Read the depth buffer into the shadow map texture
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, shadowMapSize, shadowMapSize);

	// Restore states
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glColorMask(1, 1, 1, 1);

	glDisable(GL_CULL_FACE);

	// 2nd pass - Draw from camera's point of view
	glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(cameraProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(cameraViewMatrix);

	glViewport(0, 0, windowWidth, windowHeight);

	// Use dim light to represent shadowed areas
	glLightfv(GL_LIGHT1, GL_POSITION, VECTOR4D(lightPosition));
	glLightfv(GL_LIGHT1, GL_AMBIENT, white*0.2f*lightPower);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white*0.2f*lightPower);
	glLightfv(GL_LIGHT1, GL_SPECULAR, black*lightPower);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	// 相机缩放
	glScalef(cameraScale, cameraScale, cameraScale);

	DrawScene();

	// 3rd pass
	// Draw with bright light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white*lightPower);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white*lightPower);

	// Calculate texture matrix for projection
	// This matrix takes us from eye space to the light's clip space
	// It is postmultiplied by the inverse of the current view matrix when specifying texgen
	static MATRIX4X4 biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);
	MATRIX4X4 textureMatrix = biasMatrix * lightProjectionMatrix*lightViewMatrix;

	// Set up texture coordinate generation.
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, textureMatrix.GetRow(0));
	glEnable(GL_TEXTURE_GEN_S);

	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, textureMatrix.GetRow(1));
	glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, textureMatrix.GetRow(2));
	glEnable(GL_TEXTURE_GEN_R);

	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, textureMatrix.GetRow(3));
	glEnable(GL_TEXTURE_GEN_Q);

	// Bind & enable shadow map texture
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glEnable(GL_TEXTURE_2D);

	// Enable shadow comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);

	// Shadow comparison should be true (ie not in shadow) if r<=texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);

	// Shadow comparison should generate an INTENSITY result
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

	// Set alpha test to discard false comparisons
	glAlphaFunc(GL_GEQUAL, 0.99f);
	glEnable(GL_ALPHA_TEST);

	DrawScene();

	// NURBS
	{
		nurbs = gluNewNurbsRenderer();
		gluNurbsProperty(nurbs, GLU_SAMPLING_TOLERANCE, 5000.0f);
		gluNurbsProperty(nurbs, GLU_DISPLAY_MODE, GLU_FILL);
		GLfloat ctrlpoints[5][5][3] =
		{
			{{-1.5, -1.5, -1.5},	{-1.5, -0.5, -1.5},	{-1.5, 0.0, -1.5},	{-1.5, 0.5, -1.5},	{-1.5, 1.5, -1.5}},
			{{-0.5, -1.5, -1.5},	{-0.5, -0.5, -4.5},	{-0.5 ,0.0, 4.5},	{-0.5, 0.5, -4.5},	{-0.5, 1.5, -1.5}},
			{{0.5, -1.5, -1.5},		{0.5, -0.5, 1.5},	{0.5, 0.0, 1.5,},	{0.5, 0.5, 1.5},	{0.5, 1.5, -1.5}},
			{{1.5, -1.5, -1.5},		{1.5, -0.5, -1.5},	{1.5, 0.0, -1.5},	{1.5, 0.5, -1.5},	{1.5, 1.5, -1.5}},
			{{2.5, -1.5, -1.5},		{2.5, -0.5, -4.5},	{2.5, 0.0, 4.5},	{2.5, 0.5, -4.5},	{2.5, 1.5, -1.5}}
		};
		GLfloat knots[10] = { 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
		gluBeginSurface(nurbs);
		glTranslatef(4, 2, 0);
		gluNurbsSurface(nurbs, 10, knots, 10, knots, 5 * 3, 3, &ctrlpoints[0][0][0], 5, 5, GL_MAP2_VERTEX_3);
		gluEndSurface(nurbs);
	}

	// Disable textures and texgen
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);

	// Restore other states
	glDisable(GL_LIGHTING);
	glDisable(GL_ALPHA_TEST);

	// Set matrices for ortho
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Reset matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	DrawSkybox();

	glFinish();
	glutSwapBuffers();
	glutPostRedisplay();

	{
		UpdateCameraHoriMovement();
		physicsEngine->updateCameraVertMovement(cameraPosition, cameraTarget);
		UpdateView();
	}
}

// Called on window resize
void Reshape(int w, int h)
{
	// Save new window size
	windowWidth = w, windowHeight = h;

	// Update the camera's projection matrix
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(45.0f, (float)windowWidth / windowHeight, 0.1f, 1000.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);
	glPopMatrix();
}

void Grab()
{
	FILE*    pDummyFile;																	// 指向另一bmp文件，用于复制它的文件头和信息头数据
	FILE*    pWritingFile;																	// 指向要保存截图的bmp文件
	GLubyte* pPixelData;																	// 指向新的空的内存，用于保存截图bmp文件数据
	GLubyte  BMP_Header[BMP_Header_Length];
	GLint    i, j;
	GLint    PixelDataLength;																// BMP文件数据总长度

	// 计算像素数据的实际长度
	i = 800 * 3;																			// 得到每一行的像素数据长度
	while (i % 4 != 0)																		// 补充数据，直到i是的倍数
		++i;
	PixelDataLength = i * 800;																// 补齐后的总位数

	// 分配内存和打开文件
	pPixelData = (GLubyte*)malloc(PixelDataLength);
	if (pPixelData == 0)
		exit(0);

	pDummyFile = fopen("CG-Final\\Image\\door.bmp", "rb");									// 只读形式打开
	if (pDummyFile == 0)
		exit(0);

	pWritingFile = fopen("CG-Final\\Image\\grab.bmp", "wb");								// 只写形式打开
	if (pWritingFile == 0)
		exit(0);

	// 把读入的bmp文件的文件头和信息头数据复制，并修改宽高数据
	fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);									// 读取文件头和信息头，占据54字节
	fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
	fseek(pWritingFile, 0x0012, SEEK_SET);													// 移动到0X0012处，指向图像宽度所在内存
	i = 800;
	j = 800;
	fwrite(&i, sizeof(i), 1, pWritingFile);
	fwrite(&j, sizeof(j), 1, pWritingFile);

	// 读取当前画板上图像的像素数据
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);													// 设置4位对齐方式
	glReadPixels(0, 0, 800, 800,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);

	// 写入像素数据
	fseek(pWritingFile, 0, SEEK_END);
	// 把完整的BMP文件数据写入pWritingFile
	fwrite(pPixelData, PixelDataLength, 1, pWritingFile);

	// 释放内存和关闭文件
	fclose(pDummyFile);
	fclose(pWritingFile);
	free(pPixelData);
}

//Called when a key is pressed
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q': {exit(0); break; }
	case 'a': {aPressed = true; break; }
	case 'd': {dPressed = true; break; }
	case 'w': {wPressed = true; break; }
	case 's': {sPressed = true; break; }
	case 'i': {lightPosition.y -= 0.1f; break; }
	case 'j': {lightPosition.x -= 0.1f; break; }
	case 'k': {lightPosition.y += 0.1f; break; }
	case 'l': {lightPosition.x += 0.1f; break; }
	case 'o': {lightPosition.z += 0.1f; break; }
	case 'u': {lightPosition.z -= 0.1f; break; }
	case '.': {lightPower += 0.1; if (lightPower >= 1.0f)lightPower = 1.0f; break; }
	case ',': {lightPower -= 0.1; if (lightPower <= 0.0f)lightPower = 0.0f; break; }
	case '0': {selectId = 0; break; }
	case '1': {selectId = 1; break; }
	case '2': {selectId = 2; break; }
	case '3': {selectId = 3; break; }
	case '4': {selectId = 4; break; }
	case 'g': {Grab(); break; }
	case ' ': {if (!physicsEngine->isJumping) physicsEngine->jumpAndUpdateVelocity(); physicsEngine->isJumping = true; break; }
	}
}

void KeyUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a': {aPressed = false; break; }
	case 'd': {dPressed = false; break; }
	case 'w': {wPressed = false; break; }
	case 's': {sPressed = false; break; }
	}
}

void Idle()
{
	glutPostRedisplay();
}

int mouseX, mouseY;
bool mouseLeftDown;
void Mouse(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;

	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN)
			mouseLeftDown = true;
		else if (state == GLUT_UP)
			mouseLeftDown = false;

	if (state == GLUT_UP && button == GLUT_WHEEL_UP)
		cameraScale *= 1.02f;
	if (state == GLUT_UP && button == GLUT_WHEEL_DOWN)
		cameraScale *= 0.98f;
	if (state == GLUT_DOWN && button == GLUT_MIDDLE_BUTTON)
		cameraScale = 1.0f;
}

void Motion(int x, int y)
{
	if (mouseLeftDown)
	{
		yaw -= (float)(x - mouseX) * SENSITIVITY;
		pitch -= (float)(y - mouseY) * SENSITIVITY;
		mouseX = x;
		mouseY = y;
		UpdateView();
	}
}

void UpdateCameraHoriMovement()
{
	float dx = 0;
	float dz = 0;

	if (wPressed)
		dz += 0.01f;
	if (sPressed)
		dz -= 0.01f;
	if (aPressed)
		dx -= 0.01f;
	if (dPressed)
		dx += 0.01f;

	if (dz != 0 || dx != 0) 
	{
		// 行走不改变y轴坐标
		VECTOR3D forward = VECTOR3D(cameraViewMatrix.GetEntry(2), 0.0f, cameraViewMatrix.GetEntry(10));
		VECTOR3D strafe = VECTOR3D(cameraViewMatrix.GetEntry(0), 0.0f, cameraViewMatrix.GetEntry(8));

		cameraPosition += (-dz * forward + dx * strafe) * MoveSpeed;
		cameraTarget = cameraPosition + (-dz * forward + dx * strafe) * 1.5f;

		// 每次做完坐标变换后，先进行碰撞检测来调整坐标
		physicsEngine->outCollisionTest(cameraPosition, cameraTarget);
		physicsEngine->inCollisionTest(cameraPosition, cameraTarget);
	}
}

void UpdateView()
{
	{
		// Calculate & save matrices
		glPushMatrix();

		glLoadIdentity();
		gluPerspective(45.0f, 1.0f, 1.0f, 100.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);

		glLoadIdentity();
		gluLookAt(lightPosition.x, lightPosition.y, lightPosition.z,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);

		glPopMatrix();
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	MATRIX4X4 matRoll, matPitch, matYaw, matTrans;
	matRoll.SetRotationAxis(-roll, VECTOR3D(0.0f, 0.0f, 1.0f));
	matPitch.SetRotationAxis(-pitch, VECTOR3D(1.0f, 0.0f, 0.0f));
	matYaw.SetRotationAxis(-yaw, VECTOR3D(0.0f, 1.0f, 0.0f));
	matTrans.SetTranslation(VECTOR3D(-cameraPosition.x, -cameraPosition.y, -cameraPosition.z));
	cameraViewMatrix = matRoll * matPitch * matYaw * matTrans;
	glMultMatrixf(cameraViewMatrix);
}

void DrawScene()
{
	glActiveTexture(GL_TEXTURE0);

	// Set Material
	build::Material material = build::Material();
	material.setDiffuse(VECTOR4D(1, 1, 1, 1));
	material.setSpecular(VECTOR4D(1, 1, 1, 1));
	material.setShininess(32);

	// Ground
	{
		build::Cube gd(60.0f, 0.2f, 60.0f, VECTOR3D(0, -0.1f, 0)); gd.draw();
	}

	// Desks
	{
		desk.Draw(VECTOR3D(-3, 0, 0), VECTOR4D(90, 0, 1, 0), VECTOR3D(0.3, 0.3, 0.3), door);
		desk.Draw(VECTOR3D(3, 0, 0), VECTOR4D(90, 0, 1, 0), VECTOR3D(0.3, 0.3, 0.3), door);
		desk.Draw(VECTOR3D(-3, 0, 3), VECTOR4D(90, 0, 1, 0), VECTOR3D(0.3, 0.3, 0.3), door);
		desk.Draw(VECTOR3D(3, 0, 3), VECTOR4D(90, 0, 1, 0), VECTOR3D(0.3, 0.3, 0.3), door);
	}

	// Light Note : LightBall
	{
		glDisable(GL_LIGHTING);
		build::Ball lightBall(0.3f);
		lightBall.translate(VECTOR3D(lightPosition.x, lightPosition.y, lightPosition.z));
		lightBall.scale(VECTOR3D(1, 1, 1));
		lightBall.draw();
		glEnable(GL_LIGHTING);
	}

	material.setDiffuse(VECTOR4D(1, 1, 1, 1));
	material.setSpecular(VECTOR4D(1, 1, 1, 1));
	material.setShininess(32);

	// Ball
	{
		build::Ball ball(0.3f);
		ball.translate(VECTOR3D(0, (count1 % 400) / 400.0f + 0.3f, 0));
		ball.scale(VECTOR3D((count1 % 400) / 400.0f + 1, (count1 % 400) / 400.0f + 1, (count1 % 400) / 400.0f + 1));
		if (selectId == 1)
		{
			count1++;

			material.setDiffuse(VECTOR4D(1, 0, 0, 1));
			material.setSpecular(VECTOR4D(1, 0, 0, 1));
			material.setShininess(32);

			ball.onTexture(ground);
		}
		else
			ball.onTexture(wall);
		ball.draw();
		ball.offTexture();
	}

	material.setDiffuse(VECTOR4D(1, 1, 1, 1));
	material.setSpecular(VECTOR4D(1, 1, 1, 1));
	material.setShininess(32);

	// Cube
	{
		build::Cube cube(0.6f, 0.6f, 0.6f);
		cube.translate(VECTOR3D(0.0f, (count2 % 400) / 400.0f + 0.3f, -4.0f));
		if (selectId == 2)
		{
			count2++;

			material.setDiffuse(VECTOR4D(1, 0, 0, 1));
			material.setSpecular(VECTOR4D(1, 0, 0, 1));
			material.setShininess(32);

			cube.onTexture(ground);
		}
		else
			cube.onTexture(wall);
		cube.draw();
		cube.offTexture();
	}

	material.setDiffuse(VECTOR4D(1, 1, 1, 1));
	material.setSpecular(VECTOR4D(1, 1, 1, 1));
	material.setShininess(32);

	// Cylin
	{
		build::Cylin cylin(0.3f, 0.6f);
		cylin.translate(VECTOR3D(0.0f, 2.0f, -(count3 % 400) / 400.0f - 0.6f));
		cylin.rotate(VECTOR4D(90, 1, 0, 0));
		if (selectId == 3)
		{
			count3++;

			material.setDiffuse(VECTOR4D(1, 0, 0, 1));
			material.setSpecular(VECTOR4D(1, 0, 0, 1));
			material.setShininess(32);

			cylin.onTexture(ground);
		}
		else
			cylin.onTexture(wall);
		cylin.draw();
		cylin.offTexture();
	}

	material.setDiffuse(VECTOR4D(1, 1, 1, 1));
	material.setSpecular(VECTOR4D(1, 1, 1, 1));
	material.setShininess(32);

	// Pyram
	{
		build::Pyram pyram(0.3f, 0.6f);
		pyram.translate(VECTOR3D(0.0f, 2.0f, (count4 % 400) / 400.0f));
		pyram.rotate(VECTOR4D(270, 1, 0, 0));
		if (selectId == 4)
		{
			count4++;

			material.setDiffuse(VECTOR4D(1, 0, 0, 1));
			material.setSpecular(VECTOR4D(1, 0, 0, 1));
			material.setShininess(32);

			pyram.onTexture(ground);
		}
		else
			pyram.onTexture(wall);
		pyram.draw();
		pyram.offTexture();
	}

	// Particle System
	{
		glPushMatrix();
		glTranslatef(0, 10.0f, 0);
		particleSystem.simulate(0.01);
		particleSystem.render();
		glPopMatrix();
	}

	// Box Colliders
	for (int i = 0; i < boxNum; ++i)
	{
		build::Cube box(boxScale[i].x, boxScale[i].y, boxScale[i].z, VECTOR3D(boxPosition[i].x, boxPosition[i].y, boxPosition[i].z));
		box.onTexture(ground);
		box.draw();
		box.offTexture();
	}

	glActiveTexture(GL_TEXTURE1);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Project");

	if (!Init())
		return 0;

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutMotionFunc(Motion);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyUp);
	glutIdleFunc(Idle);
	glutMainLoop();
	delete physicsEngine;
	return 0;
}