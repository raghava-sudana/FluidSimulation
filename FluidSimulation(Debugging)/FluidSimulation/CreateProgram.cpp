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
	GLint compileSuccess;
	GLchar compilerSpew[256];

	//Attach Vertex Shader 
	GLuint vsHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsHandle, 1, &vpString, 0);
    glCompileShader(vsHandle);
	glGetShaderiv(vsHandle, GL_COMPILE_STATUS, &compileSuccess);
    glGetShaderInfoLog(vsHandle, sizeof(compilerSpew), 0, compilerSpew);
	if (!compileSuccess) {
		 std::cout<<"Failed to compile/attach";
	 }
	 else
	 {	
		 std::cout<<"Succeded to compile/attach";
	}



    glAttachShader(programHandle, vsHandle);

	//Attach Fragment Shader
	GLuint fsHandle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsHandle, 1, &fpString, 0);
    glCompileShader(fsHandle);
	glAttachShader(programHandle, fsHandle);
	glGetShaderiv(fsHandle, GL_COMPILE_STATUS, &compileSuccess);
    glGetShaderInfoLog(fsHandle, sizeof(compilerSpew), 0, compilerSpew);
	if (!compileSuccess) {
		 std::cout<<"Failed to compile/attach";
	 }
	 else
	 {	
		 std::cout<<"Succeded to compile/attach";
	}


	glLinkProgram(programHandle);

	GLint linkSuccess;
	
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    glGetProgramInfoLog(programHandle, sizeof(compilerSpew), 0, compilerSpew);

	 if (!linkSuccess) {
		 std::cout<<"Failed to compile/attach";
	 }
	 else
	 {	
		 std::cout<<"Succeded to compile/attach";
	}


	return programHandle;

}

