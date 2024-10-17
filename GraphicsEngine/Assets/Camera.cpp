#pragma once
#include "Camera.h"
#include "glm/gtx/transform.hpp"

//my camera is based off LearnOpengl's camera system, I changed the way some of these functions worked and made personal changes
//https://learnopengl.com/Getting-started/Camera
Camera::Camera(glm::vec3 Position, glm::vec3 Up, float Yaw, float Pitch, glm::vec3 orient, float speed, float sense, float zoomSpeed)
{
    orientation = orient;
    moveSpeed = speed*0.2f;
    lookSens = sense;
    zoom = zoomSpeed;

    position = glm::vec3(Position.x, Position.y, Position.z);
    WorldUp = Up;
    yaw = Yaw;
    pitch = Pitch;

    updateCameraVectors();
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{

    //set position to 1 so then the lookAt function will work correctly
    glm::mat4 view = glm::mat4(1.0f);
    /*
    * when i had a static camera *
    glm::vec3 forwardDir = glm::normalize(position - front);
    glm::vec3 side = glm::normalize(glm::cross(WorldUp, forwardDir));
    glm::vec3 newUp = glm::normalize(glm::cross(forwardDir, side));
    //glm::mat4 view = glm::lookAt(position, position + front, Up);
    /*view = {
        side.x, side.y, side.z, -glm::dot(side, position),
        newUp.x, newUp.y, newUp.z, -glm::dot(newUp, position),
        forwardDir.x, forwardDir.y, forwardDir.z, -glm::dot(forwardDir, position),
        0, 0, 0, 1 };*/
    if(target != glm::vec3(0)) {
        view = glm::lookAt(position, lookAtTarget, WorldUp);
    }
    else
    {
        view = glm::lookAt(position, position + orientation, up);

    }
    return view;
}


void Camera::Input(int key)
{
    char inputChar = char(key);
    //use to track positions of where to place objects or to fix object positions
    if (inputChar == 'T') 
    {
        std::cout << "X: "+ std::to_string(position.x) << std::endl;
        std::cout << "Y: "+ std::to_string(position.y) << std::endl;
        std::cout << "Z: "+ std::to_string(position.z) << std::endl;
    }
    //movement controls
    if (inputChar == 'D')
    {
        position += Right * moveSpeed;
    }
    if (inputChar == 'A')
    {
        position -= Right * moveSpeed;
    }
    if (inputChar == 'W')
    {
        position += orientation * moveSpeed;
    }
    if (inputChar == 'S')
    {
        position -= orientation * moveSpeed;
    }
    if (inputChar == ' ')
    {
        position += up * moveSpeed;
    }

    //release the cursor
    //\0 is escape
    if (inputChar == '\0')
    {
        glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    //quick travel positions
    //centre
    if (inputChar == '1') 
    {
        position = glm::vec3(0, 2, 5);
    }
    //temple
    if (inputChar == '2') 
    {
        position = glm::vec3(2, 2, -5);

    }
    //temple upper
    if (inputChar == '3') 
    {
        position = glm::vec3(0, 10, -2);
    }
    //pond
    if (inputChar == '4') 
    {
        position = glm::vec3(-3, 2, 5);
    }

    //updateCameraVectors();
}

void Camera::MouseInput(double mouseX, double mouseY) 
{
    mouseX *= lookSens;
    mouseY *= lookSens;

    yaw += mouseX;
    pitch += mouseY;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 85.0f)
        pitch = 85.0f;
    if (pitch < -85.0f)
        pitch = -85.0f;

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::Follow(Object* targ)
{
    if (targ->GetComponent<Rigidbody>() == nullptr)
    {
        return;
    }

    Rigidbody* rb = targ->GetComponent<Rigidbody>();

    position = rb->pos - (rb->heading * 5.0f);

    position.y += 2.0f;
    lookAtTarget = rb->pos + (rb->heading * 5.0f);

    target = rb->pos;

    //position = target;
}


void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    //explanation
    /*
    
    Think of the view as a triangle.
    imagine the hypotenuse is always 1 as if we the target for where to look is 1 space in front of us.
    our starting point will be 0,0,0 as that is where the camera is in local space (ignore if we are moving, that is what normalisation fixes)
    to get the adjacent side, it will now be cos *angle
    to get the opposite side it will now be sin *angle
    because anything/1 is anything 
    so for example sin *yaw* = sin opposite face / hypotenuse = sin ?/1 = sin ?, this will give us the length of the x and y sides on a trangle.

    if we imagine a triangle with the z going up and x going across
    the distance of how far we have moved on the x axis will be our adjacent face and the distance of how far up the z axis we are is the opposite face
    to get the x axis it will now be cos of how far right or left we are looking (the yaw), so cos(yaw), so now we have the x position of our target to look at.
    and the z will be sin of yaw which gets us the z position of where we are looking at
    this will get us the x and z axis, but thats only 2D, we want 3D, so we have to do the y.

    now imagine a y axis triangle, x/z axis are the bottom face (the adjacent) and the opposite face will be how far up we have looked on the y axis
    this is sin of the angle that we have looked up by (the pitch), that is our y coordinates/length

    this means that cos*pitch* gives us something relating the x/y as that is perpendicular to the y axis, the adjacent / bottom face of our imaginary triangle
    
    this means that the x and z need to be in relation to this point of the adjacent.
    so we multiply the radians of the yaw by the cos of the pitch to get where this looking point is in relation the y axis.

    so sin or cos of the yaw * cos(pitch) gives us the x and y positions in relation to the y so it is all lined up

    */
    glm::vec3 Front;
    //get where we are looking on the x axis relative to how left or right we are looking
    //multiply by the pitch cos to get it to be inline with the pitch looking position
    Front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    //finding the height of where we are looking at
    Front.y = sin(glm::radians(pitch));
    //same as x axis but for the z
    Front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    //normalize it to the world space
    orientation = glm::normalize(Front);

    // also re-calculate the Right and Up vector
    //the right would be the perpendicular line to the current orientation of my camera and the up vector
    Right = glm::normalize(glm::cross(orientation, WorldUp));
    up = glm::normalize(glm::cross(Right, orientation));
    //these are normalised, so as their length changes due to looking up and down, the speed doesn't change
}