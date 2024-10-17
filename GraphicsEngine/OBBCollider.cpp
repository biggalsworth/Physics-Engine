#include "OBBCollider.h"
#include "Assets/Collider.h"
#include "Assets/common.h"
#include "Assets/Object.h"
#include "Assets/PlaneCollider.h"
#include "CubeCollider.h"

OBBCollider::OBBCollider(Object* p, Shader* collShader, ObjShape* collShape)
{
	collType = colliderTypes::OBB;

	//owner = p;

	/*
	width = scale.x;
	height = scale.y;
	length = scale.z;
	*/

	//if no scales have been set, set them to the parent by default.
	width = p->transform.scale.x;
	height = p->transform.scale.y;
	length = p->transform.scale.z;


	scale.x = width;
	scale.y = height;
	scale.z = length;

	centre = &p->transform.pos;

	//pos = owner->transform.pos;

	colliderShader = collShader;
	colliderShape = collShape;
}

void OBBCollider::Render(Object* parent)
{
	//pos = owner->transform.pos; //position is calculated in its parent class
	rot = owner->transform.rotation; //position is calculated in its parent class
	//rot = glm::vec3(0);

	UpdateAxis();

	scale = glm::vec3(width, height, length);

	colliderShader->use();
	colliderShader->changeTransform(pos, scale, rot);
	colliderShape->Draw();
}


bool OBBCollider::OBBCollisions(Object* obj)
{
	OBBCollider* otherObb = obj->GetComponent<OBBCollider>();
	//make sure all axis are correct.
	UpdateAxis();
	otherObb->UpdateAxis();

	//transform this collider into local space of the other collider
	glm::mat4 OtherLocal = glm::inverse(otherObb->transform);
	glm::vec3 CenterInLocalSpace = glm::vec3(OtherLocal * glm::vec4(*centre, 1.0f));

	//calculate the bounds of this collider in otherObb space
	glm::vec3 ExtentsInLocalSpace =
		glm::abs(glm::vec3(transform[0]) * width +
		glm::vec3(transform[1]) * height +
		glm::vec3(transform[2]) * length);

	// calculate the AABB of obb1 in obb2 space
	glm::vec3 obb1MinInObb2Space = CenterInLocalSpace - ExtentsInLocalSpace;
	glm::vec3 obb1MaxInObb2Space = CenterInLocalSpace + ExtentsInLocalSpace;

	// calculate AABB of other collider
	glm::vec3 otherMin = *otherObb->centre - otherObb->scale;// * 0.5f;
	glm::vec3 otherMax = *otherObb->centre + otherObb->scale;// *0.5f;
	
	//check collision normal and point
	FindCollisionPoint(otherObb);
	otherObb->FindCollisionPoint(this);

	// Check for AABB overlap
	if (obb1MaxInObb2Space.x < otherMin.x || obb1MinInObb2Space.x > otherMax.x) return false;
	if (obb1MaxInObb2Space.y < otherMin.y || obb1MinInObb2Space.y > otherMax.y) return false;
	if (obb1MaxInObb2Space.z < otherMin.z || obb1MinInObb2Space.z > otherMax.z) return false;

	//collisionData.depth = glm::length(*otherObb->centre - collisionData.collPoint) - 0.001f;

	//otherObb->collisionData.depth = glm::length(*otherObb->centre - collisionData.collPoint) - 0.001f;

	glm::vec3 halfExtents1 = scale;
	glm::vec3 halfExtents2 = otherObb->scale;

	glm::vec3 distance = *otherObb->centre - *centre;
	glm::vec3 absDistance = glm::abs(distance);

	// get the minimum and maximum distances between the two AABBs along each axis
	float xOverlap = halfExtents1.x + halfExtents2.x - absDistance.x;
	float yOverlap = halfExtents1.y + halfExtents2.y - absDistance.y;
	float zOverlap = halfExtents1.z + halfExtents2.z - absDistance.z;

	// determine the axis with the smallest overlap and the normal direction
	glm::vec3 normal = glm::vec3(0);

	// by finding the mininum overlap we can find what is the axis it is closest to colliding with, finding which direction we are colliding in
	float minOverlap = std::min({ xOverlap, yOverlap, zOverlap });

	if (xOverlap < yOverlap && xOverlap < zOverlap) {
		normal.x = (pos.x < otherObb->pos.x) ? -1.0f : 1.0f;
	}
	else if (yOverlap < xOverlap && yOverlap < zOverlap) {
		normal.y = (pos.y < otherObb->pos.y) ? -1.0f : 1.0f;
	}
	else {
		normal.z = (pos.z < otherObb->pos.z) ? -1.0f : 1.0f;
	}

	// Calculate penetration depth
	// if the penetration depth is ever more than -0.01, we are colliding;
	float penetrationDepth = std::max(0.0f, minOverlap + 0.05f); // the minOverlap has an offset to increase colliding distance

	// Set collision data
	collisionData.collNorm = glm::normalize(normal);
	collisionData.depth = penetrationDepth;
	collisionData.collPoint = pos + (collisionData.collNorm * scale / 2.0f);

	otherObb->collisionData.collNorm = glm::normalize(normal);
	otherObb->collisionData.depth = penetrationDepth;
	otherObb->collisionData.collPoint = pos + (collisionData.collNorm * scale / 2.0f);


	return true; // the new AABBs overlap, OBBs may intersect
	
}

bool OBBCollider::SphereCollisions(Object* obj)
{
	return false;
}

bool OBBCollider::CubeCollisions(Object* obj)
{
	return false;
}

bool OBBCollider::PlaneCollisions(Object* obj)
{
	UpdateAxis();

	PlaneCollider* plane = obj->GetComponent<PlaneCollider>();

	// Compute the projection interval radius of obb onto the plane normal
	float r = glm::dot(glm::abs(axes[0] * width), plane->normal) +
		glm::dot(glm::abs(axes[1] * height), plane->normal) +
		glm::dot(glm::abs(axes[2] * length), plane->normal);

	// Compute the signed distance from the OBB center to the plane
	float s = glm::dot(*centre - plane->pos, plane->normal);

	collisionData.collNorm = glm::normalize(plane->normal);
	collisionData.depth = r - s;
	collisionData.collPoint = pos - plane->normal * s;
	//collisionData.collPoint = pos + (collisionData.collNorm * scale / 2.0f);

	plane->collisionData.collNorm = -glm::normalize(plane->normal);
	plane->collisionData.depth = r - s;
	collisionData.collPoint = pos - plane->normal * s;

	//plane->collisionData.collPoint = pos + (collisionData.collNorm * scale / 2.0f);

	// Intersection occurs if the signed distance is within the projection interval radius
	return glm::abs(s) <= r;
}
