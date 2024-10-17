#include "WorldManager.h"
#include "Assets/Object.h"

std::vector<Object*> GameObjects;

Object* CurrentPlayer;

enum Tags;

Object* FindObject(std::string name);
Object* FindObjectWithTag(Tags name);

Object* FindObjectWithTag(Tags tag)
{
    for (Object* obj : GameObjects)
    {
        if (obj->tag == tag)
        {
            return obj;
        }
    }
}

Object* FindObject(std::string name)
{
    for (Object* obj : GameObjects)
    {
        if (obj->name == name)
        {
            return obj;
        }
    }
}
