// glutEx1.cpp : 定义控制台应用程序的入口点。

#include <stdlib.h>
#include "gl/glut.h"

float fTranslate;
float fRotate;
float fScale = 1.0f;	// set inital scale value to 1.0f

float mat_diffuse[] = { 0.85f, 0.65f, 0.2f, 1.0f };	// teapot
float mat_specular[] = { 0.6f, 0.6f, 0.6f, 1.0f };	// teapot
float mat_color0[] = { 0.85f, 0.65f, 0.2f, 1.0f };
float mat_color1[] = { 1.0f, 0.0f, 0.0f, 1.0f };	// desk
float mat_color2[] = { 0.0f, 1.0f, 0.0f, 1.0f };	// leg1
float mat_color3[] = { 1.0f, 1.0f, 0.0f, 1.0f };	// leg2
float mat_color4[] = { 0.0f, 1.0f, 1.0f, 1.0f };	// leg3
float mat_color5[] = { 0.0f, 0.0f, 1.0f, 1.0f };	// leg4

bool red = false;

GLfloat lightPos[] = { 5.0f,5.0f,5.0f,1.0f };
GLfloat lightColor[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat zero[] = { 0,0,0,0 };
GLfloat lightSpotPos[] = { 0.0f,10.0f,0.0f,1.0f };
GLfloat lightSpotCutOff = 2.0f;
GLfloat lightSpotDirection[] = { 0.0f,-1.0f,0.0f };
GLfloat lightSpotExponent = 2.0f;

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

int wHeight = 0;
int wWidth = 0;

void Draw_Leg();

void Draw_Scene()
{
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMateriali(GL_FRONT, GL_SHININESS, 50);
	glTranslatef(0, 0, 4.7);
	glRotatef(90, 1, 0, 0);
	glutSolidTeapot(1);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, zero);
	glMateriali(GL_FRONT, GL_SHININESS, 0);
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, zero);
	glMateriali(GL_FRONT, GL_SHININESS, 0);
	glTranslatef(1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color3);
	glMaterialfv(GL_FRONT, GL_SPECULAR, zero);
	glMateriali(GL_FRONT, GL_SHININESS, 0);
	glTranslatef(-1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color4);
	glMaterialfv(GL_FRONT, GL_SPECULAR, zero);
	glMateriali(GL_FRONT, GL_SHININESS, 0);
	glTranslatef(1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color5);
	glMaterialfv(GL_FRONT, GL_SPECULAR, zero);
	glMateriali(GL_FRONT, GL_SHININESS, 0);
	glTranslatef(-1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();
}

void Draw_Leg()
{
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
}

void updateView(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width/(GLfloat)height;
	if (bPersp) {
		gluPerspective(45.0f, whRatio,0.1f,100.0f);
		//glFrustum(-3, 3, -3, 3, 3,100);
	} else {
		glOrtho(-3 ,3, -3, 3,-100,100);
	}

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();
}

float eye[] = {0, 0, 8};
float center[] = {0, 0, 0};

void key(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 27 : {exit(0); break; }
	case 'p': {bPersp = !bPersp; break; }
	case ' ': {bAnim = !bAnim; break;}
	case 'z': {bWire = !bWire; break;}
	case 'x': {red = !red; break; }
	case 'a': {eye[0] += 0.2f; center[0] += 0.2f; break; }
	case 'd': {eye[0] -= 0.2f; center[0] -= 0.2f; break; }
	case 's': {eye[1] += 0.2f; center[1] += 0.2f; break; }
	case 'w': {eye[1] -= 0.2f; center[1] -= 0.2f; break; }
	case 'q': {eye[2] -= 0.2f; center[2] -= 0.2f; break; }
	case 'e': {eye[2] += 0.2f; center[2] += 0.2f; break; }
	case 'A': {lightSpotDirection[0] += 0.2f; break; }
	case 'D': {lightSpotDirection[0] -= 0.2f; break; }
	case 'S': {lightSpotDirection[1] += 0.2f; break; }
	case 'W': {lightSpotDirection[1] -= 0.2f; break; }
	case 'Q': {lightSpotDirection[2] += 0.2f; break; }
	case 'E': {lightSpotDirection[2] -= 0.2f; break; }
	case 'j': {lightPos[1] -= 0.2f; break; }
	case 'l': {lightPos[1] += 0.2f; break; }
	case 'k': {lightPos[0] -= 0.2f; break; }
	case 'i': {lightPos[0] += 0.2f; break; }
	case 'u': {lightPos[2] -= 0.2f; break; }
	case 'o': {lightPos[2] += 0.2f; break; }
	case 'J': {lightSpotPos[1] -= 0.2f; break; }
	case 'L': {lightSpotPos[1] += 0.2f; break; }
	case 'K': {lightSpotPos[0] -= 0.2f; break; }
	case 'I': {lightSpotPos[0] += 0.2f; break; }
	case 'U': {lightSpotPos[2] -= 0.2f; break; }
	case 'O': {lightSpotPos[2] += 0.2f; break; }
	case 'V': {lightSpotCutOff += 0.2f; break; }
	case 'B': {lightSpotCutOff -= 0.2f; break; }
	}

	updateView(wHeight, wWidth);
}


void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);				// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上

	if (bWire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (red)
	{
		lightColor[0] = 1.0f;
		lightColor[1] = 0.0f;
		lightColor[2] = 0.0f;
		lightColor[3] = 1.0f;
	}
	else
	{
		lightColor[0] = 1.0f;
		lightColor[1] = 1.0f;
		lightColor[2] = 1.0f;
		lightColor[3] = 1.0f;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor);
	glLightfv(GL_LIGHT1, GL_POSITION, lightSpotPos);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, lightSpotCutOff);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightSpotDirection);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, lightSpotExponent);
	glEnable(GL_LIGHT1);

	//	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Scene();						// Draw triangle

	if (bAnim) fRotate    += 0.5f;
	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480, 480);
	int windowHandle = glutCreateWindow("Simple GLUT App");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);	
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}