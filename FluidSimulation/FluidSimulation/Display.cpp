#include "FluidSimulation.h"

void Display(void)
{
	// Bind visualization shader and set up blend state:
    glUseProgram(DrawProgram);
    GLint fillColor = glGetUniformLocation(DrawProgram, "FillColor");
    GLint scale = glGetUniformLocation(DrawProgram, "Scale");
    glEnable(GL_BLEND);

	// Set render target to the backbuffer:
    glViewport(0, 0, g_iWidth, g_iHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

	// Draw ink:
    glBindTexture(GL_TEXTURE_2D, Density.Ping.TextureHandle);
    glUniform3f(fillColor, 1, 0, 0);
    glUniform2f(scale, 1.0f / g_iWidth, 1.0f / g_iHeight);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    // Draw obstacles:
    glBindTexture(GL_TEXTURE_2D, Boundaries.TextureHandle);
    glUniform3f(fillColor, 0.125f, 0.4f, 0.75f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    // Disable blending:
    glDisable(GL_BLEND);
}