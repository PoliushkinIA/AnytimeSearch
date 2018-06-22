// Basic class for search algorithm
#pragma once
#include "stdafx.h"
template<class G, class S>
class SearchBase
{
public:
	SearchBase(G* graph);
	virtual ~SearchBase();
	virtual std::list<S> nextSolution() = 0;
	double error;
protected:
	G* graph;
};

template<class G, class S>
SearchBase<G, S>::SearchBase(G* graph)
{
	this->graph = graph;
}


template<class G, class S>
SearchBase<G, S>::~SearchBase()
{
}
