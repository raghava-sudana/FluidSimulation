#include "FluidSimulation.h"

void Intialize()
{
	Velocity = CreatePPSurface(g_iWidth, g_iHeight, 2);

	Density = CreatePPSurface(g_iWidth, g_iHeight, 1);
	Pressure = CreatePPSurface(g_iWidth, g_iHeight, 1);
	Temperature = CreatePPSurface(g_iWidth, g_iHeight, 1);
	Divergence = CreateSurface(g_iWidth, g_iHeight, 3);
	InitOps();
	DrawProgram = CreateProgram("Vertex.ver","Visualize.frag");
	Boundaries = CreateSurface(g_iWidth, g_iHeight, 3);
	CreateBoundaries(Boundaries, g_iWidth, g_iHeight);
	VertexQuad = DrawQuad();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ClearSurface(Temperature.Ping, AmbientTemperature);

}

GLuint DrawQuad()
{
	short positions[] = {
        -1, -1,
         1, -1,
        -1,  1,
         1,  1,
    };

	GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

	GLuint vbo;
    GLsizeiptr size = sizeof(positions);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW);

	//
	GLsizeiptr stride = 2 * sizeof(positions[0]);
    glEnableVertexAttribArray(PositionSlot);
    glVertexAttribPointer(PositionSlot, 2, GL_SHORT, GL_FALSE, stride, 0);

	return vao;
}

PPSurface CreatePPSurface(GLsizei width, GLsizei height, int Components)
{
	PPSurface ppsurface;
	ppsurface.Ping = CreateSurface(width, height, Components);
	ppsurface.Pong = CreateSurface(width, height, Components);

	return ppsurface;

}

Surface CreateSurface(GLsizei width, GLsizei height, int Components)
{
	GLuint fboHandle;
    glGenFramebuffers(1, &fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);

    GLuint textureHandle;
    glGenTextures(1, &textureHandle);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	switch (Components) {
		case 1: glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width, height, 0, GL_RED, GL_HALF_FLOAT, 0); break;
        case 2: glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_HALF_FLOAT, 0); break;
        case 3: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_HALF_FLOAT, 0); break;
        case 4: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_HALF_FLOAT, 0); break;
    }
	
	GLuint colorbuffer;
    glGenRenderbuffers(1, &colorbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureHandle, 0);
    
	Surface surface = { fboHandle, textureHandle, Components };

	glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return surface;


}

void CreateBoundaries(Surface dest, int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
	glViewport(0, 0, width, height);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

	GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
	//GLuint program = CreateProgram("Fluid.Vertex", 0, "Fluid.Fill");
	//glUseProgram(program);

	#define T 0.9999f
    float positions[] = { -T, -T, T, -T, T,  T, -T,  T, -T, -T };
    #undef T
    GLuint vbo;
    GLsizeiptr size = sizeof(positions);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, positions, GL_STATIC_DRAW);
    GLsizeiptr stride = 2 * sizeof(positions[0]);
    glEnableVertexAttribArray(PositionSlot);
    glVertexAttribPointer(PositionSlot, 2, GL_FLOAT, GL_FALSE, stride, 0);
    glDrawArrays(GL_LINE_STRIP, 0, 5);
    glDeleteBuffers(1, &vbo);

	//glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);

}