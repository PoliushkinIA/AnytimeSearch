// Anytime Weighted A* with restarting
#pragma once
#include "SearchBase.h"
#include "stdafx.h"
template<class G, class S>
class RBFSRestarting :
	public SearchBase<G, S>
{
public:
	RBFSRestarting(
		G*     graph,
		S      start,
		double w
		);
	~RBFSRestarting();
	std::list<S> nextSolution();
protected:
	struct Node
	{
		S thisNode;
		const Node* parentNode;
		double g;
		double f;
		double F;
		bool operator <(const Node x) const
		{
			return F < x.F;
		}
	};
	struct NodeComparator
	{
		bool operator()(const Node& lhs, const Node& rhs) const
		{
			return lhs.thisNode < rhs.thisNode;
		}
	};
	Node* RBFS(Node* currentNode, double limit, double& newLimit);
	Node* start;
	Node* incumbent;
	double w;
};



template<class G, class S>
RBFSRestarting<G, S>::RBFSRestarting(
	G*     graph,
	S      start,
	double w
	) :
	SearchBase(graph), w(w)
{
	this->start = new Node();
	this->start->thisNode = start;
	this->start->parentNode = nullptr;
	this->start->g = 0;
	this->start->f = graph->hFunction(start);
	this->start->F = this->start->f;
	incumbent = nullptr;
}


template<class G, class S>
RBFSRestarting<G, S>::~RBFSRestarting()
{
	delete start;
}


template<class G, class S>
std::list<S> RBFSRestarting<G, S>::nextSolution()
{
	double lim;
	Node* res = RBFS(start, std::numeric_limits<double>::infinity(), lim);

	if (res == nullptr)
		throw std::runtime_error("No solution");

	incumbent = res;

	// Constructing the solution
	std::list<S> solution;
	const Node* currentNode = incumbent;
	while (currentNode != nullptr)
	{
		solution.push_front(currentNode->thisNode);
		const Node* tmp = currentNode;
		currentNode = currentNode->parentNode;
		if (tmp != start)
			delete tmp;
	}


	return solution;
}

template<class G, class S>
typename RBFSRestarting<G, S>::Node* RBFSRestarting<G, S>::RBFS(typename RBFSRestarting<G, S>::Node* currentNode, double limit, double& newF)
{
	auto comp = [](const Node* lhs, const Node* rhs) {
		return *lhs < *rhs; 
	};
	
	if (graph->isGoal(currentNode->thisNode))
	{
		newF = currentNode->f;
		return currentNode;
	}
	
	std::list<S>&& adjtmp = graph->adjacent(currentNode->thisNode);
	size_t adjcnt = adjtmp.size();

	// if there is no successors return failure
	if (adjcnt == 0)
	{
		newF = std::numeric_limits<double>::infinity();
		return nullptr;
	}

	std::vector<Node*> successors;
	successors.reserve(adjcnt);
	for (auto it = adjtmp.begin(); it != adjtmp.end(); it++)
	{
		Node* node = new Node();
		node->thisNode = *it;
		node->parentNode = currentNode;
		node->g = currentNode->g + graph->cFunction(currentNode->thisNode, *it);
		node->f = node->g + graph->hFunction(*it);
		// Prune a subtree that leads to a solution which costs more than incumbent
		if (incumbent != nullptr && incumbent->f <= node->f)
			node->F = std::numeric_limits<double>::infinity();
		else if (currentNode->f < currentNode->F)
			node->F = node->f > currentNode->F ? node->f : currentNode->F;
		else
			node->F = node->f;
		successors.push_back(node);
	}

	double Falt, galt;
	std::sort(successors.begin(), successors.end(), comp);
	if (successors.size() > 1)
	{
		Falt = successors[1]->F;
		galt = successors[1]->g;
	}
	while ((incumbent == nullptr || successors[0]->F < incumbent->f) && successors[0]->g + w*(successors[0]->F-successors[0]->g) <= limit)
	{
		double newFReturn;
		Node* result;
		if (successors.size() == 1)
			result = RBFS(successors[0], limit, newFReturn);
		else
			result = RBFS(successors[0], limit < galt + w*(Falt - galt) ? limit : galt + w*(Falt - galt), newFReturn);
		if (result != nullptr)
		{
			for (auto it = ++successors.begin(); it != successors.end(); it++)
				delete *it;
			return result;
		}
		successors[0]->F = newFReturn;
		std::sort(successors.begin(), successors.end(), comp);
		if (successors.size() > 1)
		{
			Falt = successors[1]->F;
			galt = successors[1]->g;
		}
	}
	newF = successors[0]->F;
	for each (Node* elem in successors)
		delete elem;
	return nullptr;
}