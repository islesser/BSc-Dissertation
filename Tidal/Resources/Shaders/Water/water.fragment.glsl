#version 410 core

in vec3 fragmentPosition;
in vec4 passColour;
in vec3 passNormal;

out vec4 FragColor;

uniform samplerBuffer THM;
uniform samplerBuffer HM;
uniform float waterlevel;
uniform float shorethreshold;
uniform float foamHeight;
uniform vec4 OceanColour;
uniform vec4 Tint;
uniform float AmbientStrength;
uniform float SpecularStrength;
uniform vec3 LightPosition;

uniform vec3 cameraPosition;
uniform vec3 lightPosition;

int HMIndex();
float Map(float value, float min1, float max1, float min2, float max2) {
    return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main()
{
    float heightSample = texelFetch(THM, HMIndex()).x;

    if (heightSample > fragmentPosition.y) return;
    if (passColour.a <= 0.0) return;

    // AMBIENT
    vec3 Ambient = AmbientStrength * vec3(1.0, 1.0, 1.0);

    // DIFFUSE
    vec3 nnormal = normalize(passNormal);
    vec3 lightDirection = normalize(LightPosition - fragmentPosition);
    float diff = max(dot(nnormal, lightDirection), 0.0);
    vec3 Diffuse = diff * vec3(0.9, 0.9, 0.9);

    // SPECULAR
    vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, nnormal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec3 Specular = SpecularStrength * spec * vec3(1.0, 1.0, 1.0);

    vec4 Lighting = vec4(Ambient + Diffuse + Specular, 1.0f);

    FragColor = Lighting * (OceanColour + passColour * Tint);
}

int HMIndex()
{
    return int(fragmentPosition.z) * 400 + int(fragmentPosition.x);
}