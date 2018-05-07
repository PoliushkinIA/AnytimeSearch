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
		//int positions[N][N];
		std::array<std::array<int, N>, N> positions;
		int operator ==(const State &s) const
		{
			return positions == s.positions;
		}
	};
	FifteenPuzzle(int _start[N][N]);
	~FifteenPuzzle();
	std::list<State> adjacent(State&);
	double hFunction(State&);
	double cFunction(State&, State&);
	bool   isGoal(State&);
	bool   getSolution();
	bool   isCurrentSolutionBest() { return bestSolution; }
	std::list<std::vector<State>> solutions;
	SearchBase<FifteenPuzzle, State>* search;
	State start;
protected:
	bool bestSolution = false;
};

