#pragma once

const int MAXMSG = 256;
//Types of messages
enum MSGTYPE { CHANGE_STATE, PERFORM_ACTION, DIE };


//message structure
struct msg {
	MSGTYPE mess;
	int toEntity;
};

//singleton
class MessageDispatcher {

public:
	//returns instance
	static MessageDispatcher* getInstance();
	//destructor
	~MessageDispatcher() { delete [] messages; }

	//process a single message from front of queue
	void send();
	//receive an incoming message
	void receive(msg* message);
	//process all messages
	void flush();
private:
	//singleton object
	static MessageDispatcher* _instance;
	MessageDispatcher(const MessageDispatcher&);
	MessageDispatcher();

	//number of messages in queue
	int numMsg;

	//actual message queue
	msg* messages;
};