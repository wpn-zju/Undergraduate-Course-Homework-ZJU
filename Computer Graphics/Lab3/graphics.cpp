// This assignment may cost you some efferts, so I give you some important HINTS, hope that may help you.
// Enjoy the coding and thinking, do pay more attention to the library functions used in OPENGL, such as how they are used? what are the parameters used? and why?

// 实验报告里面多写点感想，即对函数的理解，以及调用的顺序，步骤等。思考为什么是这样子调用的，为什么参数是这样子设置的？。。。等等你觉得值得研究的问题。
#include <stdlib.h>
#include "gl/glut.h"


float fTranslate;
float fRotate = 0.0f;
float fScale = 1.0f;	// set inital scale value to 1.0f

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

int wHeight = 0;
int wWidth = 0;

bool tAnim = false;
float tPos[] = { 0, 0, 0 };
float tRotate = 0.0f;

void Draw_Leg()
{
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
}

void Draw_Scene()
{
	glPushMatrix();
	glTranslatef(tPos[0], tPos[1], 4 + 1);
	glRotatef(90, 1, 0, 0); //notice the rotation here, you may have a TRY removing this line to see what it looks like.
	glRotatef(tRotate, 0, 1, 0);
	glutSolidTeapot(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

}

void updateView(int width, int height)
{
	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width / (GLfloat)height;

	if (bPersp)
		gluPerspective(60, whRatio, 0.01, 100); // 透视投影
	else
		glOrtho(-3, 3, -3, 3, -100, 100);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();
}

float eye[] = { 0, 0, 8 };
float center[] = { 0, 0, 0 };

void key(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 27:
	case 'q': {exit(0); break; } // p或esc退出
	case 'p': {bPersp = !bPersp; updateView(wHeight, wWidth); break; } // p改变投影方式
	
	case ' ': {bAnim = !bAnim; break; } // 运动与暂停
	case 'o': {bWire = !bWire; break; } // 改变填充
	
	case 'a': {eye[0] += 0.25; center[0] += 0.25; break; }
	case 'd': {eye[0] -= 0.25; center[0] -= 0.25; break; }
	case 'w': {eye[1] -= 0.25; center[1] -= 0.25; break; }
	case 's': {eye[1] += 0.25; center[1] += 0.25; break; }
	case 'z': {eye[2] -= 0.25; center[2] -= 0.25; break; }
	case 'c': {eye[2] += 0.25; center[2] += 0.25; break; }
	
	//茶壶相关操作
	case 'l': {if (tPos[0] < 2.5) tPos[0] += 0.25; break; }
	case 'j': {if (tPos[0] > -2.5) tPos[0] -= 0.25; break; }
	case 'i': {if (tPos[1] < 2) tPos[1] += 0.25; break; }
	case 'k': {if (tPos[1] > -2) tPos[1] -= 0.25; break; }
	case 'e': {tAnim = !tAnim; break; }
	}
}


void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color buffer and depth buffer 清楚深度和颜色缓冲
	glLoadIdentity();									// Reset The Current Modelview Matrix 单位矩阵 Identity Matrix

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0);				// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上

	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // without fill, only lines 不填充
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // with fill 填充
	}

	glEnable(GL_DEPTH_TEST); // open depth test 开启深度测试
	glEnable(GL_LIGHTING); // open light 开启灯光
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = { 5,5,5,1 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos); // 点光源
	glLightfv(GL_LIGHT0, GL_AMBIENT, white); // 环境光
	glEnable(GL_LIGHT0);

	//	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Scene();						// Draw Scene

	if (bAnim) fRotate += 0.5f;
	if (tAnim) tRotate += 0.5f;

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480, 480);
	int windowHandle = glutCreateWindow("Ex 3");

	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}


