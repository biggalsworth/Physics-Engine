#version 330 core

layout(location = 0) out vec4 fragmentColour;

in vec3 LightingColour;

//uniform vec3 objColour;
uniform vec3 lightCol;
uniform vec3 lightPos = vec3(0.0, 0.0, 0.0);

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
    vec3 norm;
    norm = normalize(fragNorm);
    finalColour = texture(colourTexture, textCord);
    fragmentColour =  finalColour * vec4(TotalColour, 1.0f);

    //fragmentColour = vec4(TotalColour, 1.0f);
}