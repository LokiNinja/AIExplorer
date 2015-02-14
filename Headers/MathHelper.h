#pragma once

#include <SFML/System/Vector2.hpp>

#define PI 3.14159265358979323846


//super basic math stuffs
class MathHelper
{
public:

	static float ToRadians( float degrees ) { return degrees * ( float ) PI / 180.f; }
	static float ToDegrees( float radians ) { return radians / ( float ) PI * 180.f; }

	static int Clamp( int x, int a, int b ) { return x < a ? a : (x > b ? b : x); }
	static float Clamp( float x, float a, float b ) { return x < a ? a : (x > b ? b : x); }

	static float Max( float a, float b ) { return a > b ? a : b; }
	static float Min( float a, float b ) { return a < b ? a : b; }

	static float Length( const sf::Vector2f& vector );
	static float Normalize( sf::Vector2f& vector );
	static float Dot(sf::Vector2f a, sf::Vector2f b) { return a.x*b.x + a.y*b.y; }

	static float Distance(sf::Vector2f, sf::Vector2f);
};