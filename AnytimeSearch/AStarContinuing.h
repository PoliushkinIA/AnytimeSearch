// Anytime Weighted A* with continuing
#pragma once
#include "SearchBase.h"
#include "stdafx.h"
template<class G, class S>
class AStarContinuing :
	public SearchBase<G, S>
{
public:
	AStarContinuing(
		G*     graph,
		S      start,
		double w
		);
	~AStarContinuing();
	std::list<S> nextSolution();
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
	Node* start;
	Node* incumbent;
	double w;
};



template<class G, class S>
AStarContinuing<G, S>::AStarContinuing(
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
AStarContinuing<G, S>::~AStarContinuing()
{
	// Clean up
	open.clear();
	closed.clear();
	delete start;
}


template<class G, class S>
std::list<S> AStarContinuing<G, S>::nextSolution()
{
	bool solutionFound = false;

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

				// Reexpanding node
				auto placeInClosed = closed.find(successors[i]);
				const bool newNode = placeInClosed == closed.end();

				if (placeInClosed != closed.end() && placeInClosed->g > successors[i].g)
				{
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

	return solution;
}
