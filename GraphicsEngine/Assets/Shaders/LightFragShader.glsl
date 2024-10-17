#version 330 core

layout(location = 0) out vec4 fragmentColour;

in vec3 LightingColour;

//uniform vec3 objColour;
uniform vec3 lightCol;
uniform vec3 lightPos = vec3(0.0, 0.0, 0.0);
uniform vec3 viewPos;

in vec3 vertColour;
in vec3 TotalColour;
in vec3 fragNorm;
in vec3 FragPos;

in vec2 textCord;

uniform sampler2D colourTexture;
uniform bool isNormal = false;
uniform sampler2D normalMap;

out vec4 finalColour;

//uniform vec3 objectColour = vec3(0.89, 0.00, 1.00);

void main()
{
    float ambientStrength = 0.1;
    float specularStrength = 0.5;
    float specRadius = 64;
    vec3 ambient = ambientStrength * lightCol;

    //diffuse
    vec3 norm = normalize(fragNorm);
    //direction of vector going from light to the fragment
    vec3 lightDirection = normalize(lightPos - FragPos);
    float diff = max(dot(fragNorm, lightDirection), 0.0);
    vec3 diffuse = diff * lightCol;

    //Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specRadius);
    vec3 specular = specularStrength * spec * lightCol;

    vec3 result = (ambient + diffuse + specular) * TotalColour;
    finalColour = texture(colourTexture, textCord);

    fragmentColour = finalColour * vec4(result, 1.0f);

}