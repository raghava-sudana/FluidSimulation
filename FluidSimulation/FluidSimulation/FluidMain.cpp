#define GLEW_STATIC

#include "FluidSimulation.h"


int main (int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_iWidth, g_iHeight);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Fluid Simulation");
	glewInit();
	Intialize();
	Calculate(1);
	glutDisplayFunc(Display);
	//glutTimerFunc(125, Calculate, -1);
	glutMainLoop();
	return 0;
}



void render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glFlush();
	glutSwapBuffers();
}

