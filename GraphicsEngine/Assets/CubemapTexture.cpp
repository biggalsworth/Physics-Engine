#pragma once
#include "stb/stb_image.h"
#include <sstream> // for std::stringstream
#include <fstream>
#include <iostream>
#include <vector>
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>
#include "common.h"
#include "Shader.h"
#include "Textures/CubemapTexture.h"



CubemapT::CubemapT(std::vector<const char*> faces, Shader shader)
{
    //stbi_set_flip_vertically_on_load(true);
    unsigned int textureID;
    glGenTextures(1, &textureID);
    location = textureID - 1;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (int i = 0; i < faces.size(); i++)
    {
        stbi_set_flip_vertically_on_load(false);
        unsigned char* imgData = stbi_load(faces[i], &width, &height, &channels, 3);
        if (imgData)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
            stbi_image_free(imgData);
        }
        else 
        {
            std::cout << "Failed to load texture" << std::endl;
        }
    }
    //setting the texture wrap mode
    /* wrap modes
    https://docs.worldviz.com/vizard/latest/Texture_wrapping.htm#:~:text=Another%20OpenGL%20feature%20of%20textures%20is%20setting%20the,must%20set%20it%20for%20each%20texture%20axis%20individually.
    */
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    //setting the texture filtering mode
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    programID = shader.programId;
}

void CubemapT::Use()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}