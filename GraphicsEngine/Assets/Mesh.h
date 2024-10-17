#pragma once
#include "common.h"
#include "Component.h"
#include "Object.h"

class Mesh : public Component
{

public:
	glm::vec3 meshPos = glm::vec3(0);
	glm::vec3 meshRot = glm::vec3(0, 0, 0);
	glm::vec3 meshScale = glm::vec3(1);

	ObjShape* mesh;
	Shader* shader;


	Mesh(ObjShape* Data, Shader* _shader);

	//void Update(float dt) override;

	void Draw(Object* parent) override;
};

