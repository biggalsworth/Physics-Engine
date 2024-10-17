#pragma once
#include "Assets/common.h"

class Object;

//This script manages data and methods that every script should be able to access
/*
static class WorldManager
{
public:
	std::vector<Object*> GameObjects;

};

/*
//a class that contains all the functions relative for the current gameplay scene
class Scene
{
public:
    static std::vector<Object*> GameObjects;

    Scene();

};
*/

/// <summary>
/// Stores all the objects within the scene.
/// <para>From WorldManager.cpp</para>
/// </summary>
extern std::vector<Object*> GameObjects;

extern Object* CurrentPlayer;

static enum Tags
{
    Nothing,
    Player,
    Ground
};

/// <summary>
/// Find an object by tag
/// </summary>
/// <param name="Tag:">tag of the object (Tag:: *your tag here*)</param>
Object* FindObjectWithTag(Tags tag);


/// <summary>
/// Find an object by tag
/// </summary>
/// <param name="Tag:">tag of the object (Tag:: *your tag here*)</param>
Object* FindObject(std::string name);


