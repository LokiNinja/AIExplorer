#include "../Headers/EntityManager.h"
#include "../Headers/GameObjects.h"
#include "../Headers/AIManager.h"
#include "../Headers/AIEntity.h"
#include "../Headers/AIFactory.h"

EntityManager* EntityManager::_instance = 0;
int EntityManager::currentObj = 0;
int EntityManager::numObj = 20;
int EntityManager::numRemoved = 0;

EntityManager::EntityManager()
{
	objectList = new AIEntity*[numObj];
	numObj = numObj;
	for (int i = 0; i < numObj; i++)
		objectList[i] = 0;
}


EntityManager::~EntityManager()
{
	for (int i = 0; i < numObj; i++)
	{
		if (objectList[i] != 0)
		delete objectList[i];
	}
	delete [] objectList;
	delete _instance;
}


AIEntity* EntityManager::getObj(int num)
{
	return objectList[num];
}



void EntityManager::update(float elapsedTime, sf::RenderWindow &target, AIManager *AI)
{
	int num = 0;
	int i = 0;
	while ( num != currentObj)
	{
		if (AIFactory::checkID(i))
		{
			objectList[i]->Update(elapsedTime, target, *AI->getSteering(i));
			num++;
		}
		i++;
	}
}


bool EntityManager::add(AIEntity* ai)
{
	if (currentObj < numObj && ai != 0)
	{
		objectList[ai->getID()] = ai;
		currentObj++;
		return true;
	}
		LOG("Could not add entity with id %i to map", ai->getID());
		return false;
}


bool EntityManager::remove(int id)
{
	if( objectList[id] != 0)
	{
		delete objectList[id];
		objectList[id] = 0;
		currentObj--;
		AIFactory::getInstance()->releaseID(id);
		numRemoved++;
		return true;
	}
		LOG("Could not delete entity with id %i from map", id);
		return false;
}

EntityManager* EntityManager::getInstance()
{
	if (_instance == 0)
		_instance = new EntityManager;
	return _instance;
}