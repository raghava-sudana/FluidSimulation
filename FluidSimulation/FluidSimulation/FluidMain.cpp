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
	glutTimerFunc(125, Calculate, -1);
	glutMainLoop();
	return 0;
}



void render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*glBindTexture(GL_TEXTURE_2D, g_iTexID[0]);
	glUseProgram(g_iProgram1);

	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 1); glVertex2f(-1, 1);
		glTexCoord2f(1, 1); glVertex2f(1, 1);
		glTexCoord2f(1, 0); glVertex2f(1, -1);
		glTexCoord2f(0, 0); glVertex2f(-1, -1);
	}
	glEnd();

	if ( g_bSwitched ) {
		glBindTexture(GL_TEXTURE_2D, g_iTexID[1]);
		glCopyTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 512, 512, 0 );
		glUseProgram(g_iProgram2);

		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 1); glVertex2f(-1, 1);
			glTexCoord2f(1, 1); glVertex2f(1, 1);
			glTexCoord2f(1, 0); glVertex2f(1, -1);
			glTexCoord2f(0, 0); glVertex2f(-1, -1);
		}
		glEnd();
	}*/

	glFlush();
	glutSwapBuffers();
}

