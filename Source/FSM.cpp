#include "../Headers/FSM.h"
#include "../Headers/AIEntity.h"
#include "../Headers/EntityManager.h"
#include "../Headers/MessageDispatcher.h"
#include "../Headers/GameObjects.h"
#include "../Headers/Steering.h"
#include "../Headers/AIManager.h"
#include "../Headers/AIFactory.h"


/////////////////////////////////////////////////////TRANSITION (TIMER)////////////////////////////////////////////////////////////////

bool TimerTransition::isTriggered(AIEntity* a)
{
	if (a->getCurrentTime() >= time)
	{
		a->getCurrentTime(true);
		return true;
	}
	return false;
}


void TimerTransition::action(AIEntity* a)
{
	LOG("Entity %i is in timer transition.\n", a->getID());
}
///////////////////////////////////////////////////////STATE BASE CLASS////////////////////////////////////////////////////////////////

State::State(int i)
{
	transitions = new Transition*[i];
	numTransitions = i;
	for (int i = 0; i < numTransitions; i++)
		transitions[i] = 0;
}

State::~State()
{
	delete [] transitions;
}

bool State::addTransition(Transition* t)
{
	for (int i = 0; i < numTransitions; i++)
	{
		if (transitions[i] == 0)
		{
			transitions[i] = t;
			return true;
		}
	}
		return false;
}

//////////////////////////////////////////////////////////////FSM//////////////////////////////////////////////////////////////////////////

void FSM::update(int id)
{
	Transition* triggered = 0;
	State* targetState = 0;

	State* currentState = 0;

	AIEntity* temp = EntityManager::getInstance()->getObj(id);

	//Get the entity's current state
	currentState = temp->getState();
	currentState->setCurrentObj(id);

	for (int i = 0; i < currentState->getNumTransitions(); i++)
	{
		Transition* tran = currentState->check(i);
		if (tran->isTriggered(temp))
		{
			triggered = tran;
			break;
		}
	}
	//If something triggered send the message to change state
 	if (triggered != 0)
	{
		currentState->setTriggered(triggered);
		msg* message = new msg;
		message->mess = CHANGE_STATE;
		message->toEntity = temp->getID();
		temp->send(message);
		delete message;
	}
	//otherwise send a message to perform action
	else
	{
		msg* message = new msg;
		message->mess = PERFORM_ACTION;
		message->toEntity = temp->getID();
		temp->send(message);
		delete message;
	}
}


//////////////////////////////////////////////////////////SBIRTH/////////////////////////////////////////////////////////////////////////

void SBirth::onEntry()
{
	LOG("Entity %i entering birth state.\n", currentObj);
}

void SBirth::onExit()
{
	LOG("Entity %i finished birth state.  Transitioning to death.\n", currentObj);
}

void SBirth::action()
{
	SteeringOutput* steering = AIManager::getInstance()->getSteering(currentObj);
	AIEntity* temp = EntityManager::getInstance()->getObj(currentObj);
	float seek = AIManager::getInstance()->getWeight(SEEK);
	float flee = AIManager::getInstance()->getWeight(FLEE);
	float arrive = AIManager::getInstance()->getWeight(ARRIVE);
	float wander = AIManager::getInstance()->getWeight(WANDER);
	float avoid = AIManager::getInstance()->getWeight(AVOID);
	*steering = Steering::Seek(temp, GameObjects::GetPlayer().GetPosition()) * seek;
				*steering += Steering::Flee(temp, GameObjects::GetPlayer().GetPosition()) * flee;
				*steering += Steering::Arrive(temp, GameObjects::GetPlayer().GetPosition(), 100.f, 50.f) * arrive;
				*steering += Steering::Wander(temp, temp->GetRotation( true ), 25 ) * wander;
				MathHelper::Normalize( steering->linear );
				steering->linear *= temp->GetMaxAcceleration();


}

///////////////////////////////////////////////////////////SDEATH//////////////////////////////////////////////////////////////////////

void SDeath::onEntry()
{
	LOG("Entity %i entering death state.\n", currentObj);
	EntityManager::getInstance()->add(AIFactory::getInstance()->create(SHIP));
}

void SDeath::onExit()
{
	LOG("Entity %i exiting death state. Entity will now die.\n", currentObj);	
	
}

void SDeath::action()
{
	SteeringOutput* steering = AIManager::getInstance()->getSteering(currentObj);
	AIEntity* temp = EntityManager::getInstance()->getObj(currentObj);
	*steering = Steering::Flee(temp, GameObjects::GetPlayer().GetPosition());
}