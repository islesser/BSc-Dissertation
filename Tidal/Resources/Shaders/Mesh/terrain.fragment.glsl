#version 410 core

in vec3 fragmentPosition;
in vec4 passColour;
in vec3 passNormal;

out vec4 FragColor;

uniform float AmbientStrength;
uniform float SpecularStrength;
uniform vec3 LightPosition;

uniform vec3 cameraPosition;

const float HEIGHT = 200.0f;
uniform vec4 Tint;
uniform vec4 c1;
uniform float t1;

uniform vec4 c2;
uniform float t2;

uniform vec4 c3;
uniform float t3;

uniform vec4 c4;
uniform float t4;

uniform vec4 c5;
uniform float t5;

uniform vec4 c6;
uniform float t6;

uniform vec4 def;

vec4 GetColourFromHeight();

void main()
{
    // AMBIENT
    vec3 Ambient = AmbientStrength * vec3(1.0, 1.0, 1.0);

    // DIFFUSE
    vec3 nnormal = normalize(passNormal);
    vec3 lightDirection = normalize(LightPosition - fragmentPosition);
    float diff = max(dot(nnormal, lightDirection), 0.0);
    vec3 Diffuse = diff *  vec3(1.0, 1.0, 1.0);

    // Specular
    vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, nnormal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec3 Specular = 0.1 * spec * vec3(1.0, 1.0, 1.0);;

    vec4 Lighting = vec4(Ambient + Diffuse + Specular, 1.0f);

    vec4 COLOUR = GetColourFromHeight();

    FragColor = Lighting * (passColour * Tint);;
}

vec4 GetColourFromHeight()
{
    if (fragmentPosition.y < t1 * HEIGHT)
    {
        return c1;
    }
    else if (fragmentPosition.y < t2 * HEIGHT)
    {
        return c2;
    }
    else if (fragmentPosition.y < t3 * HEIGHT)
    {
        return c3;
    }
    else if (fragmentPosition.y < t4 * HEIGHT)
    {
        return c4;
    }
    else if (fragmentPosition.y < t5 * HEIGHT)
    {
        return c5;
    }

    return def;
}