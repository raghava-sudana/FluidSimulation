#define GLEW_STATIC

#include "FluidSimulation.h"

//Simple main functionwhich calls Display and Calculate functions in Loop

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
	glutMainLoop();
	return 0;
}
