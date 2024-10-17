#pragma once
#include "Mesh.h"
#include "ObjShape.h"

Mesh::Mesh(ObjShape* data, Shader* _shader)
{
	type = ComponentTypes::mesh;

	mesh = data;
	shader = _shader;

}

void Mesh::Draw(Object* parent)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	meshPos = owner->transform.pos;
	meshScale = owner->transform.scale;
	meshRot = owner->transform.rotation;

	shader->use();
	Rigidbody* rTemp = parent->GetComponent<Rigidbody>(rigidbody);
	if (parent->GetComponent<Rigidbody>(rigidbody) != nullptr)
	{
		shader->changeTransform(meshPos, meshScale, meshRot);
	}
	else
	{
		shader->changeTransform(meshPos, meshScale, meshRot);
	}
	mesh->Draw();
}

