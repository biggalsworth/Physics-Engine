#pragma once
#include "common.h"
#include "Objshape.h"

class ObjShape;

class LightObj
{
public:
    // mesh data
    std::vector<VertexData> shapeData;
    const char* fileN = "";
    float currAngle = 0;
    glm::vec3 objColour;
    glm::vec3 lightcolour = { 0.75f, 0.75f, 0.75f };
    glm::vec3 targColour = { 0.75f, 0.75f, 0.75f };
    bool changeColour = true;
    glm::mat4 transform = glm::mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    //std::vector<VertexData> texture;

    LightObj(const char* vData)
    {
        this->fileN = "Assets/imageData/CubeImageInf.png";
        //this->shapeData = LoadOBJ(vData);
        ConfigObjIntoShape();
    }
    void Draw()
    {
        // draw mesh
        glBindVertexArray(vao);
        //glDrawElements(GL_TRIANGLES, shapeData.size(), GL_UNSIGNED_INT, NULL);
        glDrawArrays(GL_TRIANGLES, 0, shapeData.size());
        //glBindVertexArray(0);
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
    unsigned int vao, vbo;

    void ConfigObjIntoShape()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        //glGenBuffers(1, &ibo);
        //Bind the buffer/vertex array to the vao to allow the shader to use it.
        glBindVertexArray(vao);


        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, shapeData.size() * sizeof(VertexData), shapeData.data(), GL_STATIC_DRAW);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        //colour
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // vertex normals
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)(7 * sizeof(float)));
        glEnableVertexAttribArray(2);
        // vertex texture coords
        //glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, colour));
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)(10 * sizeof(float)));
        glEnableVertexAttribArray(3);

#pragma endregion

    }

};