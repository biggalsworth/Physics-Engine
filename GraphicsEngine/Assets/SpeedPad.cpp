#include "SpeedPad.h"
#include "Collider.h"
#include "Object.h"
#include "Component.h"
#include "common.h"

SpeedPad::SpeedPad(Collider* _collider) 
{
	type = ComponentTypes::script;
	collider = _collider;
}

void SpeedPad::Update(float dt)
{
	if (owner->GetComponent<Collider>()->collidedObjs.size() > 0)
	{
		for (Object* obj : owner->GetComponent<Collider>()->collidedObjs)
		{
			if (obj->tag == Player && !boosted)
			{
				//obj->moveSpeed += speedBoost;
				//obj->GetScript<PlayerScript>()->moveSpeed += speedBoost;
				
				if (direction == up) 
				{
					obj->GetComponent<Rigidbody>()->velocity.y = 0;
					obj->GetComponent<Rigidbody>()->velocity.y = (1 * speedBoost);
				}
				if (direction == forward) 
				{
					FindObjectWithTag(Tags::Player)->GetScript<PlayerScript>()->moveSpeed += speedBoost;

					//obj->GetComponent<PhysicsSystem>()->AddForce(obj->GetComponent<Rigidbody>()->heading * speedBoost);
				}
				
				
				enteredTime = glfwGetTime();
				spedObject = obj;
				boosted = true;
				std::cout << "zoom" << std::endl;
				return;
			}
		}
		
		if ((glfwGetTime() - enteredTime) >= speedDuration && boosted) 
		{
			if (direction == forward)
			{
				FindObjectWithTag(Tags::Player)->GetScript<PlayerScript>()->moveSpeed -= speedBoost;
			}
			//FindObjectWithTag(Tags::Player)->moveSpeed -= speedBoost;
			std::cout << "boost ended" << std::endl;
			boosted = false;
		}
		
	}
}