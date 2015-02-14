#include "../Headers/MessageDispatcher.h"
#include "../Headers/EntityManager.h"
#include "../Headers/GameObjects.h"


MessageDispatcher* MessageDispatcher::_instance = 0;

MessageDispatcher::MessageDispatcher()
{
	numMsg = 0;
	messages = new msg[MAXMSG];
}

MessageDispatcher* MessageDispatcher::getInstance()
{
	if (_instance == 0)
		_instance = new MessageDispatcher;
	return _instance;
}

void MessageDispatcher::receive(msg* message)
{
	if (numMsg == MAXMSG)
	{
		send();
		numMsg--;
	}
		messages[numMsg] = *message;
		numMsg++;
}

void MessageDispatcher::send()
{
	if (numMsg == 0)
		return;

	//take message from front of queue
	msg message = messages[0];
	//send the message to the correcty entity
	if (EntityManager::getInstance()->getObj(message.toEntity) != 0)
		EntityManager::getInstance()->getObj(message.toEntity)->receive(&message);
	//move all elements up one
	for (int i = 0; i < numMsg - 1; i++)
		messages[i] = messages[i+1];
	//decrement message counter
	numMsg--;
}

//flushes the entire queue, sending all messages
void MessageDispatcher::flush()
{
	while (numMsg != 0)
		send();
}
