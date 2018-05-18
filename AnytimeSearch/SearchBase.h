// Basic class for search algorithm
#pragma once
#include "stdafx.h"
template<class G, class N>
class SearchBase
{
public:
	SearchBase(G* graph);
	virtual ~SearchBase();
	virtual std::list<N> nextSolution() = 0;
	double error;
protected:
	G* graph;
};

template<class G, class N>
SearchBase<G, N>::SearchBase(G* graph)
{
	this->graph = graph;
}


template<class G, class N>
SearchBase<G, N>::~SearchBase()
{
}
