#pragma once

#include "MathHelper.h"
#include "AIEntity.h"
struct SteeringOutput;


class AIShip : public AIEntity
{
private:

	
public:

	AIShip();

	virtual void Update( float elapsedTime, sf::RenderWindow& target, SteeringOutput &steering );
};