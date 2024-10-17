#pragma once
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Texture
{
public:
    int width;
    int height;
    int channels;
    GLuint textureID;
    GLuint normalID;
    bool normal;
    GLuint location;
    int programID;

    Texture(const char* filePath, Shader shader, const char* normalFile = "");
    
    void Use();

};