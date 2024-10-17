#pragma once
#include "Assets/Collider.h"

class CubeCollider : public Collider
{
public:

	float width = 0;
	float height = 0;
	float length = 0;

	ObjShape* colliderShape;

	CubeCollider(Object* p, Shader* collShader, ObjShape* collShape);

	void Render(Object* parent) override;

	//bool CheckForCollision(std::vector<Object> objs) override;

	bool SphereCollisions(Object* obj) override;
	bool CubeCollisions(Object* obj) override;
	bool PlaneCollisions(Object* obj) override;
	bool OBBCollisions(Object* obj) override { return false; };

};

