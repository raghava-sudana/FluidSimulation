#include <GL\glew.h>
#include <GL\freeglut.h>
#include <windows.h>
#include <iostream>
#include <stdio.h>

void Intialize();
void Calculate(int data);
void Display(void);
void render(void);

#define GridWidth (480)
#define GridHeight (853)
#define g_iWidth (512)
#define g_iHeight (512)

GLuint DrawQuad();

static const int PositionSlot = 0;

typedef struct Surface_ {
    GLuint FboHandle;
    GLuint TextureHandle;
    int NumComponents;
} Surface;

typedef struct PPSurface_ {
	Surface Ping;
	Surface Pong;
} PPSurface;

typedef struct Vector2_ {
    int X;
    int Y;
} Vector2;

#define CellSize (1.25f)
#define SplatRadius ((float) g_iWidth / 8.0f)

Surface CreateSurface(GLsizei width, GLsizei height, int Components);
PPSurface CreatePPSurface(GLsizei width, GLsizei height, int Components);
void ClearSurface(Surface s, float value);

void CreateBoundaries(Surface dest, int width, int height);

GLuint CreateProgram(const char* VertexShader, const char* FragmentShader);

void InitOps();
void SwapSurfaces(PPSurface* ppsurface);

static const float AmbientTemperature = 0.0f;
static const float VelocityDissipation = 1.0f;
static const float DensityDissipation = 1.5f;
static const float TemperatureDissipation = 1.0f;
static const float TimeStep = 0.125f;
static const float GradientScale = 1.125f / CellSize;
static const float SmokeBuoyancy = 10.0f;
static const float SmokeWeight = 0.05f;
static const float ImpulseTemperature = 10.0f;
static const float ImpulseDensity = 1.0f;
static const int NumJacobiIterations = 40;
static const Vector2 ImpulsePosition = { g_iWidth / 2, - (int) SplatRadius / 2};



void Advect(Surface velocity, Surface source, Surface boundaries, Surface dest, float dissipation);
void Jacobi(Surface pressure, Surface divergence, Surface boundaries, Surface dest);
void ComputeDivergence(Surface velocity, Surface boundaries, Surface dest);
void ApplyImpulse(Surface dest, Vector2 position, float value);
void ApplyBuoyancy(Surface velocity, Surface temperature, Surface density, Surface dest);
void SubtractGradient(Surface velocity, Surface pressure, Surface boundaries, Surface dest);
