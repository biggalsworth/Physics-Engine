#pragma once
#include "Component.h"
#include "ObjShape.h"
class Common;
class Particle;

class ParticleSystem : public Component
{
public:
	std::vector<Particle*> particles;

	ObjShape* mesh;
	Shader* shader;
	Texture* particleMat;

	glm::vec3 pos = glm::vec3(0);
	glm::vec3 scale = glm::vec3(0.1f, 0.5f, 0.1f); // default scale for each particle, defaulted to rain
	float mass;
	int size;
	int amount;
	float duration;
	float lifetime;
	bool gravity = true;

	bool looping = true;

	bool enabled = true;

	glm::vec3 speed = glm::vec3(0);

	int currAmount = 0;

	//int currentTime = glfwGetTime();
	float oldTime = 0.0f;

	/// <summary>
	/// A particle system
	/// </summary>
	/// <param name="_pos: ">The position of the particle system</param>
	/// <param name="_size: ">How large the spawning distance of particles is. FROM THE CENTRE POINT</param>
	/// <param name="_amount: ">How many particles to spawn a second</param>
	/// <param name="_duration: ">How long particles should spawn for</param>
	/// <param name="_lifetime: ">How long the particles live for</param>
	/// <param name="_mass: ">Mass of each particle</param>
	ParticleSystem(glm::vec3 _pos, ObjShape* _mesh, Shader* _shader, int _size, int _amount = 10, float _duration = 5.0f, float _lifetime = 10.0f, float _mass = 1.0f);

	void Update(float dt) override;

	void Draw(Object* parent) override;


	void Stop() 
	{
		oldTime = 0;
		particles.clear();
		enabled = true;
	};
	void Start() 
	{ 
		oldTime = 0; 
		particles.clear();
		enabled = true; 
	};


};

