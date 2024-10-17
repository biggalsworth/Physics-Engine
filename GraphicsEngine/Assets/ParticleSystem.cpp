#include "ParticleSystem.h"
#include "Particle.h"
#include "common.h"
#include "Component.h"
#include <random>

ParticleSystem::ParticleSystem(glm::vec3 _pos, ObjShape* _mesh, Shader* _shader, int _size, int _amount, float _duration, float _lifetime, float _mass)
{
	type = ComponentTypes::particleSystem;

	mass = _mass;
	pos = _pos;
	size = _size;
	amount = _amount;
	duration = _duration;
	lifetime = _lifetime;

	mesh = _mesh;
	shader = _shader;
}

void ParticleSystem::Update(float dt)
{
	if (!enabled) 
	{
		return;
	}
	//random number generation
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(-size, size); // define the range
	
	oldTime += dt;

	if (particles.size() == amount && !looping)
	{
		oldTime = 0.0f;
		//currAmount = 0.0f;
		delete this;
	}

	if (oldTime > (duration / amount))
	{
		srand((unsigned)time(NULL));

		glm::vec3 tempPos = { distr(gen) + pos.x, (pos.y), distr(gen) + pos.z };
		Particle* part = new Particle(tempPos, mass);
		part->mesh = mesh;
		part->scale = scale;

		particles.push_back(part);
	}

	for (Particle* part : particles)
	{
		part->AddForce(speed);
		part->Update(dt);
	}
}

void ParticleSystem::Draw(Object* parent)
{
	particleMat->Use();
	for (Particle* part : particles)
	{
		if (part->lifeLength >= duration)
		{
			particles.erase(std::remove(particles.begin(), particles.end(), part), particles.end());
			delete part;
		}
		else 
		{
			part->mesh = mesh;
			part->Render(shader);
		}
	}
}
