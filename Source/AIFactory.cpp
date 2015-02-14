#include "../Headers/AIFactory.h"
#include "../Headers/GameObjects.h"
#include "../Headers/EntityManager.h"
#include "../Headers/AIManager.h"

AIFactory* AIFactory::_instance = 0;
bool* AIFactory::isUsed = 0;
int AIFactory::createdObj = 0;


AIFactory::AIFactory()
{
	int max = EntityManager::getInstance()->getMaxObj();
	isUsed = new bool[max];
	for (int i = 0; i < max; i++)
			isUsed[i] = false;
}
AIFactory* AIFactory::getInstance()
{
	if (_instance == 0)
		_instance = new AIFactory;
			
	return _instance;
}

AIFactory::~AIFactory()
{
	delete [] isUsed;
	delete _instance;
}


AIEntity* AIFactory::create(TYPE type, sf::Vector2u& size)
{
	if (EntityManager::getNumObj() < EntityManager::getInstance()->getMaxObj())
	{
		AIEntity* temp;
		if (type == SHIP)
		{
			temp = new AIShip;
			*temp = GameObjects::GetAIShip();
		}
			temp->setID(acquireID());
			//Atempt at catching error when no id's are left
			if (temp->getID() == -1)
			{
				LOG("Could not create AIEntity");
				delete temp;
				return 0;
			}
			temp->SetPosition( sf::Vector2f( (float)(rand() % size.x), (float)(rand() % size.y) ) );
			temp->setState(AIManager::getInstance()->getInitialState());
			temp->getCurrentTime(true);
			createdObj++;
			return temp;
	}
	LOG("Maximum allowed ai already spawned.");
	return 0;
}

int AIFactory::acquireID()
{
	for (int i = 0; i < EntityManager::getInstance()->getMaxObj(); i++)
	{
		if (isUsed[i] == false)
		{
			isUsed[i] = true;
			return i;
		}
	}
	LOG("No valid ids");
	return -1;
}

void AIFactory::releaseID(int id)
{
	if (id < EntityManager::getInstance()->getMaxObj())
		isUsed[id] = false;
}

bool AIFactory::checkID(int i)
{
	return isUsed[i];
}