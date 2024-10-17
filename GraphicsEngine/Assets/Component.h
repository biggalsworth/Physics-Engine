#pragma once
#include "common.h"

class Object;

enum ComponentTypes 
{
	mesh,
	rigidbody,
	collider,
	sphereCollider,
	planeCollider,
	particleSystem,
	particle,
	script,
	follow,
	BLANKTYPE
};

class Component
{
public:
	Object* owner;
	ComponentTypes type;
	bool active = true;

	Component();
	virtual ~Component() { };
	virtual void Update(float dt) { };

	//virtual void Draw() = 0;
	virtual void Draw(Object* parent) { };
};

