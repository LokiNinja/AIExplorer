#include <SFML/Window/Keyboard.hpp>
#include "../Headers/PlayerShip.h"
#include "../Headers/Pathfinding.h"

using namespace sf;

PlayerShip::PlayerShip()
{
}

void PlayerShip::Update( float elapsedTime, RenderWindow& target, Graph &graph ) 
{
	Vector2f position;
	std::vector<Node*>::iterator it;
	Node *start, *goal;
	float targetRadius = 50.f;
	float slowRadius = 100.f;

	position = Entity::GetPosition();

	if (currentPath.size() == 0)
	{
		goal = graph.getRandomNode();
		start = graph.getClosestTo(position);
		graph.setStart(start);
		graph.setGoal(goal);
		currentPath = AStar::Pathfind(&graph, start, goal);
	}
	else if (MathHelper::Distance(position, currentPath[0]->getPos()) <= 2)
	{
		it = currentPath.begin();
		currentPath.erase(it);
	}
	//Basic Arrive behavior
	else 
	{
		float timeToTarget = .1f;
		float targetSpeed;

		sf::Vector2f targetVelocity;
		sf::Vector2f direction = currentPath[0]->getPos() - position;

		float distance = MathHelper::Length( direction );

		if (distance < targetRadius )
			targetSpeed = 0.0f;

		if (distance > slowRadius)
			targetSpeed = MAXSPEED;
		else
			targetSpeed = MAXSPEED * distance / slowRadius;

		targetVelocity = direction;
		MathHelper::Normalize( targetVelocity );
		targetVelocity *= targetSpeed;

		steering.linear = targetVelocity - velocity;
		steering.linear /= timeToTarget;

		if (MathHelper::Length( steering.linear ) > MAXACCEL )
		{
			MathHelper::Normalize( steering.linear );
			steering.linear *= MAXACCEL;
		}

		steering.angular = 0;
	}


	
	position.x += velocity.x * elapsedTime;
	position.y += velocity.y * elapsedTime;
	Entity::SetPosition( position );

	
	float velLength = MathHelper::Length( velocity );

	if ( velLength > 0.0f )
	{
		float angle = atan2( velocity.x, -velocity.y );
		angle += steering.angular * elapsedTime;
		Entity::SetRotation( MathHelper::ToDegrees( angle ) );
	}
	
	velocity.x += steering.linear.x * elapsedTime;
    velocity.y += steering.linear.y * elapsedTime;

	if ( velLength > MAXSPEED )
	{
		MathHelper::Normalize( velocity );
		velocity *= MAXSPEED;
	}
	

	Entity::Update( elapsedTime, target );
}

PlayerShip::~PlayerShip()
{
}
