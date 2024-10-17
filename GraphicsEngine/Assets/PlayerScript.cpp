#include "PlayerScript.h"
#include "Component.h"
#include "Object.h"

PlayerScript::PlayerScript(Rigidbody* rigidBody) 
{
	if (rigidBody != nullptr) 
	{
		rb = rigidBody;
	}
}

void PlayerScript::Update(float dt)
{
    if (!active) 
    {
        return;
    }
    rb = owner->GetComponent<Rigidbody>();
    rb->gravity = CheckGrounded();
    #pragma region Movement Control
        if (rb != nullptr)
        {
            if (Object::keyInput[GLFW_KEY_W] == true)
            {
                rb->AddForce(rb->heading * moveSpeed);

            }
            if (Object::keyInput[GLFW_KEY_S] == true)
            {
                rb->AddForce(-rb->heading * moveSpeed);
            }
            if (Object::keyInput[GLFW_KEY_D] == true)
            {
                rb->AddForce(rb->GetRight() * (turnSpeed),
                    glm::rotate({ -130, 0, 1 }, glm::radians(rb->orientation.y), glm::vec3(0, 1, 0)));
            }
            if (Object::keyInput[GLFW_KEY_A] == true)
            {
                rb->AddForce(-rb->GetRight() * (turnSpeed),
                    glm::rotate({ 130, 0, 1 }, glm::radians(rb->orientation.y), glm::vec3(0, 1, 0)));

            }
            else if (Object::keyInput[GLFW_KEY_S] == false && Object::keyInput[GLFW_KEY_W] == false && Object::keyInput[GLFW_KEY_A] == false && Object::keyInput[GLFW_KEY_D] == false)
            {
                rb->ResetTotalForces();
            }
            
            if (Object::keyInput[GLFW_KEY_SPACE] == true && !jumped)
            {
                rb->AddForce(rb->WorldUp * 500.0f);
                jumped = true;
            }

            //owner->transform.pos = rb->pos;
            //owner->transform.rotation = rb->orientation;

        }
    #pragma endregion


    //FindObject("Tracker")->transform.pos = owner->GetComponent<Collider>()->collisionData.collPoint;

}

bool PlayerScript::CheckGrounded()
{
    /*
    if (owner->GetComponent<Collider>()->PlaneCollisions(FindObjectWithTag(Ground)))
    {
        return false;
    }
    return true;
    */

    
    for (Object* obj : owner->GetComponent<Collider>(ComponentTypes::collider)->collidedObjs)
    {
        if (obj->tag == Ground)
        {
            jumped = false;
            if (rb->gravity)
            {
                //rb->velocity.y = 0.0f;
            }
            return false;
        }
    }
    return true;
    
    
}
