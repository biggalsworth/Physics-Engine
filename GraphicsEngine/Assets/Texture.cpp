#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "Shader.h"
#include "Textures/Texture.h"


Texture::Texture(const char* filePath, Shader shader, const char* normalFile)
{
    stbi_set_flip_vertically_on_load(true);
    //unsigned char* imgData = stbi_load(<file path to the image>, &width, &height, &channels, 3);
    unsigned char* imgData = stbi_load(filePath, &width, &height, &channels, 3);
    unsigned char* normalData = NULL;
    if(normalFile != "")
    {
        normalData = stbi_load(normalFile, &width, &height, &channels, 3);
        normal = true;

    }
    else 
    {
        normalData = stbi_load("/Assets/imageData/BaseNormal.png", &width, &height, &channels, 3);
        //normalData = NULL;
        normal = false;
    }
    glGenTextures(1, &textureID);
    location = textureID - 1;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);

    glUniform1i(glGetUniformLocation(shader.programId, "colourTexture"), 0);
    glGenerateMipmap(GL_TEXTURE_2D);

    //setting the texture wrap mode
   /* wrap modes
   https://docs.worldviz.com/vizard/latest/Texture_wrapping.htm#:~:text=Another%20OpenGL%20feature%20of%20textures%20is%20setting%20the,must%20set%20it%20for%20each%20texture%20axis%20individually.
   */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //setting the texture filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glGenTextures(1, &normalID);
    location = normalID - 1;
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, normalData);
    

    glUniform1i(glGetUniformLocation(shader.programId, "normalMap"), 1);
    glGenerateMipmap(GL_TEXTURE_2D);

    programID = shader.programId;
}

void Texture::Use()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(programID, "colourTexture"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalID);    
    glUniform1i(glGetUniformLocation(programID, "normalMap"), 1);

    glUniform1i(glGetUniformLocation(programID, "isNormal"), normal);

}