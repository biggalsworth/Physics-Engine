#pragma once
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

Rigidbody::Rigidbody(Object* p, float objMass)
{
    type = rigidbody;

    linearForce = glm::vec3(0);
    angularForce = glm::vec3(0);
	acceleration = glm::vec3(0);
	velocity = glm::vec3(0);

	mass = objMass/10.0f;

    //pos = glm::vec3(0);
    //rot = glm::vec3(0);

    angularAcceleration = glm::vec3(0);
    angularVelocity = glm::vec3(0);
    
    parent = p;
    massCentre = pos;

    BaseDrag = drag;

    orientation = { 0.0f, 0.0f, 0.0f };

    heading = glm::vec3(0, 0, 1);

    //inertia
    inertia = glm::mat3
    (
        1.0f/12*mass*(pow(h, 2) + pow(w, 2)), 0, 0,
        0, 1.0f / 12 * mass * (pow(l, 2) + pow(w, 2)), 0,
        0, 0, 1.0f / 12 * mass * (pow(h, 2) + pow(l, 2))
    );

}

void Rigidbody::ResetTotalForces()
{
	linearForce = glm::vec3(0);
    angularForce = glm::vec3(0);

    torque = glm::vec3(0);
}

void Rigidbody::AddForce(glm::vec3 force, glm::vec3 _pos)
{
    torque = glm::cross(force, _pos);

    if (_pos == glm::vec3(0)) 
    {
        linearForce += force;
    }

    angularForce += torque;
}

void Rigidbody::Update(float dt)
{
	//ResetTotalForces();
    pos = owner->transform.pos;

	acceleration = linearForce / mass;
    angularAcceleration = angularForce * glm::inverse(inertia);

    if (gravity == true)
    {
        acceleration.y -= weight;
    }

	pos = pos + (velocity) * dt;
    massCentre = pos;

    orientation = orientation + (angularVelocity)*dt;

	velocity = velocity + (acceleration)*dt;
    angularVelocity = angularVelocity + (angularAcceleration)*dt;

    if (drag <= 0.0f) 
    {
        drag = 0.01f;
    }

	velocity *= pow(drag, dt);
    angularVelocity *= pow(drag / 2.0f, dt);

    //if (orientation.y > 360.0f) orientation.y -= 360.0f;
    //if (orientation.y < 0.0f) orientation.y += 360.0f;

    heading = glm::normalize(glm::rotate(glm::vec3(0, 0, 1), glm::radians (orientation.y), glm::vec3(0, 1, 0)));

    owner->transform.pos = pos;            
    owner->transform.rotation = orientation;

    ResetTotalForces();
}



//currently not doing anything
void Rigidbody::UpdateVectors()
{
    glm::vec3 Front;
    //get where we are looking on the x axis relative to how left or right we are looking
    //multiply by the pitch cos to get it to be inline with the pitch looking position
    Front.x = cos(glm::radians(yaw)) * cos(glm::radians(0.0f));
    //finding the height of where we are looking at
    Front.y = sin(glm::radians(0.0f));
    //same as x axis but for the z
    Front.z = sin(glm::radians(yaw)) * cos(glm::radians(0.0f));
    //normalize it to the world space
    orientation = glm::normalize(Front);

    // also re-calculate the Right and Up vector
    //the right would be the perpendicular line to the current orientation of my camera and the up vector
    Right = glm::normalize(glm::cross(orientation, WorldUp));
    up = glm::normalize(glm::cross(Right, orientation));
    //these are normalised, so as their length changes due to looking up and down, the speed doesn't change
}

glm::vec3 Rigidbody::GetRight() 
{
    Right = glm::normalize(glm::cross(WorldUp, heading));
    return Right;
}
