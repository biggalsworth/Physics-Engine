#include "CubeCollider.h"
#include "Assets/Object.h"
#include "Assets/SphereCollider.h"
#include "Assets/PlaneCollider.h"
#include <algorithm>
#include "Assets/common.h"

//This code is adapted from an example on openGL
//https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection
//A lot was changed to fit into a 3D space instead of 2D space, but the base ideas and code guides were used

CubeCollider::CubeCollider(Object* p, Shader* collShader, ObjShape* collShape)
{
	//Component::type = collider;
	collType = colliderTypes::Cube;


	//if no scales have been set, set them to the parent by default.
	width = p->transform.scale.x;
	height = p->transform.scale.y;
	length = p->transform.scale.z;

	scale.x = width;
	scale.y = height;
	scale.z = length;

	//pos = owner->transform.pos;

	colliderShader = collShader;
	colliderShape = collShape;
}

void CubeCollider::Render(Object* parent)
{
	//pos = owner->transform.pos; //position is calculated in its parent class
	rot = glm::vec3(0);
	
	scale = glm::vec3(width, height, length);

	colliderShader->use();
	colliderShader->changeTransform(pos, scale, rot);
	colliderShape->Draw();
}


bool CubeCollider::SphereCollisions(Object* obj)
{
	Collider* coll = obj->GetComponent<Collider>();

	// Get the center point of the sphere
	glm::vec3 spherePos = coll->pos;

	// Calculate distances from cube center to its edges
	glm::vec3 collDistances(width, height, length);
	glm::vec3 cubeCentre = pos;

	// Calculate the closest point on the cube to the sphere
	glm::vec3 difference = spherePos - cubeCentre;
	glm::vec3 clamped = glm::clamp(difference, -collDistances, collDistances);
	glm::vec3 closest = cubeCentre + clamped;

	float xOverlap = scale.x + coll->collisionDist - difference.x;
	float yOverlap = scale.y + coll->collisionDist - difference.y;
	float zOverlap = scale.z + coll->collisionDist - difference.z;

	// by finding the mininum overlap we can find what is the axis it is closest to colliding with, finding which direction we are colliding in
	float minOverlap = std::min({ xOverlap, yOverlap, zOverlap });


	// collision normal
	collisionData.collNorm = -glm::normalize(spherePos - closest);
	coll->collisionData.collNorm = glm::normalize(spherePos - closest);

	// Calculate collision point on sphere and cube
	//coll->collisionData.collPoint = spherePos - (collisionData.collNorm * coll->collisionDist);
	coll->collisionData.collPoint = closest;
	collisionData.collPoint = closest;

	// Calculate distance squared between sphere center and closest point on cube
	float distanceSquared = glm::dot(difference, difference);

	float penetrationDepth = glm::length(spherePos - closest) - coll->collisionDist;

	// Calculate penetration depth
	//coll->collisionData.depth = coll->collisionDist - sqrt(distanceSquared);
	//collisionData.depth = coll->collisionData.depth;

	//float penetrationDepth = -minOverlap;
	collisionData.depth = penetrationDepth;

	coll->collisionData.depth = penetrationDepth;


	if (penetrationDepth < 0.0f && (!coll->trigger && !trigger))
	{
		//add force away from the normal
		glm::vec3 impulse = 200.0f*(collisionData.collNorm * (penetrationDepth));
		//obj->GetComponent<Rigidbody>()->AddForce(impulse);
		
		//obj->GetComponent<Rigidbody>()->pos += coll->collisionData.collNorm * penetrationDepth;
	}

	// Check if collision occurred
	float dist = glm::distance(closest, spherePos);
	if (dist < coll->collisionDist)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CubeCollider::CubeCollisions(Object* obj)
{

	CubeCollider* tempColl = obj->GetComponent<CubeCollider>(collider);

	//define variables
	scale = glm::vec3(width, height, length);
	tempColl->scale = glm::vec3(tempColl->width, tempColl->height, tempColl->length);

	//half extents seem to cause a problem so I use the full extents
	glm::vec3 halfExtents1 = scale;
	glm::vec3 halfExtents2 = tempColl->scale;

	// define the centers of both AABBs
	glm::vec3 centre1 = pos;
	glm::vec3 centre2 = tempColl->pos;

	// distance between the centers of the two AABBs
	glm::vec3 distance = centre2 - centre1;
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
		normal.x = (pos.x < tempColl->pos.x) ? -1.0f : 1.0f;
	}
	else if (yOverlap < xOverlap && yOverlap < zOverlap) {
		normal.y = (pos.y < tempColl->pos.y) ? -1.0f : 1.0f;
	}
	else {
		normal.z = (pos.z < tempColl->pos.z) ? -1.0f : 1.0f;
	}

	// Calculate penetration depth
	// if the penetration depth is ever more than -0.01, we are colliding;
	float penetrationDepth = std::max(0.0f, minOverlap + 0.05f); // the minOverlap has an offset to increase colliding distance

	// Set collision data
	collisionData.collNorm = glm::normalize(normal);
	collisionData.depth = penetrationDepth;
	collisionData.collPoint = pos + (collisionData.collNorm * scale / 2.0f);

	tempColl->collisionData.collNorm = glm::normalize(normal);
	tempColl->collisionData.depth = penetrationDepth;
	tempColl->collisionData.collPoint = pos + (collisionData.collNorm * scale / 2.0f);

	// Check if objects are intersecting
	if (penetrationDepth > 0.0f)
	{
		return true;
	}
	else {
		return false;
	}

	//old calculation
	/*
	// Calculate the direction of overlap along each axis
	float xOverlap = std::min(pos.x + width  / 2.0f, tempColl->pos.x + tempColl->width  / 2.0f) - std::max(pos.x - width  / 2.0f, tempColl->pos.x - tempColl->width  / 2.0f );
	float yOverlap = std::min(pos.y + height / 2.0f, tempColl->pos.y + tempColl->height / 2.0f) - std::max(pos.y - height / 2.0f, tempColl->pos.y - tempColl->height / 2.0f );
	float zOverlap = std::min(pos.z + length / 2.0f, tempColl->pos.z + tempColl->length / 2.0f) - std::max(pos.z - length / 2.0f, tempColl->pos.z - tempColl->length / 2.0f );

	// Determine the axis with the smallest overlap (normal direction)
	glm::vec3 normal = glm::vec3(0);
	glm::vec3 displacement = glm::vec3(0);
	float minOverlap = std::min({ xOverlap, yOverlap, zOverlap });

	if (xOverlap < yOverlap && xOverlap < zOverlap) {
		normal.x = (pos.x < tempColl->pos.x) ? -1.0f : 1.0f;
	}
	else if (yOverlap < xOverlap && yOverlap < zOverlap) {
		normal.y = (pos.y < tempColl->pos.y) ? -1.0f : 1.0f;
	}
	else {
		normal.z = (pos.z < tempColl->pos.z) ? -1.0f : 1.0f;
	}

	// Determine the closest point on the face of the collider
	glm::vec3 closestPoint;
	if (normal.x != 0) {
		closestPoint.x = (normal.x > 0) ? tempColl->pos.x + tempColl->width : tempColl->pos.x - tempColl->width;
		closestPoint.y = glm::clamp(pos.y, tempColl->pos.y - tempColl->height, tempColl->pos.y + tempColl->height);
		closestPoint.z = glm::clamp(pos.z, tempColl->pos.z - tempColl->length, tempColl->pos.z + tempColl->length);
	}
	else if (normal.y != 0) {
		closestPoint.x = glm::clamp(pos.x, tempColl->pos.x - tempColl->width, tempColl->pos.x + tempColl->width);
		closestPoint.y = (normal.y > 0) ? tempColl->pos.y + tempColl->height : tempColl->pos.y - tempColl->height;
		closestPoint.z = glm::clamp(pos.z, tempColl->pos.z - tempColl->length, tempColl->pos.z + tempColl->length);
	}
	else {
		closestPoint.x = glm::clamp(pos.x, tempColl->pos.x - tempColl->width, tempColl->pos.x + tempColl->width);
		closestPoint.y = glm::clamp(pos.y, tempColl->pos.y - tempColl->height, tempColl->pos.y + tempColl->height);
		closestPoint.z = (normal.z > 0) ? tempColl->pos.z + tempColl->length : tempColl->pos.z - tempColl->length;
	}
	

	float penetrationDepth = -minOverlap;
	//penetrationDepth = glm::dot(normal, closestPoint);
	collisionData.collNorm = glm::normalize(normal);
	collisionData.collPoint = closestPoint;
	collisionData.depth = penetrationDepth;


	tempColl->collisionData.collNorm = glm::normalize(normal);
	tempColl->collisionData.collPoint = closestPoint;
	tempColl->collisionData.depth = penetrationDepth;

	if (
		(pos.x - width ) <= (tempColl->pos.x + tempColl->width) &&
		(pos.x + width ) >= (tempColl->pos.x - tempColl->width) &&
		(pos.y - height) <= (tempColl->pos.y + tempColl->height) &&
		(pos.y + height) >= (tempColl->pos.y - tempColl->height) &&
		(pos.z - length) <= (tempColl->pos.z + tempColl->length) &&
		(pos.z + length) >= (tempColl->pos.z - tempColl->length)
		) 
	{
		// Adjust object positions to resolve penetration
		if (collisionData.depth < -0.001f && (!tempColl->trigger && !trigger))
		{
			//normal = pos - collisionData.collPoint;
			// Calculate impulse vectors
			glm::vec3 impulse = abs(collisionData.collNorm * collisionData.depth);

			if (abs(impulse).y > 0.0f)
			{
 				std::cout << glm::to_string(abs(impulse)) << std::endl;
			}
			if (abs(impulse).x > 0.0f)
			{
 				std::cout << glm::to_string(abs(impulse)) << std::endl;
			}

			// make sure the object is moved back to the correct position
			//obj->GetComponent<Rigidbody>()->AddForce(impulse);
			// 
			//obj->GetComponent<Rigidbody>()->pos += impulse;

		}



		return true;
	}
	else
	{
		return false;
	}
	*/
}

bool CubeCollider::PlaneCollisions(Object* obj)
{

	PlaneCollider* coll = obj->GetComponent<PlaneCollider>();
	if (coll == nullptr) {
		return false; // No collision component found, exit
	}

	//Get the plane position and normal
	glm::vec3 planePos = coll->pos;
	glm::vec3 planeNormal = coll->normal;

	// Calculate the AABB center and half extents
	glm::vec3 aabbCenter = pos; // Assuming pos is the center of the AABB
	//glm::vec3 aabbHalfExtents(width / 2.0f, height / 2.0f, length / 2.0f);
	glm::vec3 aabbHalfExtents(width , height , length );

	// compute the projection interval radius
	// from the centre, what are the two max values that would appear as you add the halfextents partially to each axis
	// if the normal is 1,1,0 and the cube pos is 0,0,0 with all half extents are 3,3,3
	// 3*1 + 3*1 + 3*0
	//these will produce a value that is the highest on each axis that the extents can possibly reach
	float r =	aabbHalfExtents.x	*	abs(planeNormal.x) +
				aabbHalfExtents.y	*	abs(planeNormal.y) +
				aabbHalfExtents.z	*	abs(planeNormal.z);

	// distance of box center from plane
	float s = glm::dot(planeNormal, aabbCenter) - glm::dot(planeNormal, planePos);
	//float s = glm::dot(planeNormal, pos) - glm::length(coll->pos);

	//glm::vec3 clamped = glm::clamp(difference, -aabbHalfExtents, aabbHalfExtents);
	collisionData.depth = (r - s);
	collisionData.collPoint = pos - planeNormal * s;
	collisionData.collNorm = planeNormal;

	coll->collisionData.depth = (r - s);
	coll->collisionData.collPoint = pos - planeNormal * s;
	coll->collisionData.collNorm = -planeNormal;

	// is the distance between the box centre and plane less than or equal to extents that the cube can reach
	if (abs(s) <= r)
	{
		//if (collisionData.depth < -0.001f && (!coll->trigger && !trigger))

		return true; // Collision occurred and resolved
	}

	return false; // No collision or penetration

	//
	// ---OPTIMISATION---
	// 
	//old data calculations

	/*

	PlaneCollider* coll = obj->GetComponent<PlaneCollider>(collider);
	// get the plane location
	glm::vec3 planePos(coll->pos.x, coll->pos.y, coll->pos.z);
	//glm::vec3 planePos(coll->pos);

	//this is how far each side of the cube is from the centre
	glm::vec3 collDistances(width, height, length);
	glm::vec3 cubeCentre = pos;

	// get difference vector between both centers
	glm::vec3 difference = planePos - cubeCentre;
	glm::vec3 clamped = glm::clamp(difference, -collDistances, collDistances);

	//find the vertex closest to the plane
	glm::vec3 closest = cubeCentre + clamped;

	coll->collisionData.collNorm = coll->normal;
	collisionData.collNorm = coll->normal;

	float distanceSquared = glm::dot(difference, difference);

	//coll->collisionData.depth = coll->collisionDist - sqrt(distanceSquared);
	//collisionData.depth = coll->collisionData.depth;


	float penetrationDepth = glm::dot(coll->normal, closest);// - 0.01f;
	//float penetrationDepth = glm::dot(coll->normal, closest);




	float dot = glm::dot(coll->normal, closest);// -coll->collisionDist;
	float leng = glm::length(difference);

	if (dot <= 0.01f)
	{
		if (penetrationDepth < -0.001f && (!coll->trigger && !trigger) && (owner->GetComponent<Rigidbody>() != NULL))
		{
			//add force away from the normal
			glm::vec3 impulse = (-coll->normal * penetrationDepth);
			//owner->GetComponent<PhysicsSystem>()->AddForce(impulse);
			owner->GetComponent<PhysicsSystem>()->pos += (impulse);
			//obj->GetComponent<Rigidbody>()->pos += coll->collisionData.collNorm * penetrationDepth;
		}

		return true;
	}

	return false;
	*/
}
