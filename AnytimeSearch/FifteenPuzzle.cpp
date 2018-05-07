#include "stdafx.h"
#include "FifteenPuzzle.h"


FifteenPuzzle::FifteenPuzzle(int _start[N][N])
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			start.positions[i][j] = _start[i][j];
}


FifteenPuzzle::~FifteenPuzzle()
{
}

std::list<FifteenPuzzle::State> FifteenPuzzle::adjacent(State& s)
{
	// Finding empty cell
	// Empty cell coordinates
	int i = 0, j = 0;
	for (; i < N; i++)
	{
		for (j = 0; j < N && s.positions[i][j] != 0; j++);
		if (j < N && s.positions[i][j] == 0)
			break;
	}
	std::list<State> res;
	if (i != 0)
	{
		State s1;
		for (int i1 = 0; i1 < N; i1++)
			for (int j1 = 0; j1 < N; j1++)
			{
				if (i1 == i&&j1 == j)
					s1.positions[i1][j1] = s.positions[i1 - 1][j1];
				else if (i1 == i - 1 && j1 == j)
					s1.positions[i1][j1] = 0;
				else
					s1.positions[i1][j1] = s.positions[i1][j1];
			}
		res.push_back(s1);
	}
	if (i != N - 1)
	{
		State s1;
		for (int i1 = 0; i1 < N; i1++)
			for (int j1 = 0; j1 < N; j1++)
			{
				if (i1 == i&&j1 == j)
					s1.positions[i1][j1] = s.positions[i1 + 1][j1];
				else if (i1 == i + 1 && j1 == j)
					s1.positions[i1][j1] = 0;
				else
					s1.positions[i1][j1] = s.positions[i1][j1];
			}
		res.push_back(s1);
	}
	if (j != 0)
	{
		State s1;
		for (int i1 = 0; i1 < N; i1++)
			for (int j1 = 0; j1 < N; j1++)
			{
				if (i1 == i&&j1 == j)
					s1.positions[i1][j1] = s.positions[i1][j1 - 1];
				else if (i1 == i && j1 == j - 1)
					s1.positions[i1][j1] = 0;
				else
					s1.positions[i1][j1] = s.positions[i1][j1];
			}
		res.push_back(s1);
	}
	if (j != N - 1)
	{
		State s1;
		for (int i1 = 0; i1 < N; i1++)
			for (int j1 = 0; j1 < N; j1++)
			{
				if (i1 == i&&j1 == j)
					s1.positions[i1][j1] = s.positions[i1][j1 + 1];
				else if (i1 == i && j1 == j + 1)
					s1.positions[i1][j1] = 0;
				else
					s1.positions[i1][j1] = s.positions[i1][j1];
			}
		res.push_back(s1);
	}
	return res;
}

double FifteenPuzzle::hFunction(State& s)
{
	// Manhattan Distance
	int h = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			int pos = s.positions[i][j];
			h += std::abs(pos / N - i) + std::abs(pos % N - j);
		}
	return h;
}

double FifteenPuzzle::cFunction(State& s1, State& s2)
{
	// Cost for any move is 1
	return 1.0;
}

bool FifteenPuzzle::isGoal(State& s)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (s.positions[i][j] != (i * N + j))
				return false;
	return true;
}

/*bool FifteenPuzzle::isEqual(State&& s1, State&& s2)
{
	State* _s1 = (State*)s1;
	State* _s2 = (State*)s2;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (_s1->positions[i][j] != _s2->positions[i][j])
				return false;
	return true;
}*/

bool FifteenPuzzle::getSolution()
{
	try
	{
		std::list<State> solution = search->nextSolution();
		std::vector<State> vec;
		for (auto it = solution.begin(); it != solution.end(); it++)
			vec.push_back(*it);
		solutions.push_back(vec);
		return true;
	}
	catch (...)
	{
		return false;
	}

}
