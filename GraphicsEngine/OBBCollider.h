#pragma once
#include "Assets/Collider.h"
#include "Assets/Object.h"
#include <glm/gtx/norm.hpp>

class OBBCollider : public Collider
{
public:
	float width = 0;
	float height = 0;
	float length = 0;

	glm::vec3* centre;
	glm::mat4 transform;
	glm::mat3 axes;

	ObjShape* colliderShape;

	OBBCollider(Object* p, Shader* collShader, ObjShape* collShape);

	void Render(Object* parent) override;

	//bool CheckForCollision(std::vector<Object> objs) override;

	bool OBBCollisions(Object* obj) override;
	bool SphereCollisions(Object* obj) override;
	bool CubeCollisions(Object* obj) override;
	bool PlaneCollisions(Object* obj) override;

	void UpdateAxis() 
	{
		glm::mat3 rotationMatrix =
			glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(owner->transform.rotation.x), glm::vec3(1, 0, 0)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(owner->transform.rotation.y), glm::vec3(0, 1, 0)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(owner->transform.rotation.z), glm::vec3(0, 0, 1)));

		glm::mat4 test(1.0f);
		glm::rotate(test, glm::radians(owner->transform.rotation.x), glm::vec3(1, 0, 0));
		glm::rotate(test, glm::radians(owner->transform.rotation.y), glm::vec3(0, 1, 0));
		glm::rotate(test, glm::radians(owner->transform.rotation.z), glm::vec3(0, 0, 1));

		// Construct the transformation matrix
 		transform = rotationMatrix;

		// Assuming transform is the transformation matrix of the OBB
		axes[0] = glm::vec3(transform[0]); // Extract the first row of the matrix as the X axis
		axes[1] = glm::vec3(transform[1]); // Extract the second row of the matrix as the Y axis
		axes[2] = glm::vec3(transform[2]); // Extract the third row of the matrix as the Z axis

		// Make sure to normalize the axes if needed
		axes[0] = glm::normalize(axes[0]);
		axes[1] = glm::normalize(axes[1]);
		axes[2] = glm::normalize(axes[2]);

	}

	void FindCollisionPoint(OBBCollider* otherObb) 
	{
		// Find the edges of both OBBs
		std::vector<glm::vec3> edges1 = {
			axes[0] * width, 
			axes[1] * height, 
			axes[2] * length
		};

		std::vector<glm::vec3> edges2 = {
			otherObb->axes[0] * otherObb->width,
			otherObb->axes[1] * otherObb->height,
			otherObb->axes[2] * otherObb->length
		};
		int i = 0;
		for (glm::vec3 edge1 : edges1) {
			for (glm::vec3 edge2 : edges2) {
				// Calculate the direction and distance between the closest points of the two edges
				glm::vec3 d = glm::cross(edge1, edge2);
				glm::vec3 dist = *otherObb->centre - *centre;

				// are edges parallel
				// glm::length2, (, T const &, x, ). Returns the squared length of x. 
				if (glm::length2(d) < 1e-8f)
					continue;

				// points of intersection
				float p1 = glm::dot(glm::cross(dist, edge2), d) / glm::length2(d);
				float p2 = glm::dot(glm::cross(dist, edge1), d) / glm::length2(d);

				glm::vec3 intersectionPoint = *centre + edge1 * p1 - *otherObb->centre - edge2 * p2;

				// is point within both edges
				float edge1Length = glm::length(edge1);
				float edge2Length = glm::length(edge2);


				if (p1 >= 0 && p1 <= edge1Length && p2 >= 0 && p2 <= edge2Length) 
				{

					// The edges intersect, so update collision data
					collisionData.collNorm = glm::normalize(d);
					collisionData.collPoint = intersectionPoint; 
					
					otherObb->collisionData.collNorm = -glm::normalize(d);
					otherObb->collisionData.collPoint = intersectionPoint;

					/*collisionData.depth = glm::length(*otherObb->centre - intersectionPoint) - 0.001f;

					otherObb->collisionData.collPoint = intersectionPoint;
					otherObb->collisionData.depth = glm::length(*otherObb->centre - intersectionPoint) - 0.001f;
					otherObb->collisionData.collNorm = -glm::normalize(d);*/
				}

			}

		}
	}
};

