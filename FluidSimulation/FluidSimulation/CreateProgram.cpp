#include "FluidSimulation.h"
#include <string.h>
#include "Text.h"

GLuint CreateProgram(const char* VertexShader, const char* FragmentShader)
{
	Text sourceVert, sourceFrag;
	//Load Vertex Shader
	sourceVert.readTexFile(VertexShader);
	const char* vpString = sourceVert.getContent();
	//Load Fragment Shader
	sourceFrag.readTexFile(FragmentShader);
	const char* fpString = sourceFrag.getContent();

	GLuint programHandle = glCreateProgram();

	//Attach Vertex Shader 
	GLuint vsHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsHandle, 1, &vpString, 0);
    glCompileShader(vsHandle);
    glAttachShader(programHandle, vsHandle);

	//Attach Fragment Shader
	GLuint fsHandle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsHandle, 1, &fpString, 0);
    glCompileShader(fsHandle);
	glAttachShader(programHandle, fsHandle);
	glLinkProgram(programHandle);

	return programHandle;

}

