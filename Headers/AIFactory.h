#pragma once

#include "AIEntity.h"
#include "AIShip.h"

extern const int NUMAI;

enum TYPE{SHIP};

class AIFactory {
protected:
	AIFactory();
	AIFactory(const AIFactory&);
private:
	static AIFactory* _instance;
	static bool* isUsed;
	static int createdObj;
public:
	~AIFactory();
	AIEntity* create(TYPE type, sf::Vector2u& size = sf::Vector2u(800, 600));
	static AIFactory* getInstance();

	int getCreated() { return createdObj; }

	static void releaseID(int id);
	static int acquireID();
	static bool checkID(int i);

};