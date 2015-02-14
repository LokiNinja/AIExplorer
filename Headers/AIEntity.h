#pragma once
#include "Entity.h"
#include "MathHelper.h"
#include "MessageDispatcher.h"

class State;
struct SteeringOutput;

class AIEntity : public Entity {
public:
	
	AIEntity(float accel, float speed) { maxAccel = accel; maxSpeed = speed; state = 0; }
	AIEntity() {maxAccel = 80.f; maxSpeed = 100.f; state = 0;  }

	//Entity functions
	sf::Vector2f GetVelocity() const { return velocity; }
	virtual void Update( float elapsedTime, sf::RenderWindow& target, SteeringOutput &steering ) = 0 ; 
	float GetMaxAcceleration() const { return maxAccel; }
	void SetMaxAcceleration( float a ) { maxAccel = MathHelper::Max( 0.f, a ); }

	float GetMaxSpeed() const { return maxSpeed; }
	void SetMaxSpeed( float s ) { maxSpeed = MathHelper::Max( 0.f, s ); }
	float GetSpeed() const { return MathHelper::Length( velocity ); }
	void setID(int num) { id = num; }
	int getID() { return id; }

	float getCurrentTime(bool reset = false);

	//Messaging Functions

	//send a message to the dispatcher
	void send(msg* message);
	//receive a message from the dispatcher
	void receive(msg* message);

	State* getState() const { return state; }
	void setState(State* s) { state = s; }
	
	

protected:

	float maxAccel;
	float maxSpeed;
	sf::Vector2f velocity;
	int id;
	State* state;
	sf::Clock clock;

};