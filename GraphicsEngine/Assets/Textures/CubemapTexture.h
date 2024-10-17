#pragma once

class CubemapT
{
public:
    int width;
    int height;
    int channels;
    GLuint textureID;
    GLuint location;
    int programID;

    CubemapT(std::vector<const char*> faces, Shader shader);

    void Use();
};