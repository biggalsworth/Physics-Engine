#pragma once
#include "Component.h"
#include "Rigidbody.h"
#include "ParticleSystem.h"

class PlayerScript : public Component
{
public:
	Rigidbody* rb = nullptr;

	float moveSpeed = 15.0f;
	float turnSpeed = 12.0f; //turn speed is seperate so if the car is boosted, it does not affect the turn speed.
	bool grounded = false;
	bool jumped = false;

	ParticleSystem* particle = nullptr;

	PlayerScript(Rigidbody* rigidBody = nullptr);
	~PlayerScript() {};

	void Update(float dt) override;

	bool CheckGrounded();
};

