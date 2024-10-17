#include "PlaneCollider.h"
#include "SphereCollider.h"
#include "Textures/Texture.h"
#include "Object.h"
#include "../CubeCollider.h"

PlaneCollider::PlaneCollider(Object* p, Shader* collShader, ObjShape* collShape)
{

	//Component::type = collider;
	collType = colliderTypes::Plane;
	normal = glm::vec3(0.0f, 1.0f, 0.0f);

	//owner = p;

	scale = glm::vec3(1, 0, 1);
	pos = p->transform.pos;

	colliderShader = collShader;
	colliderShape = collShape;

	collisionNormal = normal;
}


void PlaneCollider::Render(Object* p)
{
	pos = originalPos + owner->transform.pos;
	rot = owner->transform.rotation;
	scale = glm::vec3(size, 0, size);

	//colliderShader->use();
	//colliderShader->changeTransform(pos, scale, rot);
	//colliderShape->Draw();

	//CheckForCollision(radius);
}

bool PlaneCollider::SphereCollisions(Object* obj)
{
	SphereCollider* coll = obj->GetComponent<SphereCollider>(collider);
	
	glm::vec3 pointBetweenObjs = pos - coll->pos;

	float dist = glm::dot(pointBetweenObjs, normal);

	if (dist < coll->collisionDist + collisionDist)
	{
		collisionData.collNorm = -normal;

		return true;
	}
	else
	{
		return false;
	}
	
}

bool PlaneCollider::CubeCollisions(Object* obj)
{
	CubeCollider* coll = obj->GetComponent<CubeCollider>(collider);
	return coll->PlaneCollisions(owner);
	/*
	// get the plane location
	glm::vec3 planePos(pos.x, coll->pos.y, pos.z);
	//glm::vec3 planePos(coll->pos);

	//this is how far each side of the cube is from the centre
	glm::vec3 collDistances(width, height, length);
	glm::vec3 cubeCentre = pos;

	// get difference vector between both centers
	glm::vec3 difference = planePos - cubeCentre;
	glm::vec3 clamped = glm::clamp(difference, -collDistances, collDistances);

	//find the vertex closest to the plane
	glm::vec3 closest = cubeCentre + clamped;

	//get the dot product of the closest point and the normal
	difference = closest - coll->normal;
	float dot = glm::dot(coll->normal, closest);// -coll->collisionDist;
	float leng = glm::length(difference);

	if (dot <= coll->collisionDist)
	{
		return true;
	}

	return false;
	PlaneCollider* coll = obj->GetComponent<PlaneCollider>(collider);

	glm::vec3 pointBetweenObjs = pos - coll->pos;

	float dist = glm::dot(pointBetweenObjs, normal);

	if (dist < coll->collisionDist + collisionDist)
	{
		return true;
	}

	return false;*/
}

bool PlaneCollider::PlaneCollisions(Object* obj)
{
	PlaneCollider* coll = obj->GetComponent<PlaneCollider>(collider);

	glm::vec3 pointBetweenObjs = pos - coll->pos;

	float dist = glm::dot(pointBetweenObjs, normal);

	if (dist < coll->collisionDist + collisionDist)
	{
		return true;
	}

	return false;
}



