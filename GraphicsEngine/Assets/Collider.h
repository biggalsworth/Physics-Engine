#pragma once
#include "Component.h"
#include "common.h"
#include <list>

class ObjShape;
class Shader;
class Object;

enum colliderTypes 
{
	Sphere,
	Cube,
	Plane,
	OBB
};


struct colliderDataEnum
{
	glm::vec3 collPoint = glm::vec3(0);
	glm::vec3 collNorm = glm::vec3(0);
	float depth = 0.0f;
};

class Collider : public Component
{

public:
	Shader* colliderShader = nullptr;

	colliderTypes collType;

	colliderDataEnum collisionData;

	float collisionDist;
	float deltaTime = 0.0f;

	glm::vec3 originalPos = glm::vec3(0);
	glm::vec3 pos = glm::vec3(0);
	glm::vec3 rot = glm::vec3(0);
	glm::vec3 scale = glm::vec3(1);

	//the normal for collision response
	glm::vec3 collisionNormal = glm::vec3(0, 0, 0);

	std::vector < Object* > collidedObjs;

	bool trigger = false;
	bool colliding;
	bool render = true;

	Collider();
	virtual ~Collider() { };

	void Update(float dt) override
	{
		deltaTime = dt;
	};

	void Draw(Object* parent) override
	{
		if (render)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			Render(parent);
		}
	};

	virtual void Render(Object* parent) { };

	virtual bool CheckForCollision(Object* obj);

	virtual bool SphereCollisions(Object* objs) = 0;
	virtual bool CubeCollisions(Object* objs) = 0;
	virtual bool PlaneCollisions(Object* objs) = 0;
	virtual bool OBBCollisions(Object* objs) = 0;
};

