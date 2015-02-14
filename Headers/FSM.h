#pragma once

class State;
class AIEntity;

class Transition {
public:
	Transition(State* target) : targetState(target) {}
	Transition(): targetState(0) {}

	virtual bool isTriggered(AIEntity* a) = 0;
	State* getTargetState() const { return targetState; };
	virtual void action(AIEntity* a) = 0;
protected:
	State* targetState;
};

class TimerTransition : public Transition {
public:
	TimerTransition(float t, State* target = 0) : time(t), Transition(target) {}
private:
	float time;
	virtual bool isTriggered(AIEntity* a);
	virtual void action(AIEntity* a);
};

class State {
public:
	State(int i = 1);
	~State();

	virtual void onEntry() = 0;
	virtual void onExit() = 0;
	virtual void action() = 0;

	int getNumTransitions() const { return numTransitions; }
	Transition* check(int i) const { return transitions[i]; }
	void setTriggered(Transition* t) {	triggered = t;	}
	Transition* getTriggered() { return triggered; }
	bool addTransition(Transition *t);

	void setCurrentObj(int i) { currentObj = i; }

	int getCurrentObj() { return currentObj; }

	
protected:
	Transition** transitions;
	int numTransitions;
	Transition* triggered;
	int currentObj;
};

class SBirth : public State {
public:
	SBirth(int i = 1) : State(i) {}
	virtual void onEntry();
	virtual void onExit();
	virtual void action();
};

class SDeath : public State {
public:
	SDeath(int i = 1) : State(i) {}
	virtual void onEntry();
	virtual void onExit();
	virtual void action();
};


class FSM {
public:
	FSM(State* s) :initialState(s) {}

	void update(int id);
	State* getInitialState() { return initialState; }
protected:
	State* initialState;

};