// Fifteen puzzle
#pragma once
#include "stdafx.h"
#include "SearchBase.h"
class FifteenPuzzle
{
public:
	static const int N = 3;
	struct State
	{
		int positions[N][N];
	};
	FifteenPuzzle(int _start[N][N]);
	~FifteenPuzzle();
	static std::list<void*>* adjacent(void*, void*);
	static double hFunction(void*, void*);
	static double cFunction(void*, void*, void*);
	static bool   isGoal(void*, void*);
	static bool   isEqual(void*, void*, void*);
	bool   getSolution();
	bool   isCurrentSolutionBest() { return bestSolution; }
	std::list<std::vector<State*>*> solutions;
	SearchBase* search;
	State* start;
protected:
	std::stack<State*> allocatedStates;
	bool bestSolution = false;
};

