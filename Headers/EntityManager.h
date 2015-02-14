#pragma once

#include "AIShip.h"

class AIManager;
class AIEntity;

extern const int NUMAI;

class EntityManager {
private:
	//holds all AI entities
	AIEntity** objectList;
	//number of entities
	static int numObj;
	static int currentObj;
	static int numRemoved;
	static EntityManager* _instance;
protected:
	EntityManager();
	EntityManager(const EntityManager &ent);	
public:
	//Constructor/Destructor
	~EntityManager();

	//Return the number of entities
	static int getNumObj() { return currentObj; }
	int getMaxObj() const { return numObj; }

	int getRemoved() { return numRemoved; }

	//calls all entity update functions
	void update(float elapsedTime, sf::RenderWindow &target, AIManager *AI);

	bool add(AIEntity*);
	bool remove(int id);


	//returns a specific entity
	AIEntity* getObj(int);

	static EntityManager* getInstance();

};
