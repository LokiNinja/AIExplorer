#pragma once

#include <SFML/System/Vector2.hpp>
#include "AIEntity.h"

class AIShip;

struct SteeringOutput
{
	sf::Vector2f linear;
	float angular; 

	SteeringOutput operator*(const float &num);
	SteeringOutput& operator+=(const SteeringOutput &num);
};

class Steering
{
public:
	static SteeringOutput Seek( AIEntity *ship, sf::Vector2f target );
	static SteeringOutput Flee( AIEntity *ship, sf::Vector2f target );
	static SteeringOutput Arrive(AIEntity *ship, sf::Vector2f target, float slowRadius, float targetRadius );
	static SteeringOutput Wander(AIEntity *ship, float rotation, float maxRotation );
	static SteeringOutput Avoid(AIEntity *ship, AIEntity * targets, float radius, int numTargets );
};