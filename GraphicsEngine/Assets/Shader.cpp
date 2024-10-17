#pragma once
#include "Shader.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include <fstream>
#include <sstream>
#include "common.h"
#include "Rigidbody.h"
#include "Object.h"
#include "Camera.h"

Shader::Shader(const char* vertPath, const char* fragPath) 
{
    // 1. retrieve the vertex/fragment source code from filePath
    const char* vertShaderCode;
    const char* fragShaderCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vertShaderCode = textFileRead(vertPath);

        fragShaderCode = textFileRead(fragPath);

    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    // 2. compile shaders
    //const GLuint* vertex;
    //const GLuint* fragment;
    // vertex shader
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertShaderCode, NULL);
    glCompileShader(vertexShaderId);
    CheckCompileErrors(vertexShaderId);

    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragShaderCode, NULL);
    glCompileShader(fragmentShaderId);
    CheckCompileErrors(fragmentShaderId);

    // shader Program
    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);

};