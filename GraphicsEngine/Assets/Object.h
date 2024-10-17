#pragma once
#include "common.h"
#include "stb/stb_image.h"
#include <sstream> // for std::stringstream
#include <fstream>
#include <iostream>
#include <vector>
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>
#include <map>
#include "ObjShape.h"
#include "Shader.h"
#include "Textures/CubemapTexture.h"
#include "Particle.h"
#include "rigidbody.h"
#include "Mesh.h"
#include "Collider.h"
#include <typeinfo>
#include "PlayerScript.h"

struct Transform
{
    glm::vec3 pos;
    glm::vec3 rotation;
    glm::vec3 scale;
};


enum comps 
{
    MeshComp,
    RBodyComp
};


class Object 
{
public:
    static std::map<int, bool> keyInput;
    Transform transform;
    std::string name;
    Tags tag = Tags::Nothing;

    Shader* shader;
    Texture* objTexture;

    Rigidbody* rigidbody;

    float moveSpeed;

    std::vector<Component*> ObjComps;
    std::vector<Component*> scripts;


    Object(Shader objShader, glm::vec3 Pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 Scale = glm::vec3(1.0f));

    ~Object() 
    {
        //for (Component* comp : ObjComps) 
        //{
            //delete comp;
            //comp = nullptr;
        //}
        ObjComps.clear();

        for (int i = 0; i <= GameObjects.size(); i++) 
        {
            if (GameObjects[i] == this)
            {
                GameObjects.erase(GameObjects.begin() + i);
                break;
            }
        }
    }

    void Render(float dt);

    void Update(float dt);

    void UpdateScripts(float dt);

    #pragma region Components

    void PushComponent(Component* compType);


    /// <summary>
    /// Add a component.
    /// </summary>
    /// <param name="Type">What type of component to add.</param>
    void AddComponent(ComponentTypes Type);
    void AddComponent(ComponentTypes type = ComponentTypes::mesh, ObjShape* meshData = nullptr);
    /// <summary>
    /// Add a component.
    /// </summary>
    /// <param name="Type:">Select the type of component. Either rididbody or particle</param>
    /// <param name="Mass:">The mass of this rigidbody.</param>
    void AddComponent(ComponentTypes type = ComponentTypes::rigidbody, float mass = 1.0f);

    void AddComponent(ComponentTypes type = ComponentTypes::collider, colliderTypes colliderType = colliderTypes::Sphere, float collideDistance = 1, Shader* collShader = nullptr, ObjShape* collMesh = NULL);
    void AddComponent(ComponentTypes type = ComponentTypes::particleSystem, glm::vec3 position = glm::vec3(0), float size = 5.0f, int amount = 5, float duration = 5.0f, float mass = 1.0f, ObjShape* particleMesh = nullptr, bool gravity = true);

    void AddScript(Component* script);

    template<typename compType> compType* GetComponent(ComponentTypes type = BLANKTYPE)
    {
        //for (int i = 0; i < ObjComps.size(); i++)
        for (Component* comp : ObjComps)
        {
            if (dynamic_cast<compType*>(comp) != nullptr)
            {
                return dynamic_cast<compType*>(comp);
                /*
                if (type == ComponentTypes::mesh)
                {
                    return static_cast<compType*>(comp);
                }
                else if (type == ComponentTypes::collider)
                {
                    return static_cast<compType*>(comp);
                }
                else if (type == ComponentTypes::sphereCollider)
                {
                    return static_cast<compType*>(comp);
                }
                else if (type == ComponentTypes::planeCollider)
                {
                    return static_cast<compType*>(comp);
                }
                else if (type == ComponentTypes::rigidbody)
                {
                    return static_cast<compType*>(comp);
                }
                else if (type == ComponentTypes::particleSystem)
                {
                    return static_cast<compType*>(comp);
                }
                else if (type == ComponentTypes::script)
                {
                    return static_cast<compType*>(comp);
                }
                */
            }
        }
        return nullptr;
    };
    template<typename compType> compType* GetScript()
    {
        for (Component* comp : scripts)
        {
            //is it the same type of script
            if (dynamic_cast<compType*>(comp) != nullptr)
            {
                return static_cast<compType*>(comp);
            }
        }
        return nullptr;
    };

    //template<Collider*>GetComponent<Collider*>(ComponentTypes type);
    //template<> Mesh* GetComponent<Mesh*>(ComponentTypes type);
    //template<> Component* GetComponent<Component*>(ComponentTypes type);



    #pragma endregion
};