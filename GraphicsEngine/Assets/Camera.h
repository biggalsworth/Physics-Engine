#pragma once
#include <iostream>
#include <vector>
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/transform.hpp"
#include "common.h"
#include "Object.h"

class Camera
{

public:
    // camera Attributes
    glm::vec3 position;
    glm::vec3 orientation = glm::vec3(0, 0, 1);
    glm::vec3 up = glm::vec3(0,1,0);
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float yaw;
    float pitch;
    
    // cam/move settings
    float moveSpeed;
    float lookSens;
    float zoom;

    glm::vec3 target = glm::vec3(0);
    glm::vec3 lookAtTarget;
    glm::vec3 cameraDirection;
    //Object followTarget;

    // constructor with vectors
    Camera(glm::vec3 Position = glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f, glm::vec3 Orintation = glm::vec3(0,0,1), float speed = 0.5f, float sense = 0.1, float zoomSpeed = 50.0f);



    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    void Input(int key);

    void MouseInput(double MouseX, double MouseY);

    void Follow(Object* targ);


private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();


};