#include "../Headers/AIship.h"
#include "../Headers/Steering.h"
#include "../Headers/GameObjects.h"

using namespace sf;

AIShip::AIShip()
	: AIEntity(80.f, 100.f) {
}

void AIShip::Update( float elapsedTime, RenderWindow& target, SteeringOutput &steering )
{
	//kinematic stuff
	Vector2f position = Entity::GetPosition();
	
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

	if ( velLength > maxSpeed )
	{
		MathHelper::Normalize( velocity );
		velocity *= maxSpeed;
	}
	
	Entity::Update( elapsedTime, target );
}