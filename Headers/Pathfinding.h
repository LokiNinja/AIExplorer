#pragma once
#include "GameObjects.h"


//This is the distance between nodes
#define YDEPTH 100
#define XDEPTH 100
#define XOFFSET 50
#define YOFFSET 50


class Node;

class Connection {
public:
	//The usual
	Connection(Node* destination, int fcost);
	Connection();
	virtual ~Connection();
	
	//Accessors and a single Mutator for the hueristic cost
	int getFCost() const;

	Node* getDestination() const;

private:
	//fCost is the actual cost
	int fCost;
	//Target node
	Node* destination;
};

class Node {
public:
	//The usual
	Node(sf::Vector2f position);
	Node();
	virtual ~Node();

	//Accessor
	int getNumConnections() const;
	//add a Connection
	bool addConnection(Connection* connect);
	//for debug purposes mostly (to draw the grid once I have created it)
	sf::Vector2f getPos() const;
	//get a specific connection
	Connection* getConnection(int i);
	void setPos(sf::Vector2f pos);

	int getCostSoFar() const;
	void setCostSoFar(int c);

	void setHCost(int cost);
	int getHCost() const;

	void setParent(Node* p);
	Node* getParent() const;

protected:
	//No empty nodes
private:
	Connection* connections;
	int numConnections;
	sf::Vector2f pos;
	int maxConnections;
	int hCost;
	int costSoFar;
	Node* parent;

};

class Graph {
public:
	Graph(int x, int y);
	~Graph();
	//finds location of node in node array
	bool FindNode(Node* node, int &x);
	
	//Returns a node given the id
	Node* getNode(int x) const ;
	//connects two nodes
	bool ConnectNodes(Node* a, int fa, Node *b, int fb);

	void CreateGraph();

	//gets current number of nodes
	int getNumNodes() const;

	Node* getRandomNode() const;
	Node* getClosestTo(sf::Vector2f pos);

	//Draw the graph..for debugging purposes
	//need to make sure my create function works as it should
	void Draw(sf::RenderWindow &win);

	void setStart( Node* start);
	void setGoal(Node* goal);


private:
	bool graphCreated;
	Node* graph;
	int numNodes;
	int width, height;
	int xdepth, ydepth;
	Node* start, *goal;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class AStar {
public:
	static int Heuristic(Node*, Node*);
	static std::vector<Node*> Pathfind(Graph* graph, Node* start, Node* end);
};