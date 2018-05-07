// Anytime Weighted A* with restarting
#pragma once
#include "SearchBase.h"
#include "stdafx.h"
class AStarRestarting :
	public SearchBase
{
public:
	AStarRestarting(
		void*  graph,
		std::list<void*>* (*adjacent)(void*, void*),
		double (*hFunction)(void*, void*),
		double (*cFunction)(void*, void*, void*),
		bool   (*isGoal)(void*, void*),
		bool   (*isEqual)(void*, void*, void*),
		void*  start,
		double w
		);
	~AStarRestarting();
	std::list<void*>* nextSolution();
protected:
	struct Node
	{
		void* thisNode;
		Node* parentNode;
		double g;
		double f;
		double fw;
		bool operator <(const Node x) const
		{
			return fw < x.fw;
		}
	};
	//void insert(Node* n);
	//std::list<Node*>* open;
	std::multiset<Node> open;
	std::list<Node> closed;
	Node* start;
	Node* incumbent;
	double w;
};

