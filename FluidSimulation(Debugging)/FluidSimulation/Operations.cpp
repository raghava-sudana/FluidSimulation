#include "FluidSimulation.h"
#include <math.h>

struct ProgramsRec {
    GLuint Advect;
    GLuint Jacobi;
    GLuint SubtractGradient;
    GLuint ComputeDivergence;
    GLuint ApplyImpulse;
    GLuint ApplyBuoyancy;
	GLuint ApplyBuoyancy2;
} Programs;

static void ResetState()
{
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_BLEND);
}

void InitOps()
{
    Programs.Advect = CreateProgram("Vertex.ver", "Advect.frag");
    Programs.Jacobi = CreateProgram("Vertex.ver", "Jacobi.frag");
    Programs.SubtractGradient = CreateProgram("Vertex.ver", "SubractGradient.frag");
    Programs.ComputeDivergence = CreateProgram("Vertex.ver", "ComputeDivergence.frag");
    Programs.ApplyImpulse = CreateProgram("Vertex.ver", "Splat.frag");
    Programs.ApplyBuoyancy = CreateProgram("Vertex.ver", "Buoyancy.frag");
	Programs.ApplyBuoyancy2 = CreateProgram("Vertex.ver", "Buoyancy2.frag");
}

void ClearSurface(Surface s, float v)
{
    glBindFramebuffer(GL_FRAMEBUFFER, s.FboHandle);
    glClearColor(v, v, v, v);
    glClear(GL_COLOR_BUFFER_BIT);
}

void SwapSurfaces(PPSurface* ppsurface)
{
	Surface temp = ppsurface->Ping;
	ppsurface->Ping = ppsurface->Pong;
	ppsurface->Pong = temp;
}

void Advect(Surface velocity, Surface source, Surface boundaries, Surface dest, float dissipation)
{
	
	GLuint p = Programs.Advect;
    glUseProgram(p);

	GLint inverseSize = glGetUniformLocation(p, "InverseSize");
    GLint timeStep = glGetUniformLocation(p, "TimeStep");
    GLint dissLoc = glGetUniformLocation(p, "Dissipation");
    GLint sourceTexture = glGetUniformLocation(p, "SourceTexture");
    GLint boundariesTexture = glGetUniformLocation(p, "Boundaries");
	
	glUniform2f(inverseSize, 1.0f / g_iWidth, 1.0f / g_iHeight);
    glUniform1f(timeStep, TimeStep);
    glUniform1f(dissLoc, dissipation);
    glUniform1i(sourceTexture, 1);
    glUniform1i(boundariesTexture, 2);

	glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, velocity.TextureHandle);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, source.TextureHandle);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, boundaries.TextureHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    ResetState();
}

void Jacobi(Surface pressure, Surface divergence, Surface boundaries, Surface dest)
{
    GLuint p = Programs.Jacobi;
    glUseProgram(p);

    GLint alpha = glGetUniformLocation(p, "Alpha");
    GLint inverseBeta = glGetUniformLocation(p, "InverseBeta");
    GLint dSampler = glGetUniformLocation(p, "Divergence");
    GLint bSampler = glGetUniformLocation(p, "Boundaries");

    glUniform1f(alpha, -CellSize * CellSize);
    glUniform1f(inverseBeta, 0.25f);
    glUniform1i(dSampler, 1);
    glUniform1i(bSampler, 2);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pressure.TextureHandle);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, divergence.TextureHandle);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, boundaries.TextureHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    ResetState();
}

void SubtractGradient(Surface velocity, Surface pressure, Surface boundaries, Surface dest)
{
    GLuint p = Programs.SubtractGradient;
    glUseProgram(p);

    GLint gradientScale = glGetUniformLocation(p, "GradientScale");
    glUniform1f(gradientScale, GradientScale);
    GLint halfCell = glGetUniformLocation(p, "HalfInverseCellSize");
    glUniform1f(halfCell, 0.5f / CellSize);
    GLint sampler = glGetUniformLocation(p, "Pressure");
    glUniform1i(sampler, 1);
    sampler = glGetUniformLocation(p, "Boundaries");
    glUniform1i(sampler, 2);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, velocity.TextureHandle);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pressure.TextureHandle);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, boundaries.TextureHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    ResetState();
}

void ComputeDivergence(Surface velocity, Surface boundaries, Surface dest)
{
    GLuint p = Programs.ComputeDivergence;
    glUseProgram(p);

    GLint halfCell = glGetUniformLocation(p, "HalfInverseCellSize");
    glUniform1f(halfCell, 0.5f / CellSize);
    GLint sampler = glGetUniformLocation(p, "Boundaries");
    glUniform1i(sampler, 1);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, velocity.TextureHandle);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, boundaries.TextureHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    ResetState();
}

void ApplyImpulse(Surface dest, Vector2 position, float value)
{
    GLuint p = Programs.ApplyImpulse;
    glUseProgram(p);

    GLint pointLoc = glGetUniformLocation(p, "Point");
    GLint radiusLoc = glGetUniformLocation(p, "Radius");
    GLint fillColorLoc = glGetUniformLocation(p, "FillColor");

    glUniform2f(pointLoc, (float) position.X, (float) position.Y);
    glUniform1f(radiusLoc, SplatRadius);
    glUniform3f(fillColorLoc, value, value, value);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glEnable(GL_BLEND);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    ResetState();
}

void ApplyBuoyancy(Surface velocity, Surface temperature, Surface density, Surface dest, float x, float y)
{
    GLuint p = Programs.ApplyBuoyancy;
    glUseProgram(p);

    GLint tempSampler = glGetUniformLocation(p, "Temperature");
    GLint inkSampler = glGetUniformLocation(p, "Density");
    GLint ambTemp = glGetUniformLocation(p, "AmbientTemperature");
    GLint timeStep = glGetUniformLocation(p, "TimeStep");
    GLint sigma = glGetUniformLocation(p, "Sigma");
    GLint kappa = glGetUniformLocation(p, "Kappa");
	GLint ix = glGetUniformLocation(p, "X");
	GLint iy = glGetUniformLocation(p, "Y");

    glUniform1i(tempSampler, 1);
    glUniform1i(inkSampler, 2);
    glUniform1f(ambTemp, AmbientTemperature);
    glUniform1f(timeStep, TimeStep);
    glUniform1f(sigma, SmokeBuoyancy);
    glUniform1f(kappa, SmokeWeight);
	glUniform1f(ix, x);
	glUniform1f(iy, y);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, velocity.TextureHandle);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, temperature.TextureHandle);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, density.TextureHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    ResetState();
}

void ApplyBuoyancy2(Surface velocity, Surface temperature, Surface density, Surface dest)
{
    GLuint p = Programs.ApplyBuoyancy2;
    glUseProgram(p);

    GLint tempSampler = glGetUniformLocation(p, "Temperature");
    GLint inkSampler = glGetUniformLocation(p, "Density");
    GLint ambTemp = glGetUniformLocation(p, "AmbientTemperature");
    GLint timeStep = glGetUniformLocation(p, "TimeStep");
    GLint sigma = glGetUniformLocation(p, "Sigma");
    GLint kappa = glGetUniformLocation(p, "Kappa");

    glUniform1i(tempSampler, 1);
    glUniform1i(inkSampler, 2);
    glUniform1f(ambTemp, AmbientTemperature);
    glUniform1f(timeStep, TimeStep);
    glUniform1f(sigma, SmokeBuoyancy);
    glUniform1f(kappa, SmokeWeight);

    glBindFramebuffer(GL_FRAMEBUFFER, dest.FboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, velocity.TextureHandle);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, temperature.TextureHandle);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, density.TextureHandle);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    ResetState();
}