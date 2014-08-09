#version 150

out vec4 FragColor;

uniform sampler2D Pressure;
uniform sampler2D Divergence;
uniform sampler2D Boundaries;

uniform float Alpha;
uniform float InverseBeta;

void main()
{
    ivec2 T = ivec2(gl_FragCoord.xy);

    // Find neighboring pressure:
    vec4 pN = texelFetchOffset(Pressure, T, 0, ivec2(0, 1));
    vec4 pS = texelFetchOffset(Pressure, T, 0, ivec2(0, -1));
    vec4 pE = texelFetchOffset(Pressure, T, 0, ivec2(1, 0));
    vec4 pW = texelFetchOffset(Pressure, T, 0, ivec2(-1, 0));
    vec4 pC = texelFetch(Pressure, T, 0);

	// Find Boundaries:
    vec3 bN = texelFetchOffset(Boundaries, T, 0, ivec2(0, 1)).xyz;
    vec3 bS = texelFetchOffset(Boundaries, T, 0, ivec2(0, -1)).xyz;
    vec3 bE = texelFetchOffset(Boundaries, T, 0, ivec2(1, 0)).xyz;
    vec3 bW = texelFetchOffset(Boundaries, T, 0, ivec2(-1, 0)).xyz;

    // Use center pressure for solid cells:
    if (bN.x > 0) pN = pC;
    if (bS.x > 0) pS = pC;
    if (bE.x > 0) pE = pC;
    if (bW.x > 0) pW = pC;

    vec4 bC = texelFetch(Divergence, T, 0);
    FragColor = (pW + pE + pS + pN + Alpha * bC) * InverseBeta;
}