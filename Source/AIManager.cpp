#include "../Headers/AIManager.h"
#include "../Headers/EntityManager.h"
#include "../Headers/AIFactory.h"
#include "../Headers/FSM.h"


AIManager* AIManager::_instance = 0;

AIManager::AIManager()
{
	numObj = NUMAI;
	steering = new SteeringOutput[NUMAI];
	for (int i = 0; i < 5; i++)
		weights[i] = 0;
	state = 0;
}


AIManager* AIManager::getInstance()
{
	if (_instance == 0)
		_instance = new AIManager;
	return _instance;
}
AIManager::~AIManager()
{
	delete [] steering;
	delete _instance;
}

//Performs the steering behavior and stores the results in the steeringoutput array
void AIManager::update()
{
	numObj = EntityManager::getInstance()->getNumObj();
	int num = 0;
	int i = 0;
	while (num != numObj)
	{
		if (AIFactory::getInstance()->checkID(i))
		{
			state->update(i);
			num++;
		}
		i++;
	}
}

State* AIManager::getInitialState()
{
	return state->getInitialState();
}

