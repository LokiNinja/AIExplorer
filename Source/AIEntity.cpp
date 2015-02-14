#include "../Headers/AIEntity.h"
#include "../Headers/GameObjects.h"
#include "../Headers/FSM.h"
#include "../Headers/EntityManager.h"

void AIEntity::send(msg* message)
{
	//simply place the message in the queue
	MessageDispatcher::getInstance()->receive(message);
}

void AIEntity::receive(msg* message)
{
	state->setCurrentObj(id);
	//perform actions based on message...logs are for debug purposes
	switch (message->mess) {
	case CHANGE_STATE:
		state->onExit();
		state->getTriggered()->action(this);
		if (state->getTriggered()->getTargetState() == 0)
		{
			msg* message = new msg;
			message->mess = DIE;
			message->toEntity = id;
			send(message);
			delete message;
		}
		else {
			state = state->getTriggered()->getTargetState();
			state->setCurrentObj(id);
			state->onEntry();
		}
		break;
	case PERFORM_ACTION:
		state->action();
		break;
	case DIE:
		EntityManager::getInstance()->remove(id);
		break;
	default:
		LOG("Entity with id %i received unknown message\n", id);
	}
}

float AIEntity::getCurrentTime(bool reset)
{
	float time = clock.getElapsedTime().asSeconds();
	if (reset == true)
		clock.restart();
	return time;
}