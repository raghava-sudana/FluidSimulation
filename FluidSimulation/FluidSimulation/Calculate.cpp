#include "FluidSimulation.h"

void Calculate(int data)
{

	glViewport(0, 0, g_iWidth, g_iHeight);

	Advect(Velocity.Ping, Velocity.Ping, Boundaries, Velocity.Pong, VelocityDissipation);
	SwapSurfaces(&Velocity);

	Advect(Velocity.Ping, Temperature.Ping, Boundaries, Temperature.Pong, TemperatureDissipation);
    SwapSurfaces(&Temperature);

	Advect(Velocity.Ping, Density.Ping, Boundaries, Density.Pong, DensityDissipation);
    SwapSurfaces(&Density);

	ApplyBuoyancy(Velocity.Ping, Temperature.Ping, Density.Ping, Velocity.Pong);
    SwapSurfaces(&Velocity);

	ApplyImpulse(Temperature.Ping, ImpulsePosition, ImpulseTemperature);
    ApplyImpulse(Density.Ping, ImpulsePosition, ImpulseDensity);

	ComputeDivergence(Velocity.Ping, Boundaries, Divergence);
    ClearSurface(Pressure.Ping, 0);

	for (int i = 0; i < NumJacobiIterations; ++i) {
        Jacobi(Pressure.Ping, Divergence, Boundaries, Pressure.Pong);
        SwapSurfaces(&Pressure);
    }

	SubtractGradient(Velocity.Ping, Pressure.Ping, Boundaries, Velocity.Pong);
    SwapSurfaces(&Velocity);

	glutPostRedisplay();
}