// Basic class for search algorithm
#pragma once
#include "stdafx.h"
class SearchBase
{
public:
	SearchBase(
		void*  graph,
		std::list<void*>* (*adjacent)(void*, void*),
		double (*hFunction)(void*, void*),
		double (*cFunction)(void*, void*, void*),
		bool   (*isGoal)(void*, void*),
		bool   (*isEqual)(void*, void*, void*),
		void*  start
	);
	virtual ~SearchBase();
	virtual std::list<void*>* nextSolution() = 0;
protected:
	void*  graph;
	std::list<void*>* (*adjacent)(void*, void*);
	double (*hFunction)(void*, void*);
	double (*cFunction)(void*, void*, void*);
	bool   (*isGoal)(void*, void*);
	bool   (*isEqual)(void*, void*, void*);
};

