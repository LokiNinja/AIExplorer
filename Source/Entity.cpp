#include "../Headers/Entity.h"
#include "../Headers/MathHelper.h"
#include "../Headers/GameObjects.h"

using namespace sf;

bool Entity::LoadSprite( const std::string& fileName )
{
	if ( !texture.loadFromFile( fileName ) )
	{
		return false;
	}

	Vector2u dimensions = texture.getSize();
	sprite.setOrigin( dimensions.x * 0.5f, dimensions.y * 0.5f );
	sprite.setTexture( texture, true );

	// bounding box for debug drawing
	FloatRect rect = sprite.getLocalBounds();
	aabb.setSize( Vector2f( rect.width, rect.height ) );
	aabb.setOrigin( rect.width * 0.5f, rect.height * 0.5f );
	aabb.setFillColor( Color( 0, 0, 0, 0 ) );
	aabb.setOutlineThickness( 1.f );

	// text for debug drawing
	debugText.setFont( GameObjects::GetFont() ); 
	debugText.setCharacterSize( 16 );

	return true;
}

Vector2f Entity::GetPosition() const
{
	Vector2f position( sprite.getPosition() );
	return position;
}

void Entity::SetPosition( const Vector2f& position )
{
	sprite.setPosition( position );
}

float Entity::GetRotation( bool radians /* = false */ ) const
{
	float rotation = sprite.getRotation();

	if ( radians )
	{
		rotation = MathHelper::ToRadians( rotation );
	}

	return rotation;
}

void Entity::SetRotation( float angle )
{
	sprite.setRotation( angle );
}

void Entity::Draw( RenderWindow& target )
{
	target.draw( sprite );
}

void Entity::DebugDraw( RenderWindow& target )
{
	const Vector2f position = sprite.getPosition();
	
	// update all of the debug objects before drawing
	aabb.setPosition( position );

	char buf[32];
	sprintf_s( buf, 32, "{%.1f, %.1f}", position.x, position.y );

	debugText.setPosition( position );
	debugText.setString( buf );

	target.draw( aabb );
	target.draw( debugText );
}

Entity& Entity::operator=(const Entity& ent)
{
	if (&ent == this)
		return *this;
	else
	{
		texture = ent.texture;
		sprite = ent.sprite;
		aabb = ent.aabb;
		debugText = ent.debugText;
	}
	return *this;
}

void Entity::Update( float elapsedTime, RenderWindow& target )
{
	Vector2f size = target.getView().getSize();
	Vector2f position = sprite.getPosition();


	position.x = position.x < 0.f ? size.x : position.x;
	position.x = position.x > size.x ? 0.f : position.x;
	position.y = position.y < 0.f ? size.y : position.y;
	position.y = position.y > size.y ? 0.f : position.y;

	sprite.setPosition( position );
}