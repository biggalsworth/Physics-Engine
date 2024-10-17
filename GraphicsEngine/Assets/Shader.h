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
#include "common.h"
#include "glm/gtx/rotate_vector.hpp"

class Shader
{
public:
    glm::mat4 transform;
    glm::vec3 currPos;
    GLuint programId;

    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertPath, const char* fragPath);

    //build the cam matrices
    void buildCam(glm::mat4 modMat, glm::mat4 viewMat, glm::mat4 projMat)
    {
        glUseProgram(programId);
        GLuint modelMatrixLocation = glGetUniformLocation(programId, "modelMatrix");
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_TRUE, glm::value_ptr(modMat));

        GLuint viewMatrixLocation = glGetUniformLocation(programId, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMat));

        GLuint projectionMatrixLocation = glGetUniformLocation(programId, "ProjectionMatrix");
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projMat));

    };


    void changeScale(glm::mat4 trans)
    {
        unsigned int transformLoc = glGetUniformLocation(programId, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_TRUE, glm::value_ptr(trans));
    }

    void changeTransform(glm::vec3 pos, glm::vec3 scale, glm::vec3 axis = glm::vec3(0))
    {
        glm::vec3 tempPos = pos;

        
        glm::vec3(90, 0, 75);
        glm::mat4 transform = glm::mat4(1.0f);
        
        transform = glm::mat4(
            scale.x , 0         , 0         , pos.x,
            0       , scale.y   , 0         , pos.y,
            0       , 0         , scale.z   , pos.z,
            0       , 0         , 0         , 1
        );
        
        //do we need to rotate
        /*   
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, pos);
        model = glm::rotate(model, glm::radians(axis.x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(axis.y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(axis.z), glm::vec3(0, 0, 1));
        model = glm::translate(model, -pos);*/

        if (axis != glm::vec3(0))
        {
            //glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::normalize(axis));
            //pos = rotationMatrix * glm::vec4(pos, 1.0f);
            /*
            transform = glm::mat4(
                scale.x, 0, 0, pos.x,
                0, scale.y, 0, pos.y,
                0, 0, scale.z, pos.z,
                0, 0, 0, 1
            );
            */
            glm::mat4 rotation = glm::mat4(1.0f); 
            //transform = glm::inverse(transform);
            //transform = glm::mat4(1.0f);
            //transform = glm::translate(transform, -pos);
            //rotation = glm::translate(rotation, {0,0,0});
            transform = glm::mat4(1.0f);

            
            transform = glm::translate(transform, pos);
            transform = glm::rotate(transform, glm::radians(axis.x), glm::vec3(1, 0, 0));
            transform = glm::rotate(transform, glm::radians(axis.y), glm::vec3(0, 1, 0));
            transform = glm::rotate(transform, glm::radians(axis.z), glm::vec3(0, 0, 1));
            transform = glm::scale(transform, scale);
           // transform = glm::translate(transform, -pos);

            //rotation = glm::translate(transform, pos);
            /*
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::translate(trans, axis);
            trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
            */
            //transform = glm::scale(transform, scale);
            //transform = glm::translate(rotation, pos);
            //transform = (transform) * rotation;
            //transform = glm::inverse(transform);

            currPos = glm::vec3(pos);
            unsigned int transformLoc = glGetUniformLocation(programId, "modelMatrix");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        }
        else 
        {
            transform = glm::mat4(
            scale.x , 0         , 0         , pos.x,
            0       , scale.y   , 0         , pos.y,
            0       , 0         , scale.z   , pos.z,
            0       , 0         , 0         , 1
            );
            currPos = glm::vec3(pos);
            unsigned int transformLoc = glGetUniformLocation(programId, "modelMatrix");
            glUniformMatrix4fv(transformLoc, 1, GL_TRUE, glm::value_ptr(transform));
        }
        
       // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
    }

    
    glm::mat4 ChangeRotation(glm::vec3 axis, float amount)
    {
        unsigned int transformLoc = glGetUniformLocation(programId, "modelMatrix");
        glUniformMatrix4fv(transformLoc, 1, GL_TRUE, glm::value_ptr(transform));

        return transform;
    }

    void LookAt(glm::vec3 currPos)
    {
        //transform = glm::inverse(glm::lookAt(currPos, cam.position, glm::vec3(0, 1, 0)));
        //std::cout << glm::to_string(cam->position) << std::endl;
        //unsigned int transformLoc = glGetUniformLocation(programId, "modelMatrix");
        //glUniformMatrix4fv(transformLoc, 1, GL_TRUE, glm::value_ptr(transform));
    }

    // activate the shader
    void use()
    {
        glUseProgram(programId);
    }
    // change shader data
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
    }
    void setVec4(const GLchar* name, float vec[])
    {
        glUniform4f(glGetUniformLocation(programId, name), vec[0], vec[1], vec[2], vec[3]);
    }
    void setVec3(const GLchar* name, glm::vec3 vec)
    {
        glUniform3f(glGetUniformLocation(programId, name), vec.r, vec.g, vec.b);
    }
    void assignVec3(const GLchar* name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(programId, name), 1, &value[0]);
    }
    void assignVec3(const GLchar* name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(programId, name), x, y, z);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void CheckCompileErrors(GLuint shader)
    {
        GLint result = GL_FALSE;
        int logLength;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
        glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]);
        std::cout << &vertShaderError[0] << std::endl;

    }

    char* textFileRead(const char* textFilepath)
    {
        FILE* filePointer;
        fopen_s(&filePointer, textFilepath, "rb"); //opens the file to read it in binary
        char* content = NULL;
        long numVal = 0;

        fseek(filePointer, 0L, SEEK_END); //go to the end of the file
        numVal = ftell(filePointer); //save the number of bytes from the start to the end of the file
        fseek(filePointer, 0L, SEEK_SET); //go to the beginning of the file
        content = (char*)malloc((numVal + 1) * sizeof(char)); //allocate enough memory know we know the byte amount
        fread(content, 1, numVal, filePointer); //read and store the contents into the block of allocated memory
        content[numVal] = '\0'; //add a null terminator at the end
        fclose(filePointer); //close the file
        return content; //pass the read contents as a char* 
    }
};