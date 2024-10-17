#pragma once
#include "SphereCollider.h"
#include "Textures/Texture.h"
#include "Object.h"
#include "PlaneCollider.h"
#include <glm/gtx/norm.hpp> 

SphereCollider::SphereCollider(Object* p, Shader* collShader, ObjShape* collShape)
{
	//Component::type = collider;
	collType = colliderTypes::Sphere;

	//owner = p;

	radius = collisionDist;

	scale = glm::vec3(collisionDist);
	//pos = owner->transform.pos;

	colliderShader = collShader;
	colliderShape = collShape;
}


void SphereCollider::Render(Object* p)
{
	pos = originalPos + owner->transform.pos;
	rot = Component::owner->transform.rotation;
	scale = glm::vec3(collisionDist);

	colliderShader->use();
 	colliderShader->changeTransform(pos, scale, rot);
	colliderShape->Draw();

	//CheckForCollision(radius);
}

bool SphereCollider::SphereCollisions(Object* obj)
{
	SphereCollider* coll = obj->GetComponent<SphereCollider>(collider);

	glm::vec3 collisionDirection = glm::normalize(pos - coll->pos);

	collisionData.collPoint = pos + (collisionDirection * collisionDist);
	coll->collisionData.collPoint = coll->pos - (collisionDirection * collisionDist);

	collisionData.collNorm = glm::normalize(pos - coll->pos);
	coll->collisionData.collNorm = -glm::normalize(pos - coll->pos);

	//coll->collisionData.collNorm = glm::normalize(coll->pos - pos);

	float dist = glm::distance(coll->pos, pos);
 	if (dist < (coll->collisionDist + collisionDist))
	{
 		return true;
	}
	else
	{
		return false;
	}
}

bool SphereCollider::CubeCollisions(Object* obj)
{
	CubeCollider* coll = obj->GetComponent<CubeCollider>(collider);
	bool collided = coll->SphereCollisions(this->owner);
	if (collided) 
	{
		return true;
	}
	else 
	{
		return false;
	}
}

bool SphereCollider::PlaneCollisions(Object* obj)
{
	PlaneCollider* coll = obj->GetComponent<PlaneCollider>(collider);

	glm::vec3 pointBetweenObjs = pos - coll->pos;

	float dist = glm::dot(pointBetweenObjs, coll->normal);

	collisionData.collNorm = coll->normal;
	collisionData.collPoint = pos - (collisionData.collNorm * radius);
	collisionData.depth = radius - dist;

	coll->collisionData.collNorm = -coll->normal;
	coll->collisionData.collPoint = pos - (collisionData.collNorm * radius);
	coll->collisionData.depth = radius - dist;


	//if (dist < collisionDist + coll->collisionDist)
	if (collisionData.depth > -0.001)
	{
		return true;
	}

	return false;
}


