#include "../Headers/Pathfinding.h"


////////////////////////////////////////////////////CONNECTION////////////////////////////////////////////////////////////////////////////////

Connection::Connection(Node* d, int f)
{
	destination = d;
	fCost = f;
}

Connection::Connection() : destination(0), fCost(0)
{
}

Connection::~Connection()
{
}

int Connection::getFCost() const
{
	return fCost;
}

Node* Connection::getDestination() const
{
	return destination;
}

///////////////////////////////////////////////////////////////////NODE/////////////////////////////////////////////////////////////////////


//Constructor not used because I dynamically allocate at runtime
//probably can delete this
//Node::Node(sf::Vector2f p)
//{
//	pos = p;
//	numConnections = 0;
//	maxConnections = 4;
//	connections = new Connection[maxConnections];
//	memset(connections, 0, sizeof(Connection) * maxConnections);
//	hCost = std::numeric_limits<int>::max();
//	costSoFar = 0;
//}


Node::Node()
{
	pos = sf::Vector2f(0, 0);
	numConnections = 0;
	maxConnections = 4;
	connections = new Connection[maxConnections];
	memset(connections, 0, sizeof(Connection) * maxConnections);
	hCost = std::numeric_limits<int>::max();
	costSoFar = 0;
}


//Destructor
Node::~Node()
{
	delete [] connections;
}

int Node::getNumConnections() const
{
	return numConnections;
}


sf::Vector2f Node::getPos() const
{
	return pos;
}

Connection* Node::getConnection(int i)
{
	//Make sure the connection is in bounds and return connection
	if (i < numConnections && i >= 0)
		return &connections[i];
	//otherwise simply return 0
	return 0;
}

int Node::getHCost() const
{
	return hCost;
}

void Node::setHCost(int h)
{
	hCost = h;
}

bool Node::addConnection(Connection* c)
{
	//Make sure there is room for it
	if (numConnections < maxConnections)
	{
		//Copy it to the connections pointer and increment the total number of connections for the node
		memcpy(&connections[numConnections], c, sizeof(Connection));
		numConnections++;
		return true;
	}
	return false;
}

void Node::setPos(sf::Vector2f position)
{
	pos = position;
}

void Node::setCostSoFar(int c)
{
	costSoFar = c;
}

int Node::getCostSoFar() const
{
	return costSoFar;
}

void Node::setParent(Node* p)
{
	parent = p;
}

Node* Node::getParent() const 
{
	return parent;
}

//////////////////////////////////////////////////GRAPH/////////////////////////////////////////////////////////////////////////////////////

Graph::Graph(int x, int y)
{
	ydepth = y/YDEPTH;
	xdepth = x/XDEPTH;
	numNodes = (xdepth)*(ydepth);

	graph = new Node[numNodes];
	width = x;
	height = y;
	graphCreated = false;
	start = NULL;
	goal = NULL;
}

Graph::~Graph()
{
	delete [] graph;
}

 Node* Graph::getRandomNode() const
{
	return &graph[rand() % numNodes];
}

int Graph::getNumNodes() const
{
	return numNodes;
}

void Graph::setStart(Node* s)
{
	start = s;
}

void Graph::setGoal(Node* g)
{
	goal = g;
}

Node* Graph::getNode(int x) const
{
	return &graph[x];
}

bool Graph::ConnectNodes(Node* a, int fab, Node* b, int fba)
{
	if (a == b)
		return false;
	Connection ab(b, fab);
	Connection ba(a, fba);
	if (a->addConnection(&ab) && b->addConnection(&ba))
		return true;
	return false;
}

Node* Graph::getClosestTo(sf::Vector2f pos)
{
	float smallest = std::numeric_limits<float>::max();
	int nodeNum;
	for (int i = 0; i < numNodes; i++)
	{
		float distance = pow((pos.x - graph[i].getPos().x),2) + pow((pos.y - graph[i].getPos().y), 2);
		if (distance < smallest)
		{
			smallest = distance;
			nodeNum = i;
		}
	}
	return &graph[nodeNum];

}

bool Graph::FindNode(Node* node, int &x)
{
	for (int i = 0; i < numNodes; i++)
	{
		if (&graph[i] == node)
		{
			x = i;
			return true;
		}
	}
	return false;
}


void Graph::CreateGraph()
{
	int currentX = XOFFSET;
	int currentY = YOFFSET;
	//Set position of nodes and horizontal connections
	for (int i = 0; i < numNodes; i++)
	{
		graph[i].setPos(sf::Vector2f(float(currentX), float(currentY)));
			if ((currentX + XDEPTH) < width)
				currentX += XDEPTH;
			else
			{
				currentX = XOFFSET;
				if ((currentY + YDEPTH) < height)
					currentY += YDEPTH;
				else
					currentY = YOFFSET;
			}
			//dont connect if its the beginning of a row
			if (i % xdepth == 0)
				continue;
			//otherwise connect to the node to the immediate left of current node
			ConnectNodes(&graph[i], XDEPTH, &graph[i-1], XDEPTH);
		

	}
	//Go through graph again doing the vertical connections
	for (int i = 0; i < numNodes; i++)
	{
		if (i >= numNodes-xdepth)
			break;
			//otherwise connect to the node immediately below
		ConnectNodes(&graph[i], YDEPTH, &graph[i + xdepth], YDEPTH);
		
	}
	graphCreated = true;
}

void Graph::Draw(sf::RenderWindow &win)
{
	if (graphCreated == false)
		return;
	for (int i = 0; i < numNodes; i++)
	{
			sf::Vector2f nodePos = graph[i].getPos();
			sf::CircleShape circle(10.f);
			if (&graph[i] == goal || &graph[i] == start)
			{
				circle.setFillColor(sf::Color::Blue);
				circle.setOutlineColor(sf::Color::Blue);
			}
			else
			{
				circle.setFillColor(sf::Color::Red);
				circle.setOutlineColor(sf::Color::Red);
			}
			circle.setPosition(nodePos.x, nodePos.y);
			win.draw(circle);
	}
}


//////////////////////////////////////////////////////////////ASTAR////////////////////////////////////////////////////////////////////////

Node* getSmallestElement(std::vector<Node*> &vector)
{
	Node* smallest = vector.front();
	for (std::vector<Node*>::iterator it = vector.begin(); it != vector.end(); it++)
	{
		Node* temp = *it;
		if (temp->getHCost() < smallest->getHCost())
			smallest = temp;
	}
	return smallest;
}

std::vector<Node*> Reverse(std::vector<Node*> &vector)
{
	std::vector<Node*> temp;
	for(unsigned int i = vector.size() - 1; temp.size() != vector.size(); i--)
	{
		Node* tempNode = vector[i];
		temp.push_back(tempNode);
	}
	return temp;
}

std::vector<Node*> ReconstructPath(Node* goal, Node* start)
{
	std::vector<Node*> temp;
	Node* tempNode = goal;
	while (tempNode != start)
	{
		temp.push_back(tempNode);
		tempNode = tempNode->getParent();
	}
	return Reverse(temp);
}

std::vector<Node*>::iterator findElement(std::vector<Node*> &vector, Node* toFind)
{
	std::vector<Node*>::iterator it = vector.begin();
	while (*it != toFind)
	{
		it++;
	}
	return it;
}

bool Contains(std::vector<Node*> &vector, Node* toFind)
{
	for (std::vector<Node*>::iterator it = vector.begin(); it != vector.end(); it++)
	{
		if (*it == toFind)
			return true;
	}
	return false;
	
}

int AStar::Heuristic(Node* start, Node* goal)
{
	return int(sqrt(pow(start->getPos().x - goal->getPos().x, 2) + pow(start->getPos().y - goal->getPos().y, 2)));
}

std::vector<Node*> AStar::Pathfind(Graph* graph, Node* start, Node* goal)
{
	//Data structures needed
	std::vector<Node*> open, closed;
	std::vector<Node*> finalPath;
	Node* current;
	//get the hCost for the starting node
	int hCost = Heuristic(start, goal);
	//set the hCost for the starting node
	start->setHCost(hCost);
	start->setCostSoFar(0);
	//Add it to the open list
	open.push_back(start);
	Connection connections[4];
	int currentNumConnections;
	std::vector<Node* >::iterator it;
	

	//Continue processing until nothing is left in the open list
	while (open.size() > 0)
	{
		//The current node is the most probable node
		current = getSmallestElement(open);
		//check if we have reached the goal
		if (current->getPos() == goal->getPos())
		{
			break;
		}
		
		//This takes care of copying the connections over
		currentNumConnections = current->getNumConnections();
		for (int i = 0; i < currentNumConnections; i++)
			memcpy(&connections[i], current->getConnection(i), sizeof(Connection));

		it = findElement(open, current);
		open.erase(it);
		closed.push_back(current);

		for (int i = 0; i < currentNumConnections; i++)
		{
			Node* nextNode = connections[i].getDestination();

			int tentativeG = current->getCostSoFar() + connections[i].getFCost();
			int tentativeF = tentativeG + Heuristic(nextNode, goal);

			if (Contains(closed, nextNode) && tentativeF >= nextNode->getCostSoFar())
				continue;
			else if (!Contains(open, nextNode) || tentativeF < nextNode->getHCost())
			{
				nextNode->setParent(current);
				nextNode->setCostSoFar(tentativeG);
				nextNode->setHCost(tentativeF);
				if (!Contains(open, nextNode))
					open.push_back(nextNode);
			}
		}
	}
	return ReconstructPath(goal, start);
}