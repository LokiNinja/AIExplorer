# AIExplorer
Experiment with some basic AI, steering algorithms, pathfinding, and FSM

Author: Thomas Loudiana

Description: AIExplorer is a program written to experiment with various game AI techniques. It demonstrates pathfinding
utilizing the A* algorithm, various steering behviors and how to use a FSM to create unique and varied combinations of these
elements.  

Version Info:  This program relies on the SFML libraries for rendering which, in turn, relies upon OpenGL.

Installation:  Just clone the repository and compile.  SFML has some minor issues with varying versions of visual studio and
may need to be set up differently on your machine.  In that case follow the link here:
http://www.sfml-dev.org/tutorials/2.0/start-vc.php.  There are no controls as the simulation is fully automated.

Future:  Due to the issues with SFML discussed above much of the code has been refactored and ported to a new version running
on C# and XNA.  While the new version is not yet public it does contain some cool features and should be available
shortly.  It turns the simulation into an interactive game and spawns increasingly difficult enemies by way of a genetic
algorithm.
