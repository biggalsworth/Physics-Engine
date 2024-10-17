#version 330 core
        
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColour;
layout(location = 2) in vec3 norm;

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


void main()
{
    gl_Position = ProjectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
    //gl_Position = transform * vec4(vertexPosition, 1.0);

    TotalColour = objectColour;
    FragPos = vec3(modelMatrix * vec4(vertexPosition, 1.0));
    mat3 normMatrix = mat3(transpose(inverse(modelMatrix)));
    fragNorm = norm * normMatrix;
}

/*
    const char* fragmentShaderCode =
    {
        "#version 330 core\n \
        \n \
        layout(location = 0) out vec4 fragmentColour; \n \
        \n \
        void main()\n \
        {\n \
        \n \
            fragmentColour = vec4(0.45, 0.55, 0.7, 1.0);\n \
        \n \
        } \
        "
    };

    const char* fragmentShaderCodeOther =
    {
        "#version 330 core\n \
        \n \
        layout(location = 0) out vec4 fragmentColour; \n \
        \n \
        void main()\n \
        {\n \
        \n \
            fragmentColour = vec4(1.0, 0.0, 0.0, 1.0);\n \
        \n \
        } \
        "
    };*/