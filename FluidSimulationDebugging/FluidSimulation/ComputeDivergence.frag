#version 150

out float FragColor;

uniform sampler2D Velocity;
uniform sampler2D Boundaries;
uniform float HalfInverseCellSize;

void main()
{
    ivec2 T = ivec2(gl_FragCoord.xy);

    // Find neighboring velocities:
    vec2 vN = texelFetchOffset(Velocity, T, 0, ivec2(0, 1)).xy;
    vec2 vS = texelFetchOffset(Velocity, T, 0, ivec2(0, -1)).xy;
    vec2 vE = texelFetchOffset(Velocity, T, 0, ivec2(1, 0)).xy;
    vec2 vW = texelFetchOffset(Velocity, T, 0, ivec2(-1, 0)).xy;

	// Find Boundaries:
    vec3 bN = texelFetchOffset(Boundaries, T, 0, ivec2(0, 1)).xyz;
    vec3 bS = texelFetchOffset(Boundaries, T, 0, ivec2(0, -1)).xyz;
    vec3 bE = texelFetchOffset(Boundaries, T, 0, ivec2(1, 0)).xyz;
    vec3 bW = texelFetchOffset(Boundaries, T, 0, ivec2(-1, 0)).xyz;

    // Use Boundary velocities for solid cells:
    if (bN.x > 0) vN = bN.yz;
    if (bS.x > 0) vS = bS.yz;
    if (bE.x > 0) vE = bE.yz;
    if (bW.x > 0) vW = bW.yz;


    FragColor = HalfInverseCellSize * (vE.x - vW.x + vN.y - vS.y);
}