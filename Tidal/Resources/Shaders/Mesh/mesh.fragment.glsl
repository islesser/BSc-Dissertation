#version 410 core

in vec3 fragmentPosition;
in vec4 passColour;
in vec3 passNormal;

out vec4 FragColor;

uniform float AmbientStrength;
uniform float SpecularStrength;
uniform vec3 LightPosition;
uniform vec4 Tint;

uniform vec3 cameraPosition;
uniform vec3 lightPosition;

const float HEIGHT = 200.0f;

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
//    FragColor = Lighting * (passColour * Tint);
//    FragColor = passColour * Tint;
    FragColor = Lighting * Tint;
}
