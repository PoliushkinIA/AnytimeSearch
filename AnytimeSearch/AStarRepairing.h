// Anytime Weighted A* with repairing
#pragma once
#include "SearchBase.h"
#include "stdafx.h"
template<class G, class S>
class AStarRepairing :
	public SearchBase<G, S>
{
public:
	AStarRepairing(
		G*     graph,
		S      start,
		double w
		);
	~AStarRepairing();
	std::list<S> nextSolution();
	double weightDecreaseRate = 0.5;
protected:
	struct Node
	{
		S thisNode;
		const Node* parentNode;
		double g;
		double f;
		double fw;
		bool operator <(const Node x) const
		{
			return fw < x.fw;
		}
	};
	struct NodeComparator
	{
		bool operator()(const Node& lhs, const Node& rhs) const
		{
			return lhs.thisNode < rhs.thisNode;
		}
	};
	std::multiset<Node> open;
	std::multiset<Node, NodeComparator> closed;
	std::list<Node> inconsistent;
	Node* start;
	Node* incumbent;
	double w;
};



template<class G, class S>
AStarRepairing<G, S>::AStarRepairing(
	G*     graph,
	S      start,
	double w
	) :
	SearchBase(graph), w(w)//, closed([](const Node& a, const Node& b) { return a.thisNode < b.thisNode; })
{
	this->start = new Node();
	this->start->thisNode = start;
	this->start->parentNode = nullptr;
	this->start->g = 0;
	this->start->f = graph->hFunction(start);
	this->start->fw = w * graph->hFunction(start);
	incumbent = nullptr;
	// Initialize open and closed lists
	open.insert(Node(*this->start));
}


template<class G, class S>
AStarRepairing<G, S>::~AStarRepairing()
{
	// Clean up
	open.clear();
	closed.clear();
	delete start;
}


template<class G, class S>
std::list<S> AStarRepairing<G, S>::nextSolution()
{
	bool solutionFound = false;

	while (!solutionFound)
	{
		// Expanding nodes
		while (!open.empty() && !solutionFound)
		{
			Node currentNode = *open.begin();
			open.erase(open.begin());
			// Check, whether node's f-cost is not greater than the current upper bound
			if (incumbent == nullptr || currentNode.f < incumbent->g)
			{
				auto currentNodeIterator = closed.insert(currentNode);

				std::list<S>&& adjtmp = graph->adjacent(currentNode.thisNode);
				size_t adjcnt = adjtmp.size();

				auto it = adjtmp.begin();
				Node* successors = new Node[adjtmp.size()];
				for (int i = 0; it != adjtmp.end(); i++, it++)
				{
					successors[i].thisNode = *it;
					successors[i].parentNode = &*currentNodeIterator;
					successors[i].g = currentNode.g + graph->cFunction(currentNode.thisNode, *it);
					successors[i].f = successors[i].g + graph->hFunction(*it);
					successors[i].fw = successors[i].g + w * graph->hFunction(*it);
				}

				// Examinating successors
				for (int i = 0; i < adjcnt; i++)
				{
					// Goal node is found
					if (graph->isGoal(successors[i].thisNode))
					{
						solutionFound = true;
						if (incumbent != nullptr)
							delete incumbent;
						incumbent = new Node(successors[i]);
						break;
					}

					// Putting ready to reexpand nodes to the inconsistent list
					auto placeInClosed = closed.find(successors[i]);
					auto placeInOpen = std::find_if(open.begin(), open.end(), [&successors, &i](const Node& n) {
						return successors[i].thisNode == n.thisNode;
					});
					const bool newNode = placeInClosed == closed.end() && placeInOpen == open.end();

					if (placeInClosed != closed.end() && placeInClosed->g > successors[i].g)
					{
						inconsistent.push_back(successors[i]);
						closed.erase(placeInClosed);
					}

					if (placeInOpen != open.end())
					{
						inconsistent.push_back(successors[i]);
						open.erase(placeInOpen);
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

		if (inconsistent.empty())
			break;
		
		for each (auto x in inconsistent)
		{
			open.insert(x);
		}
		inconsistent.clear();
	}

	if (!solutionFound)
		throw std::runtime_error("No solution");

	// Constructing the solution
	std::list<S> solution;
	const Node* currentNode = incumbent;
	while (currentNode != nullptr)
	{
		solution.push_front(currentNode->thisNode);
		currentNode = currentNode->parentNode;
	}

	// Calculate the error bound
	error = incumbent->f - open.begin()->f;
	
	// Decreasing the weight
	if (w != 1)
	{
		w -= weightDecreaseRate;
		if (w < 1)
			w = 1;
	}

	return solution;
}
