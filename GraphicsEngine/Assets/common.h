#pragma once
#define GLM_SWIZZLE
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
#include "../WorldManager.h"

class Object;
class Shader;
class Texture;

struct VertexData{

    glm::vec3 pos;
    glm::vec4 colour;
    glm::vec3 norm;
    glm::vec2 textCord;

};

struct BasicVData{

    glm::vec3 pos;
    glm::vec4 colour;
    glm::vec3 norm;

};

class Shape
{
public:
    // mesh data
    std::vector<VertexData> shapeData;
    std::vector<unsigned int> indices;
    float currAngle = 0;
    float currOffset = 0;
    glm::vec3 objColour;
    glm::vec3 lightcolour = { 0.75f, 0.75f, 0.75f };
    glm::vec3 targColour = { 0.75f, 0.75f, 0.75f };
    bool changeColour = true;
    //std::vector<VertexData> texture;

    Shape(std::vector<VertexData> vData, std::vector<unsigned int> indices)
    {
        this->shapeData = vData;
        this->indices = indices;
        //this->texture = textures;
        ConfigShape();
    }
    void Draw()
    {
        // draw mesh
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 12*3);
        glBindVertexArray(0);
    }

    void ConfigLightSource()
    {
        //add light vao
        unsigned int lightCubeVAO;
        glGenVertexArrays(1, &lightCubeVAO);
        glBindVertexArray(lightCubeVAO);

        // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)0);

        glBindVertexArray(0);
    }

private:
    //  render data
    unsigned int vao, vbo, ibo;

    void ConfigShape()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ibo);
        //Bind the buffer/vertex array to the vao to allow the shader to use it.
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, shapeData.size() * sizeof(VertexData), &shapeData[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // vertex normals
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)(7 * sizeof(float)));
        glEnableVertexAttribArray(2);
        // vertex texture coords
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)(10 * sizeof(float)));
        glEnableVertexAttribArray(3);


    }

};

class ShapeFloats
{
public:
    // mesh data
    std::vector<float> shapeData;
    std::vector<unsigned int> indices;
    //std::vector<VertexData> texture;
    //std::vector<VertexData> texture;

    ShapeFloats(std::vector<float> vData, std::vector<unsigned int> indices)
    {
        this->shapeData = vData;
        this->indices = indices;
        ConfigShape();
    }


    void Draw()
    {
        // draw mesh
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 12*3);
        glBindVertexArray(0);
    }

private:
    //  render data
    unsigned int vao, vbo, ibo;
    
    void ConfigShape()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ibo);
        //Bind the buffer/vertex array to the vao to allow the shader to use it.
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, shapeData.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
        glEnableVertexAttribArray(0);


    }

};


