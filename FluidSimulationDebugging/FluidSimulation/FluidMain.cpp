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
	Calculate(1); //Passing 1 as useless value.
	glutDisplayFunc(Display); //Check for the framebuffer object to be passed over here
	//glutIdleFunc(Display);
	glutMainLoop();
	return 0;
}
