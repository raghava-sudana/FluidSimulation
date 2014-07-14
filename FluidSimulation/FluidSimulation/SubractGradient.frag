out vec2 FragColor;

uniform sampler2D Velocity;
uniform sampler2D Pressure;
uniform sampler2D Boundaries;
uniform float GradientScale;

void main()
{
    ivec2 T = ivec2(gl_FragCoord.xy);

    vec3 oC = texelFetch(Boundaries, T, 0).xyz;
    if (oC.x > 0) {
        FragColor = oC.yz;
        return;
    }

    // Find neighboring pressure:
    float pN = texelFetchOffset(Pressure, T, 0, ivec2(0, 1)).r;
    float pS = texelFetchOffset(Pressure, T, 0, ivec2(0, -1)).r;
    float pE = texelFetchOffset(Pressure, T, 0, ivec2(1, 0)).r;
    float pW = texelFetchOffset(Pressure, T, 0, ivec2(-1, 0)).r;
    float pC = texelFetch(Pressure, T, 0).r;

    // Find Boundaries:
    vec3 bN = texelFetchOffset(Boundaries, T, 0, ivec2(0, 1)).xyz;
    vec3 bS = texelFetchOffset(Boundaries, T, 0, ivec2(0, -1)).xyz;
    vec3 bE = texelFetchOffset(Boundaries, T, 0, ivec2(1, 0)).xyz;
    vec3 bW = texelFetchOffset(Boundaries, T, 0, ivec2(-1, 0)).xyz;

    // Use center pressure for solid cells:
    vec2 boundV = vec2(0);
    vec2 vMask = vec2(1);

    if (bN.x > 0) { pN = pC; boundV.y = bN.z; vMask.y = 0; }
    if (bS.x > 0) { pS = pC; boundV.y = bS.z; vMask.y = 0; }
    if (bE.x > 0) { pE = pC; boundV.x = bE.y; vMask.x = 0; }
    if (bW.x > 0) { pW = pC; boundV.x = bW.y; vMask.x = 0; }

    // Enforce the free-slip boundary condition:
    vec2 oldV = texelFetch(Velocity, T, 0).xy;
    vec2 grad = vec2(pE - pW, pN - pS) * GradientScale;
    vec2 newV = oldV - grad;
    FragColor = (vMask * newV) + boundV;  
}