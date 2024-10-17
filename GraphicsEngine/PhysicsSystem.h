#pragma once
#include "Assets/common.h"
/*
#include "Assets/Particle.h"
#include "Assets/Rigidbody.h"
*/

class Rigidbody;
class Particle;

class PhysicsSystem
{
public:
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 acceleration;
	glm::vec3 angularVelocity = glm::vec3(0.0f);
	glm::vec3 angularForce;
	glm::vec3 angularAcceleration = glm::vec3(0);

	glm::vec3 massCentre;
	float bounce = 1.0f;
	float mass = 1.0f;
	float weight = this->mass * 9.8f;

	float drag = 0.55f;
	float BaseDrag;

	bool gravity = true;

	bool movable = true;

	PhysicsSystem();

	virtual void AddForce(glm::vec3 force, glm::vec3 _pos = glm::vec3(0)) = 0;
};

