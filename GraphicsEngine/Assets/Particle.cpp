#include "Particle.h"
#include "Shader.h"
#include "Object.h"

Particle::Particle(glm::vec3 _pos, float ObjMass)
{
	owner = NULL;

	linearForce = glm::vec3(0);
	acceleration = glm::vec3(0);
	velocity = glm::vec3(0);

	mass = ObjMass / 10.0f;
	pos = _pos;
}

void Particle::ResetForce()
{
	linearForce = glm::vec3(0);
}


void Particle::AddForce(glm::vec3 force, glm::vec3 _pos)
{
	linearForce += force;
}

void Particle::Update(float dt)
{
	if (owner != NULL) 
	{
		pos = owner->transform.pos;
		massCentre = pos;
	}

	//ResetForce();

	//weight = mass * 9.8;

	acceleration = linearForce / mass;
	if (gravity) 
	{
		acceleration.y -= weight;
	}

	pos = pos + (velocity)*dt;

	velocity = velocity + (acceleration)*dt;

	velocity *= pow(drag, dt);

	lifeLength += dt;
}

void Particle::Render(Shader* shader) 
{
	shader->use();
	shader->changeTransform(pos, scale, glm::vec3(0));
	//shader->transform = glm::inverse(glm::lookAt(currPos, cam.position, glm::vec3(0, 1, 0)));
	//unsigned int transformLoc = glGetUniformLocation(shader->programId, "modelMatrix");
	//glUniformMatrix4fv(transformLoc, 1, GL_TRUE, glm::value_ptr(shader->transform));
	shader->transform;
	mesh->Draw();
}

