#pragma once

#include "Entity.h"
#include "Steering.h"

const float MAXSPEED = 100.f;
const float MAXACCEL = 80.f;

class Graph;
class Node;

class PlayerShip : public Entity
{
private:

	sf::Vector2f velocity;
	SteeringOutput steering;
	std::vector<Node*> currentPath;


public:

	PlayerShip();

	void Update( float elapsedTime, sf::RenderWindow& target, Graph &graph );
	~PlayerShip();
};