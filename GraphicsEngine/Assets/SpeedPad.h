#pragma once
#include "Component.h"

class Collider;

enum directions 
{
	up,
	forward
};

class SpeedPad : public Component
{
public:
	Collider* collider;

	float speedBoost = 50.0f;
	directions direction = up;
	float speedDuration = 2.0f;
	double enteredTime;
	bool boosted = false;

	Object* spedObject;

	SpeedPad(Collider* _collider);
	
	void Update(float dt) override;
};