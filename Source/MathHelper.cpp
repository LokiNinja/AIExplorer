#include "../Headers/MathHelper.h"
#include <cmath>
using namespace std;

float MathHelper::Length( const sf::Vector2f& vector )
{
	return sqrt( pow( vector.x, 2 ) + pow( vector.y, 2 ) );
}

float MathHelper::Normalize( sf::Vector2f& vector )
{
	float length = MathHelper::Length( vector );

	if ( length > 0.f )
	{
		vector /= length;
	}

	return length;
}

//I dont need the square root here, but Ill change it later
float MathHelper::Distance(sf::Vector2f pos1, sf::Vector2f pos2)
{
	return abs((pos1.x - pos2.x) + (pos1.y - pos2.y));
}