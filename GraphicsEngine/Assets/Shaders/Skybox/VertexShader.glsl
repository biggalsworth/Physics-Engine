#version 330 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColour;
layout(location = 2) in vec3 norm;
layout(location = 3) in vec2 texture_cords;

out vec3 TexCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
    TexCoords = vertexPosition;
    vec4 pos = ProjectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
    gl_Position = pos;
    //gl_Position = ProjectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}  