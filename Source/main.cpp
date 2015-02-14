#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <random>
#include <conio.h>

#include "../Headers/GameObjects.h"
#include "../Headers/AIShip.h"
#include "../Headers/AIManager.h"
#include "../Headers/EntityManager.h"
#include "../Headers/AIFactory.h"
#include "../Headers/MessageDispatcher.h"
#include "../Headers/FSM.h"
#include "../Headers/Pathfinding.h"

const int NUMAI = 3;

using namespace sf;

const Color cornflowerBlue( 100, 149, 237, 255 );

bool LoadResources();
void SetRandomPosition( Entity& e, Vector2u& size );
bool UpdateEvents( RenderWindow& window );
void UpdateDisplay( Text& text, char* curState );
int UpdateFps();

bool g_bDrawNodes = false;

int main()
{
	Clock totalTime;
	// Load game assets
	if ( !LoadResources() )
	{
		LOG( "<press any key to continue>" );
		_getch();
		return EXIT_FAILURE;
	}

    // Create the main window
    RenderWindow window( VideoMode( 800, 600 ), "GSP 315" );
	window.setFramerateLimit( 60 );

	Text text( "", GameObjects::GetFont(), 16 );
	srand( (unsigned int) time( NULL ) );
	
	Clock gameTime;

	SBirth birth(1);
	SDeath death(1);
	TimerTransition birthTimer(5.f, &death);
	TimerTransition deathTimer(8.f);

	birth.addTransition(&birthTimer);
	death.addTransition(&deathTimer);


	FSM stateMachine(&birth);

	//Instatiate AIMANAGER and ENTITYMANAGER
	EntityManager* Entities = EntityManager::getInstance();
	AIManager* AI = AIManager::getInstance();
	AIFactory* factory = AIFactory::getInstance();
	Graph graph(800, 600);
	graph.CreateGraph();


	//Allows to switch between more than 1 state machine for processing in a large game
	AI->setFSM(&stateMachine);

	//Set the weights for the blended steering
	AI->setWeight(0.f, SEEK);
	AI->setWeight(10.f, FLEE);
	AI->setWeight(10.f, ARRIVE);
	AI->setWeight(3.f, WANDER);
	AI->setWeight(20.f, AVOID);


	PlayerShip &player = GameObjects::GetPlayer();
	for (int i = 0; i < NUMAI; i++)
	{
		Entities->add(factory->create(SHIP, window.getSize()));
	}

	SetRandomPosition( player, window.getSize() );
  

	// Start the game loop
    while ( window.isOpen() )
    {
		float elapsed = gameTime.restart().asSeconds();
		srand((unsigned int) time(0));
		if ( !UpdateEvents( window ) )
		{
			window.close();
		}
		
		if (Keyboard::isKeyPressed( Keyboard::D) )
			g_bDrawNodes = !g_bDrawNodes;

		// Update entities
		player.Update( elapsed, window, graph );
		AI->update();
		Entities->update( elapsed, window, AI );
		MessageDispatcher::getInstance()->flush();
		

        // Begin Drawing
        window.clear( cornflowerBlue );
		if (g_bDrawNodes)
			graph.Draw(window);

		player.Draw( window );

		player.DebugDraw( window );

		int i = 0;
		int num = 0;
		while (num != EntityManager::getInstance()->getNumObj())
		{

			if (AIFactory::getInstance()->checkID(i))
			{
				Entities->getObj(i)->Draw(window);
				Entities->getObj(i)->DebugDraw(window);
				num++;
			}
			i++;
		}

		UpdateDisplay( text, "TESTING");
		window.draw( text );

        // Update the window
        window.display();
    }
	LOG("Total Run Time: %f\n", totalTime.getElapsedTime().asSeconds());
	LOG("Objects Created: %i\n", AIFactory::getInstance()->getCreated() );
	LOG("Objects Deleted: %i\n", Entities->getRemoved() );
	LOG( "<press any key to continue>" );
	_getch();

    return EXIT_SUCCESS;
}

bool LoadResources()
{
    if ( !GameObjects::GetFont().loadFromFile( "resources\\cour.ttf" ) )
	{
		LOG_ERROR( "Could not load font" );
		return false;
	}
	
	if ( !GameObjects::GetPlayer().LoadSprite( "resources\\plane1.png" ) )
	{
		LOG_ERROR( "Could not load sprite" );
		return false;
	}

	if ( !GameObjects::GetAIShip().LoadSprite( "resources\\plane2.png" ) )
	{
		LOG_ERROR( "Could not load sprite" );
		return false;
	}

	LOG( "All resources loaded successfully" );
	return true;
}

void SetRandomPosition( Entity& e, Vector2u& size )
{
	e.SetPosition( Vector2f( (float)(rand() % size.x), (float)(rand() % size.y) ) );
}

bool UpdateEvents( RenderWindow& window )
{
	Event event;
    
	while ( window.pollEvent(event) )
	{
		if ( event.type == Event::Closed )
		{
			LOG( "Closed event caught - exiting" );
			return false;
		}
	}
	if ( Keyboard::isKeyPressed( Keyboard::Escape ) )
	{
		LOG( "Escape pressed - exiting" );
		return false;
	}

	return true;
}

void UpdateDisplay( Text& text, char* curState )
{
	static const char stats[] =
		"%d FPS\n"
		"Press 'D' to draw nodes\n";

	char data[128];

	sprintf_s( data, sizeof( data ), stats, UpdateFps(), curState );
	text.setString( data );
}

int UpdateFps()
{
	static Clock clock;
	static int frameCount = 0;
	static int fps = 0;

	frameCount++;

	if ( clock.getElapsedTime().asSeconds() >= 1.f )
	{
		fps = frameCount;

		if ( fps < 20 )
		{
			LOG_WARNING( "Low framerate: %d", fps );
		}

		clock.restart();
		frameCount = 0;
	}

	return fps;
}