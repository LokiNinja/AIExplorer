#include "../Headers/Steering.h"
#include "../Headers/MathHelper.h"
#include "../Headers/AIShip.h"
#include <cstdlib>


float randomBinomial()
{
	
	return (rand() / float(RAND_MAX + 1) ) - (rand() / float(RAND_MAX + 1));
}

SteeringOutput SteeringOutput::operator*(const float &num)
{
	this->linear *= num;
	this->angular *= num;
	return *this;
}

SteeringOutput& SteeringOutput::operator+=(const SteeringOutput &num)
{
	this->linear += num.linear;
	this->angular += num.angular;
	return *this;
}

SteeringOutput Steering::Seek( AIEntity *ship, sf::Vector2f target )
{
	SteeringOutput steering;
	steering.linear = target - ship->GetPosition();
	MathHelper::Normalize( steering.linear );
	
	steering.linear *= ship->GetMaxAcceleration();
	steering.angular = 0;
	return steering;
}


SteeringOutput Steering::Flee( AIEntity *ship, sf::Vector2f target )
{
	SteeringOutput steering;
	steering.linear = ship->GetPosition() - target;
	MathHelper::Normalize( steering.linear );

	steering.linear *= ship->GetMaxAcceleration();
	steering.angular = 0;
	return steering;
}


SteeringOutput Steering::Arrive(AIEntity *ship, sf::Vector2f target, float slowRadius, float targetRadius )
{
	SteeringOutput steering;
	float timeToTarget = .1f;
	float targetSpeed;

	sf::Vector2f targetVelocity;
	sf::Vector2f direction = target - ship->GetPosition();

	float distance = MathHelper::Length( direction );

	if (distance < targetRadius )
		targetSpeed = 0.0f;

	if (distance > slowRadius)
		targetSpeed = ship->GetMaxSpeed();
	else
		targetSpeed = ship->GetMaxSpeed() * distance / slowRadius;

	targetVelocity = direction;
	MathHelper::Normalize( targetVelocity );
	targetVelocity *= targetSpeed;

	steering.linear = targetVelocity - ship->GetVelocity();
	steering.linear /= timeToTarget;

	if (MathHelper::Length( steering.linear ) > ship->GetMaxAcceleration() )
	{
		MathHelper::Normalize( steering.linear );
		steering.linear *= ship->GetMaxAcceleration();
	}

	steering.angular = 0;

	return steering;

}


SteeringOutput Steering::Wander(AIEntity *ship, float rotation, float maxRotation )
{
	SteeringOutput steering;
	sf::Vector2f orientation( sin(rotation), -cos(rotation));
	steering.linear = ship->GetMaxSpeed() * orientation;
	steering.angular = randomBinomial() * maxRotation;
	return steering;
}


SteeringOutput Steering::Avoid(AIEntity *ship, AIEntity *targets, float radius, int numTargets )
{
	SteeringOutput steering = { ship->GetVelocity(), 0};
	float shortestTime = std::numeric_limits<float>::max();

	AIEntity* firstTarget = NULL;
	float firstMinSeparation, firstDistance, relativeSpeed, timeToCollision, distance, minSeparation;
	sf::Vector2f firstRelativePos, firstRelativeVel;
	sf::Vector2f relativePos, relativeVel;

	for (int i = 0; i < numTargets; i++)
	{
		if (ship == &targets[i])
			continue;

		relativePos = targets[i].GetPosition() - ship->GetPosition();
		sf::Vector2f vel1 = targets[i].GetVelocity();
		sf::Vector2f vel2 = ship->GetVelocity();
		vel1.y = -vel1.y;
		vel2.y = -vel2.y;
		relativeVel = vel1 - vel2;

		relativeSpeed = MathHelper::Length( relativeVel );
		if (relativeSpeed == 0)
			continue;

		timeToCollision = MathHelper::Dot( relativePos, relativeVel) / (relativeSpeed * relativeSpeed);
		distance = MathHelper::Length( relativePos );

		minSeparation = distance - relativeSpeed * timeToCollision;

		if (minSeparation > 2 * radius)
			continue;

		if (timeToCollision > 0 && timeToCollision < shortestTime)
		{
			shortestTime = timeToCollision;
			firstTarget = &targets[i];
			firstMinSeparation = minSeparation;
			firstDistance = distance;
			firstRelativePos = relativePos;
			firstRelativeVel = relativeVel;
		}
	}
	if (!firstTarget)
		return steering;

	if (firstMinSeparation <= 0 || firstDistance < 2* radius)
		relativePos = ship->GetPosition() - firstTarget->GetPosition();
	else
		relativePos = firstRelativePos + firstRelativeVel * shortestTime;

	MathHelper::Normalize( relativePos );
	steering.linear = relativePos * ship->GetMaxAcceleration();
	steering.angular = 0;

	return steering;

}