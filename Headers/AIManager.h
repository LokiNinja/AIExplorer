#pragma once
#include "Steering.h"
#include "GameObjects.h"

enum STATE{SEEK, FLEE, ARRIVE, WANDER, AVOID, BLENDED}; 
class FSM;

class EntityManager;


class AIManager {
private:
	//pointer to an array of steering objects--1 for each AI
	SteeringOutput* steering;
	//holds the number of steering objects
	int numObj;
	//holds the current state machine to process
	FSM* state;
	//Holds the blend weights
	static AIManager* _instance;
	float weights[5];
protected:
	AIManager();
public:
	//Base constructors and destructors

	~AIManager();
	static AIManager* getInstance();
	//Accesor and Mutator for state member

	//Accessor for steering output
	SteeringOutput* getSteering(int num) { return &steering[num]; }

	//Perform steering behaviors
	void update();
	
	//Accessor and Mutator for weights array
	void setWeight(float weight, STATE behavior) { weights[behavior] = weight; }
	float getWeight(STATE weight) { return weights[weight]; }

	void setFSM(FSM* f) { state = f; }

	State* getInitialState();


};

