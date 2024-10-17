#pragma once
#include "common.h"
#include "Component.h"
#include "../PhysicsSystem.h"

class Rigidbody : public Component, public PhysicsSystem
{
public:
	Object* parent;

	glm::vec3 linearForce;

	glm::vec3 torque;
	glm::mat3 inertia;

	//height, width, length
	float h = 0.5f;
	//smaller the width and greater the pivot distance, quicker the turn
	float w = 0.1f;
	float l = 1.0f;

	//float bounce = 1.0f; //coefficient

	//glm::vec3 velocity;

	glm::vec3 orientation = glm::vec3(0, 0, 0);
	glm::vec3 heading = glm::vec3(0, 0, 1); 

	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 Right = glm::vec3(1, 0, 0);;
	glm::vec3 WorldUp = glm::vec3(0, 1, 0);
	float yaw;

	Rigidbody(Object* parent, float ObjMass);

	void ResetTotalForces();
	void ResetLinearForce();

	/// <summary>
	/// Apply force in a direction
	/// </summary>
	/// <param name="force">How much force to apply and what direction</param>
	void AddForce(glm::vec3 force, glm::vec3 _pos = glm::vec3(0)) override;

	void Update(float dt);

	void UpdateVectors();

	glm::vec3 GetRight();
};