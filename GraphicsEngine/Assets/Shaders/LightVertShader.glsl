#version 330 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColour;
layout(location = 2) in vec3 norm;
layout(location = 3) in vec2 texture_cords;

uniform vec3 objectColour = vec3(0.89, 0.00, 1.00);
uniform vec3 lightColour;

out vec3 LightingColour;
       
uniform vec3 offset;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 transform;

out vec4 fragmentColour;
out vec3 fragNorm;
out vec3 FragPos;

out vec3 TotalColour;

out vec2 textCord;


void main()
{
    gl_Position = ProjectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
    //gl_Position = transform * vec4(vertexPosition, 1.0);

    TotalColour = objectColour;
    FragPos = vec3(modelMatrix * vec4(vertexPosition, 1.0));
    mat3 normMatrix = mat3(transpose(inverse(modelMatrix)));
    fragNorm = normMatrix * norm;

    textCord = texture_cords;
}