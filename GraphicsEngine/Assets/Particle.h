#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include <fstream>
#include <sstream> // for std::stringstream
#include "ObjShape.h"
#include "../PhysicsSystem.h"
#include "Component.h"


class Particle : public Component, public PhysicsSystem
{
public:
	glm::vec3 linearForce;
	//float mass;
	glm::vec3 acceleration;
	//glm::vec3 velocity;
	glm::vec3 scale = glm::vec3(0.1f, 0.5f, 0.1f);

	float size;

	float weight;

	float lifeLength;

	ObjShape* mesh;

	Particle(glm::vec3 _pos, float ObjMass = 1.0f);

	void ResetForce(); 
	
	void AddForce(glm::vec3 force, glm::vec3 _pos = glm::vec3(0)) override;

	void Update(float dt);

	void Render(Shader* shader);

};