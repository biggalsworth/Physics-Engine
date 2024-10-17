#pragma once
#include "Collider.h"
class PlaneCollider : public Collider
{
public:

	Object* parent;

	//Shader* colliderShader;
	ObjShape* colliderShape;

	float size;

	glm::vec3 normal;

	//Mesh(std::vector<VertexData> Data, Shader* _shader);
	PlaneCollider(Object* p, Shader* collShader, ObjShape* collShape);

	void Render(Object* parent) override;

	//bool CheckForCollision(std::vector<Object> objs) override;

	bool SphereCollisions(Object* obj) override;
	bool CubeCollisions(Object* obj) override;
	bool PlaneCollisions(Object* obj) override;
	bool OBBCollisions(Object* obj) override { return false; };
};

