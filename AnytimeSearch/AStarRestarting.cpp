#include "stdafx.h"
#include "AStarRestarting.h"


AStarRestarting::AStarRestarting(
	void*  graph,
	std::list<void*>* (*adjacent)(void*, void*),
	double (*hFunction)(void*, void*),
	double (*cFunction)(void*, void*, void*),
	bool   (*isGoal)(void*, void*),
	bool   (*isEqual)(void*, void*, void*),
	void*  start,
	double w
	) :
	SearchBase(graph, adjacent, hFunction, cFunction, isGoal, isEqual, start), w(w)
{
	this->start = new Node();
	this->start->thisNode = start;
	this->start->parentNode = nullptr;
	this->start->g = 0;
	this->start->f = hFunction(graph, start);
	this->start->fw = w * hFunction(graph, start);
	incumbent = nullptr;
}


AStarRestarting::~AStarRestarting()
{
	delete start;
}


std::list<void*>* AStarRestarting::nextSolution()
{
	auto comp = [](Node* n1, Node* n2) {return *n1 < *n2; };
	// Initialize open and closed lists
	//open = new std::list<Node*>();
	//open.clear();

	//closed = new std::list<Node*>();
	//open->emplace_back(new Node(*start));
	open.insert(Node(*start));
	bool solutionFound = false;
	// Expanding nodes
	while (!open.empty() && !solutionFound)
	{
		Node currentNode = *open.begin();
		open.erase(open.begin());
		// Check, whether node's f-cost is not greater than the current upper bound
		if (incumbent == nullptr || currentNode.f < incumbent->f)
		{
			closed.emplace_back(currentNode);
			std::list<void*>* adjtmp = adjacent(graph, currentNode.thisNode);
			int adjcnt = adjtmp->size();
			auto it = adjtmp->begin();
			Node* successors = new Node[adjtmp->size()];
			for (int i = 0; it != adjtmp->end(); i++, it++)
			{
				//successors[i] = new Node();
				successors[i].thisNode = *it;
				successors[i].parentNode = &currentNode;
				successors[i].g = currentNode.g + cFunction(graph, currentNode.thisNode, *it);
				successors[i].f = successors[i].g + hFunction(graph, *it);
				successors[i].fw = successors[i].g + w * hFunction(graph, *it);
			}
			delete adjtmp;
			for (int i = 0; i < adjcnt; i++)
			{
				// Goal node is found
				if (isGoal(graph, successors[i].thisNode))
				{
					solutionFound = true;
					incumbent = new Node(successors[i]);
					break;
				}
				// Reexpanding node
				auto placeInOpen = std::find_if(open.begin(), open.end(), [&successors, &i, this](const Node& n) mutable {
					return isEqual(graph, n.thisNode, successors[i].thisNode);
				});
				auto placeInClosed = std::find_if(closed.begin(), closed.end(), [&successors, &i, this](const Node& n) mutable {
					return isEqual(graph, n.thisNode, successors[i].thisNode);
				});
				bool newNode = placeInOpen == open.end() && placeInClosed == closed.end();
				/*if (placeInOpen != open->end() && (*placeInOpen)->g > successors[i]->g)
				{
					delete *placeInOpen;
					open->erase(placeInOpen);
					insert(successors[i]);
				}*/
				if (placeInClosed != closed.end() && placeInClosed->g > successors[i].g)
				{
					//delete *placeInClosed;
					closed.erase(placeInClosed);
					open.insert(successors[i]);

				}
				// Expanding a new node
				if (newNode)
				{
					open.insert(successors[i]);
				}
			}
			delete[] successors;
		}
	}
	if (!solutionFound)
		throw "No solution";
	// Constructing the solution
	std::list<void*>* solution = new std::list<void*>();
	Node* currentNode = incumbent;
	while (currentNode != nullptr) 
	{
		solution->push_front(currentNode->thisNode);
		currentNode = currentNode->parentNode;
	}
	open.clear();
	closed.clear();
	return solution;
}

/*void AStarRestarting::insert(Node * n)
{
	auto it = open->begin();
	for (; it != open->end() && (*it)->fw < n->fw; it++);
	open->insert(it, n);
}*/

