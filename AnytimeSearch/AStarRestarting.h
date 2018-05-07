// Anytime Weighted A* with restarting
#pragma once
#include "SearchBase.h"
#include "stdafx.h"
template<class G, class N>
class AStarRestarting :
	public SearchBase<G, N>
{
public:
	AStarRestarting(
		G*     graph,
		N      start,
		double w
		);
	~AStarRestarting();
	std::list<N> nextSolution();
protected:
	struct Node
	{
		N thisNode;
		Node* parentNode;
		double g;
		double f;
		double fw;
		bool operator <(const Node x) const
		{
			return fw < x.fw;
		}
	};
	std::multiset<Node> open;
	std::list<Node> closed;
	Node* start;
	Node* incumbent;
	double w;
};

template<class G, class N>
AStarRestarting<G, N>::AStarRestarting(
	G*     graph,
	N      start,
	double w
	) :
	SearchBase(graph), w(w)
{
	this->start = new Node();
	this->start->thisNode = start;
	this->start->parentNode = nullptr;
	this->start->g = 0;
	this->start->f = graph->hFunction(start);
	this->start->fw = w * graph->hFunction(start);
	incumbent = nullptr;
}


template<class G, class N>
AStarRestarting<G, N>::~AStarRestarting()
{
	delete start;
}


template<class G, class N>
std::list<N> AStarRestarting<G, N>::nextSolution()
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
			std::list<N>&& adjtmp = graph->adjacent(currentNode.thisNode);
			int adjcnt = adjtmp.size();
			auto it = adjtmp.begin();
			Node* successors = new Node[adjtmp.size()];
			for (int i = 0; it != adjtmp.end(); i++, it++)
			{
				//successors[i] = new Node();
				successors[i].thisNode = *it;
				successors[i].parentNode = &*(--closed.end());
				successors[i].g = currentNode.g + graph->cFunction(currentNode.thisNode, *it);
				successors[i].f = successors[i].g + graph->hFunction(*it);
				successors[i].fw = successors[i].g + w * graph->hFunction(*it);
			}
			for (int i = 0; i < adjcnt; i++)
			{
				// Goal node is found
				if (graph->isGoal(successors[i].thisNode))
				{
					solutionFound = true;
					incumbent = new Node(successors[i]);
					break;
				}
				// Reexpanding node
				/*auto placeInOpen = std::find_if(open.begin(), open.end(), [&successors, &i, this](const Node& n) mutable {
					return isEqual(graph, n.thisNode, successors[i].thisNode);
				});*/
				auto placeInClosed = std::find_if(closed.begin(), closed.end(), [&successors, &i, this](const Node& n) mutable {
					return n.thisNode == successors[i].thisNode;
				});
				bool newNode = /*placeInOpen == open.end() && */placeInClosed == closed.end();
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
	std::list<N> solution;
	Node* currentNode = incumbent;
	while (currentNode != nullptr)
	{
		solution.push_front(currentNode->thisNode);
		currentNode = currentNode->parentNode;
	}
	open.clear();
	closed.clear();
	return solution;
}
