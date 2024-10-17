#pragma once
#include "common.h";
#include "Object.h";

//this is a grid script to create spatial partioning optimision for collisions
//this script is inspired by https://gameprogrammingpatterns.com/spatial-partition.html

class Unit;

class Grid
{
public:

    Grid()
    {
        // Clear the grid.
        for (int x = 0; x < NUM_CELLS; x++)
        {
            for (int y = 0; y < NUM_CELLS; y++)
            {
                for (int z = 0; z < NUM_CELLS; z++)
                {
                    cells[x][y][z] = nullptr;
                }
            }
        }
    }

    void Add(Unit* unit);

    /// <summary>
    /// Check the grids and handle collisions within each cell
    /// </summary>
    void handleClash();

    void handleCell(int x, int y, int z);

    void handleUnit(Unit* unit, Unit* other);

    void handleResponse(Object* unit, Object* other);

    glm::vec3 CalculateCollisionTorque(const glm::vec3& relativeAngularVelocity, Collider& collider, const PhysicsSystem* Rb, const PhysicsSystem* objRb);

    void Move(Unit* unit, glm::vec3 pos);

    glm::vec3 offset = glm::vec3(600.0f);
    static const int NUM_CELLS = 50;
    static const int CELL_SIZE = 200;

    Unit* cells[NUM_CELLS][NUM_CELLS][NUM_CELLS];
};

class Unit
{
    friend class Grid;


public:

    Object* collider;

    Unit(Grid* _grid, Object* _collider, glm::vec3 pos)
    {
        grid = _grid;
        prev = NULL;
        next = NULL;
        collider = _collider;
        x = collider->transform.pos.x;
        y = collider->transform.pos.y;
        z = collider->transform.pos.z;
        grid->Add(this);
    }


    void Move(Unit* unit);

private:
    float x, y, z;
    Grid* grid;
    Unit* prev;
    Unit* next;
};

