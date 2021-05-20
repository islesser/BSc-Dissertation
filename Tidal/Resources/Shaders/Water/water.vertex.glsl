#version 410 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec3 inNormal;

uniform float dt;
uniform float time;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform samplerBuffer THM;
uniform samplerBuffer HM;

uniform float waterlevel;
uniform float shorethreshold;
uniform float foamHeight;
uniform int sceneWidth;

out vec3 fragmentPosition;
out vec4 passColour;
out vec3 passNormal;

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = 1.57079632679489661923;
const float PI_4 = 0.785398163397448309616;
const float PI180 = float(PI / 180.0);

uniform vec4 wave0;
uniform vec4 wave1;
uniform vec4 wave2;

uniform float min;
uniform float max;

const int HMX = 61;
const int HMZ = 121;
const int HMRES = 5;
const float floorlevel = 40.0f;

float Map(float value, float min1, float max1, float min2, float max2);
int HMIndex(int x, int z);
vec3 GerstnerWave (vec4 wave, vec3 p, float factor, float ndepth, inout vec3 tangent, inout vec3 binormal);

// GPU...
void main()
{
    gl_PointSize = 20;

    float heightSample = texelFetch(THM, HMIndex(int(inPosition.x), int(inPosition.z))).x;
    float depth = waterlevel - heightSample;
    float normalizeddepth = Map(depth, 0.0, waterlevel, 0.0, 1.0);
    float depthfactor = Map(depth, 0.0, waterlevel, max, min);

    vec3 p = inPosition;
    vec3 tangent = vec3(1.0, 0.0, 0.0);
    vec3 binormal = vec3(0.0, 0.0, 1.0);

    p += GerstnerWave(wave0, inPosition, depthfactor, normalizeddepth, tangent, binormal);
    p += GerstnerWave(wave1, inPosition, depthfactor, normalizeddepth, tangent, binormal);
    p += GerstnerWave(wave2, inPosition, depthfactor, normalizeddepth, tangent, binormal);

    vec4 worldPosition = model * vec4(p, 1.0);
    float sceneHeightSample = texelFetch(HM, HMIndex(int(p.x), int(p.z))).x;

    gl_Position = projection * view * worldPosition;

    fragmentPosition = worldPosition.xyz;
    float col = 1 - Map(p.y, 50.0, 100.0, 1.0, Map(depth, 0.0, waterlevel, 1.0, 0.0));
    passColour = vec4(col, col, col, Map(waterlevel - sceneHeightSample, 0.0, waterlevel, 0.5, 1.0));

    if (depth <= 0.0) passColour.a = 0.0;
    if ((p.y - sceneHeightSample) < shorethreshold || p.y >= foamHeight)
        passColour = vec4(1.0, 1.0, 1.0, Map(waterlevel - sceneHeightSample, 0.0, waterlevel, 0.8, 1.0));

    passNormal = normalize(cross(binormal, tangent));
}

// CPU...
//void main()
//{
//    gl_PointSize = 20;
//    float heightSample = texelFetch(THM, HMIndex(int(inPosition.x), int(inPosition.z))).x;
//    float depth = waterlevel - heightSample;
//    float normalizeddepth = Map(depth, 0.0, waterlevel, 0.0, 1.0);
//    float depthfactor = Map(depth, 0.0, waterlevel, max, min);
//
//    vec4 worldPosition = model * vec4(p, 1.0);
//    float sceneHeightSample = texelFetch(HM, HMIndex(int(p.x), int(p.z))).x;
//
//    gl_Position = projection * view * worldPosition;
//
//    fragmentPosition = worldPosition.xyz;
//    float col = 1 - Map(p.y, 50.0, 100.0, 1.0, Map(depth, 0.0, waterlevel, 1.0, 0.0));
//    passColour = vec4(col, col, col, Map(waterlevel - sceneHeightSample, 0.0, waterlevel, 0.5, 1.0));
//
//    if (depth <= 0.0) passColour.a = 0.0;
//    if ((p.y - sceneHeightSample) < shorethreshold || p.y >= foamHeight)
//    passColour = vec4(1.0, 1.0, 1.0, Map(waterlevel - sceneHeightSample, 0.0, waterlevel, 0.8, 1.0));
//
//    passNormal = inNormal;
//}

float Map(float value, float min1, float max1, float min2, float max2)
{
    return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

int HMIndex(int x, int z)
{
    return z * sceneWidth + x;
}

vec3 GerstnerWave (vec4 wave, vec3 p, float factor, float ndepth, inout vec3 tangent, inout vec3 binormal)
{
    float steepness = wave.z;
    float wavelength = wave.w * factor;

    float k = 2 * PI / wavelength; // Wavelength
    float c = sqrt(9.8 / k); // Phase speed
    vec2 d = normalize(wave.xy); // Normalized wave direction
    float f = k * (dot(d, p.xz) - c * time); // Wave slope
    float a = steepness / k; // Amplitude

    tangent += vec3(
        -d.x * d.x * (steepness * sin(f)),
        d.x * (steepness * cos(f)),
        -d.x * d.y * (steepness * sin(f))
    );

    binormal += vec3(
        -d.x * d.y * (steepness * sin(f)),
        d.y * (steepness * cos(f)),
        -d.y * d.y * (steepness * sin(f))
    );

return vec3(
    d.x * (a * cos(f)),
    a * sin(f),
    d.y * (a * cos(f)) + (ndepth * sin(time))
);
}