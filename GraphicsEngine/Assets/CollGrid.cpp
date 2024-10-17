#include "CollGrid.h"


void Unit::Move(Unit* unit)
{
    grid->Move(unit, unit->collider->transform.pos);
}

void Grid::Add(Unit* unit)
{
    // Determine which grid cell it's in.
    int cellX = (int)((unit->x + offset.x) / CELL_SIZE);
    int cellY = (int)((unit->y + offset.y) / CELL_SIZE);
    int cellZ = (int)((unit->z + offset.z) / CELL_SIZE);

    // Add to the front of list for the cell it's in.
    unit->prev = nullptr;
    unit->next = cells[cellX][cellY][cellZ];
    if (unit->next != nullptr) {
        unit->next->prev = unit;
    }
    cells[cellX][cellY][cellZ] = unit;
}

void Grid::handleClash()
{
    for (int x = 0; x < NUM_CELLS; x++)
    {
        for (int y = 0; y < NUM_CELLS; y++)
        {
            for (int z = 0; z < NUM_CELLS; z++)
            {
                if (cells[x][y][z] != NULL) 
                {
                    handleCell(x, y, z);
                }
            }
        }
    }
}

void Grid::handleCell(int x, int y, int z)
{
    Unit* unit = cells[x][y][z];
    while (unit != NULL)
    {
        // Handle other units in this cell.
        //This is the narrow phase, it is detecting for any other item in the cell so only items that are close will check their collisions
        if (unit->next != NULL)
        {
            handleUnit(unit, unit->next);
        }

        // Also try the neighboring cells.
        if (x > 0 && y > 0 && z > 0) handleUnit(unit, cells[x - 1][y - 1][z - 1]);
        if (x > 0) handleUnit(unit, cells[x - 1][y][z]);
        if (y > 0) handleUnit(unit, cells[x][y - 1][z]);
        if (z > 0) handleUnit(unit, cells[x][y][z - 1]);
       
        

        if (x < NUM_CELLS - 1 && y > 0 && z > 0)
        {
            handleUnit(unit, cells[x + 1][y - 1][z - 1]);
        }
        if (x > 0 && y < NUM_CELLS - 1 && z > 0)
        {
            handleUnit(unit, cells[x - 1][y + 1][z -1]);
        }

        if (x > 0 && y > 0 && z < NUM_CELLS - 1)
        {
            handleUnit(unit, cells[x - 1][y - 1][z + 1]);
        }
        unit->Move(unit);
        unit = unit->next;
    }
}
void Grid::handleUnit(Unit* unit, Unit* other)
{
    while (other != NULL)
    {
        if (unit != other)
        {
            if (unit->collider->GetComponent<Collider>(collider)->CheckForCollision(other->collider) == true)
            {
                handleResponse(unit->collider, other->collider);

                
                if (other->collider->GetComponent<Collider>(collider)->CheckForCollision(unit->collider) == true)
                {
                    //Dont need to check collision twice
                    //handleResponse(other->collider, unit->collider);
                }
            }
        }
        other->Move(other);
        other = other->next;
    }
}


void Grid::handleResponse(Object* unit, Object* other)
{
    Collider* tempColl = unit->GetComponent<Collider>(collider);
    Collider* otherColl = other->GetComponent<Collider>(collider);
    PhysicsSystem* Rb = unit->GetComponent<PhysicsSystem>();
    PhysicsSystem* objRb = other->GetComponent<PhysicsSystem>();

    try {
        if (tempColl->trigger || otherColl->trigger)
            return;

        if (Rb == nullptr || objRb == nullptr)
            throw 505;

        // Calculate collision normal
        glm::vec3 collisionNormal = -glm::normalize(tempColl->collisionData.collNorm);

        // Calculate relative velocity
        glm::vec3 relativeVelocity = objRb->velocity - Rb->velocity;
        float velAlongNormal = glm::dot(relativeVelocity, collisionNormal);

        // Check if objects are moving towards each other
        if (velAlongNormal < 0) {


            #pragma region Penetration Resolving
            //if object is inside another object, move it out.
            if (tempColl->collisionData.depth < -0.001f && Rb->movable)
            {
                // make sure position increases by how much we penetrated by
                glm::vec3 impulse = abs(tempColl->collisionData.depth) * glm::normalize(tempColl->collisionData.collNorm);

                //Rb->AddForce(impulse);
                Rb->pos += impulse;

            }
            else if (otherColl->collisionData.depth < -0.001f && objRb->movable) 
            {
                // make sure position increases by how much we penetrated by
                glm::vec3 impulse = abs(otherColl->collisionData.depth) * glm::normalize(otherColl->collisionData.collNorm);

                objRb->pos += impulse;
            }
            #pragma endregion



            // Calculate impulse based on velocity and mass
            float j = -(1.0f + Rb->bounce) * velAlongNormal /
                ((1.0f / Rb->mass) + (1.0f / objRb->mass));

            // Update velocities
            if (Rb->movable)
                Rb->velocity -= ((j / Rb->mass) * collisionNormal);

            if (objRb->movable)
                objRb->velocity += ((j / objRb->mass) * collisionNormal);

            // Calculate angular velocity change
            Rigidbody* rigidUnit = unit->GetComponent<Rigidbody>();
            Rigidbody* rigidOther = other->GetComponent<Rigidbody>();

            //if angular rotation is on an axis it shouldn't be, change it in CalculateCollisionTorque

            if (rigidUnit != nullptr) {
                glm::vec3 relativeAngularVelocity = objRb->angularVelocity - rigidUnit->angularVelocity;
                glm::vec3 torque = CalculateCollisionTorque(relativeAngularVelocity, *tempColl, Rb, objRb) * 10.0f;
                glm::vec3 angularAccel;

                if (Rb->movable) {
                    angularAccel = torque / rigidUnit->inertia * tempColl->deltaTime;
                    rigidUnit->angularVelocity -= angularAccel;
                }
            }

            if (rigidOther != nullptr) {
                glm::vec3 relativeAngularVelocity = Rb->angularVelocity - rigidOther->angularVelocity;
                glm::vec3 torque = CalculateCollisionTorque(relativeAngularVelocity, *otherColl, objRb, Rb) * 10.0f;
                glm::vec3 angularAccel;

                if (rigidOther->movable) {
                    //objRb->angularVelocity += (torque / rigidOther->inertia) * otherColl->deltaTime;
                    angularAccel = torque / rigidOther->inertia * otherColl->deltaTime;
                    rigidOther->angularVelocity += angularAccel;
                }
            }
        }
    }
    catch (...) {
        // Handle exceptions
    }
}

glm::vec3 Grid::CalculateCollisionTorque(const glm::vec3& relativeAngularVelocity, Collider& collider, const PhysicsSystem* Rb, const PhysicsSystem* objRb)
{
    Rigidbody* rigid = collider.owner->GetComponent<Rigidbody>();
    
    //change the first glm::vec3 of this rotate to change what axis you rotate on
    glm::vec3 RotationAxis = glm::normalize(glm::rotate(glm::vec3(0, 0, 1), glm::radians(rigid->orientation.y), glm::vec3(0, 1, 0)));

    // perpendicular distance for objRb
    glm::vec3 perpendicularDistanceObj;
    if (objRb->angularVelocity != glm::vec3(0.0f)) {
        perpendicularDistanceObj = glm::cross(collider.collisionData.collPoint - objRb->massCentre, RotationAxis);
    }
    else {
        perpendicularDistanceObj = glm::vec3(0.0f);
    }

    // perpendicular distance for Rb
    glm::vec3 perpendicularDistanceRb;
    if (Rb->angularVelocity != glm::vec3(0.0f)) 
    {
        perpendicularDistanceRb = glm::cross(collider.collisionData.collPoint - Rb->massCentre, RotationAxis);
    }
    else {
        perpendicularDistanceRb = glm::vec3(0.0f);
    }

    glm::vec3 relativeVelocity = objRb->velocity - Rb->velocity;
    // calculate impulse
    float relativeSpeedAlongNormal = glm::dot(relativeVelocity, collider.collisionData.collNorm);
    float impulse = 
        -(1 + Rb->bounce) * relativeSpeedAlongNormal 
        / 
        (1 / Rb->mass + 1 / objRb->mass);


    // make the total torque a combination of torques from both objects
    //glm::vec3 torqueObj = glm::cross(perpendicularDistanceObj, impulse * RotationAxis);
    glm::vec3 torqueRb = glm::cross(perpendicularDistanceRb, impulse * RotationAxis);
    //glm::vec3 totalTorque = torqueObj + torqueRb;

    return torqueRb;
}


glm::vec3 calculateAngularVelocity(const glm::vec3& vab, const glm::vec3& n, float epsilon, float ma, float mb,
    const glm::vec3& ra, const glm::vec3& rb, const glm::mat3& Ia_inv,
    const glm::mat3& Ib_inv) {
    //  -(1 + epsilon) * (vab dot n)
    float coeff1 = -(1 + epsilon) * glm::dot(vab, n);

    //  (ra x n) and (rb x n)
    glm::vec3 ra_cross_n = glm::cross(ra, n);
    glm::vec3 rb_cross_n = glm::cross(rb, n);

    //  (I^-1_a * (ra x n)) x ra
    glm::vec3 term1 = glm::cross(Ia_inv * ra_cross_n, ra);

    //  (I^-1_b * (rb x n)) x rb
    glm::vec3 term2 = glm::cross(Ib_inv * rb_cross_n, rb);

    // calculate the final angular velocity
    glm::vec3 angular_velocity = coeff1 * n * (1.0f / ma + 1.0f / mb) + term1 + term2;

    return angular_velocity;
}

void Grid::Move(Unit* unit, glm::vec3 pos)
{
    // See which cell it was in.
    int oldCellX = (int)((unit->x + offset.x) / Grid::CELL_SIZE);
    int oldCellY = (int)((unit->y + offset.y) / Grid::CELL_SIZE);
    int oldCellZ = (int)((unit->z + offset.z) / Grid::CELL_SIZE);

    // See which cell it's moving to.
    int cellX = (int)((pos.x + offset.x) / Grid::CELL_SIZE);
    int cellY = (int)((pos.y + offset.y) / Grid::CELL_SIZE);
    int cellZ = (int)((pos.z + offset.z) / Grid::CELL_SIZE);

    // Only move the unit if it's actually changing cells.
    if (oldCellX != cellX || oldCellY != cellY || oldCellZ != cellZ) 
    {
        // Unlink it from the list of its old cell.
        if (unit->prev != nullptr) {
            unit->prev->next = unit->next;
        }
        if (unit->next != nullptr) {
            unit->next->prev = unit->prev;
        }

        // If it's the head of a list, remove it.
        if (cells[oldCellX][oldCellY][oldCellZ] == unit) {
            cells[oldCellX][oldCellY][oldCellZ] = unit->next;
        }

        // Add it back to the grid at its new cell.
        unit->x = unit->collider->transform.pos.x;
        unit->y = unit->collider->transform.pos.y;
        unit->z = unit->collider->transform.pos.z;
        Add(unit);
    }
}
