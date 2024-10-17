#pragma once
#include "Object.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "../CubeCollider.h"
#include "../OBBCollider.h"
#include "Mesh.h"
#include "ParticleSystem.h"
#include "../PhysicsSystem.h"

std::map<int, bool> Object::keyInput;

Object::Object(Shader objShader, glm::vec3 Pos, glm::vec3 Rot, glm::vec3 Scale)
{
    shader = &objShader;
    objTexture = new Texture("Assets/imageData/TestTexture.png", *shader);

    transform.pos.xyz = Pos;
    transform.rotation.xyz = Rot;
    transform.scale.xyz = Scale;


    //add this object to the list of all objects in WorldManager.h
    GameObjects.push_back(this);
}



void Object::Update(float dt) 
{
    for (Component* comp: ObjComps) 
    {
        comp->Update(dt);
    }

}
void Object::UpdateScripts(float dt) 
{
    for (Component* script : scripts)
    {
        Component* tempComp = static_cast<Component*>(script);
        script->Update(dt);
    }
}

void Object::Render(float dt)
{
    for (Component* comp : ObjComps)
    {
        if (shader != nullptr)
        {
            shader->use();
        }
        else 
        {
            continue;
        }
        if (objTexture != NULL)
        {
            objTexture->Use();
        }
        Component* tempComp = static_cast<Component*>(comp);
        //tempComp->owner = this;
        tempComp->Draw(this);
    }

    //shader.changeTransform(obj.currPos, obj.scale);
    //shader->use();
    //shader->changeTransform(transform.pos, transform.scale, transform.rotation);
    //mesh->Draw();
}

void Object::PushComponent(Component* compType)
{
    compType->owner = this;
    ObjComps.push_back(compType);
}


void Object::AddComponent(ComponentTypes Type)
{
    std::cout << "Have not selected a component type!" << std::endl;
}

void Object::AddComponent(ComponentTypes type, ObjShape* meshData)
{
    //std::vector<VertexData> objData = *meshData;
    Mesh* tempMesh = new Mesh(meshData, shader);
    PushComponent(tempMesh);

}

void Object::AddComponent(ComponentTypes type, float mass)
{
    if (type == ComponentTypes::rigidbody)
    {
        Rigidbody* tempRigidbody = new Rigidbody(this, mass);
        tempRigidbody->pos = transform.pos;
        rigidbody = tempRigidbody;
        PushComponent(tempRigidbody);
    }
    else if (type == particle) 
    {
        Particle* tempRigidbody = new Particle(this->transform.pos, mass);
        tempRigidbody->pos = transform.pos;
        PushComponent(tempRigidbody);
    }
}
/// <summary>
/// Create a collider
/// </summary>
/// <param name="colliderType">What type of collider it is</param>
/// <param name="collideDistance">How far objects need to be to collide</param>
void Object::AddComponent(ComponentTypes type, colliderTypes colliderType, float collideDistance, Shader* collShader, ObjShape* collMesh)
{
    if (colliderType == colliderTypes::Sphere) 
    {
        SphereCollider* coll = new SphereCollider(this, collShader, collMesh);
        coll->collisionDist = collideDistance;
        coll->pos = transform.pos;
        PushComponent(coll);

    }
    else if (colliderType == colliderTypes::Cube) 
    {
        CubeCollider* coll = new CubeCollider(this, collShader, collMesh);
        coll->collisionDist = collideDistance;
        coll->pos = transform.pos;
        PushComponent(coll);

    }
    else if (colliderType == colliderTypes::Plane) 
    {
        PlaneCollider* coll = new PlaneCollider(this, collShader, collMesh);
        coll->collisionDist = collideDistance;
        coll->pos = transform.pos;
        PushComponent(coll);

    }
    else if (colliderType == colliderTypes::OBB) 
    {
        OBBCollider* coll = new OBBCollider(this, collShader, collMesh);
        coll->collisionDist = collideDistance;
        coll->pos = transform.pos;
        PushComponent(coll);

    }
}

void Object::AddComponent(ComponentTypes type, glm::vec3 position, float size, int amount, float duration, float mass, ObjShape* particleMesh, bool gravity)
{
    ParticleSystem* partSystem = new ParticleSystem(position, particleMesh, shader, size, amount, duration);
    partSystem->gravity = true;
    partSystem->shader = shader;
    partSystem->particleMat = objTexture;
    PushComponent(partSystem);
}

void Object::AddScript(Component* script)
{
    script->owner = this;
    scripts.push_back(script);
}
