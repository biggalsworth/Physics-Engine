#pragma once
#include "Collider.h"
#include "common.h"
#include "Object.h"
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "PlaneCollider.h"
#include "SphereCollider.h"

Collider::Collider()
{
	collisionData = colliderDataEnum();
	Component::type = collider;
	collidedObjs.clear();
}

bool Collider::CheckForCollision(Object* obj)
{
	pos = originalPos + owner->transform.pos;
	//pos = owner->transform.pos;
	
	//rot = rot + owner->transform.rotation;
	this->rot = owner->transform.rotation;

	//colliding = false;
	//collidedObjs.clear();
	bool collidedThisLoop = false;
	Collider* comp;

	colliding = false;
	collidedThisLoop = false;
	comp = obj->GetComponent<Collider>(collider);

	if (comp->collType == colliderTypes::Sphere)
	{
		collidedThisLoop = SphereCollisions(obj);
	}
	if (comp->collType == colliderTypes::Cube)
	{
		collidedThisLoop = CubeCollisions(obj);
	}
	if (comp->collType == colliderTypes::Plane)
	{
		collidedThisLoop = PlaneCollisions(obj);
	}
	if (comp->collType == colliderTypes::OBB)
	{
		//std::cout << "WARNING: THIS IS AN OBB COLLIDER AND WILL NOT WORK WITH COLLIDERS THAT ARE NOT OBB." << std::endl;
		//std::cout << "To use this collider change the player to OBB" << std::endl;

		collidedThisLoop = OBBCollisions(obj);
	}

	//have we collided wit*h this object in a previous loop
	for (Object* pastObj : collidedObjs) 
	{
		if (pastObj == obj)
		{
			collidedThisLoop = false;
		}
	}

	if (collidedThisLoop == true)
	{
		collidedObjs.push_back(obj);
		colliding = true;
	}

	if (collidedObjs.size() <= 0 ) 
	{
		colliding = false;
	}

	return colliding;
	
}

/*
void Collider::Update(float dt)
{
    std::cout << "Collider Update`" << std::endl;
}

void Collider::Draw()
{
    std::cout << "Collider Update" << std::endl;
}*/
