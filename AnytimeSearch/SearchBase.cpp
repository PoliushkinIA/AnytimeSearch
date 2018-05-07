#include "stdafx.h"
#include "SearchBase.h"


SearchBase::SearchBase(
	void*  graph,
	std::list<void*>* (*adjacent)(void*, void*),
	double (*hFunction)(void*, void*),
	double (*cFunction)(void*, void*, void*),
	bool   (*isGoal)(void*, void*),
	bool   (*isEqual)(void*, void*, void*),
	void*  start
	)
{
	this->graph = graph;
	this->adjacent = adjacent;
	this->hFunction = hFunction;
	this->cFunction = cFunction;
	this->isGoal = isGoal;
	this->isEqual = isEqual;
}


SearchBase::~SearchBase()
{
}
