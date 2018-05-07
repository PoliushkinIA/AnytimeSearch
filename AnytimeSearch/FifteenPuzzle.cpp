#include "stdafx.h"
#include "FifteenPuzzle.h"


FifteenPuzzle::FifteenPuzzle(int _start[N][N])
{
	start = new State();
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			start->positions[i][j] = _start[i][j];
}


FifteenPuzzle::~FifteenPuzzle()
{
	delete start;
	while (!allocatedStates.empty())
	{
		delete allocatedStates.top();
		allocatedStates.pop();
	}
}

std::list<void*>* FifteenPuzzle::adjacent(void* th, void* state)
{
	State* s = (State*)state;
	// Finding empty cell
	// Empty cell coordinates
	int i = 0, j = 0;
	for (; i < N; i++)
	{
		for (j = 0; j < N && s->positions[i][j] != 0; j++);
		if (j < N && s->positions[i][j] == 0)
			break;
	}
	std::list<void*>* res = new std::list<void*>();
	if (i != 0)
	{
		State* s1 = new State();
		for (int i1 = 0; i1 < N; i1++)
			for (int j1 = 0; j1 < N; j1++)
			{
				if (i1 == i&&j1 == j)
					s1->positions[i1][j1] = s->positions[i1 - 1][j1];
				else if (i1 == i - 1 && j1 == j)
					s1->positions[i1][j1] = 0;
				else
					s1->positions[i1][j1] = s->positions[i1][j1];
			}
		res->push_back((void*)s1);
	}
	if (i != N - 1)
	{
		State* s1 = new State();
		for (int i1 = 0; i1 < N; i1++)
			for (int j1 = 0; j1 < N; j1++)
			{
				if (i1 == i&&j1 == j)
					s1->positions[i1][j1] = s->positions[i1 + 1][j1];
				else if (i1 == i + 1 && j1 == j)
					s1->positions[i1][j1] = 0;
				else
					s1->positions[i1][j1] = s->positions[i1][j1];
			}
		res->push_back((void*)s1);
	}
	if (j != 0)
	{
		State* s1 = new State();
		for (int i1 = 0; i1 < N; i1++)
			for (int j1 = 0; j1 < N; j1++)
			{
				if (i1 == i&&j1 == j)
					s1->positions[i1][j1] = s->positions[i1][j1 - 1];
				else if (i1 == i && j1 == j - 1)
					s1->positions[i1][j1] = 0;
				else
					s1->positions[i1][j1] = s->positions[i1][j1];
			}
		res->push_back((void*)s1);
	}
	if (j != N - 1)
	{
		State* s1 = new State();
		for (int i1 = 0; i1 < N; i1++)
			for (int j1 = 0; j1 < N; j1++)
			{
				if (i1 == i&&j1 == j)
					s1->positions[i1][j1] = s->positions[i1][j1 + 1];
				else if (i1 == i && j1 == j + 1)
					s1->positions[i1][j1] = 0;
				else
					s1->positions[i1][j1] = s->positions[i1][j1];
			}
		res->push_back((void*)s1);
	}
	for (auto it = res->begin(); it != res->end(); it++)
	{
		((FifteenPuzzle*)th)->allocatedStates.push((State*)*it);
	}
	return res;
}

double FifteenPuzzle::hFunction(void* th, void* s)
{
	// Manhattan Distance
	State* _s = (State*)s;
	int h = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			int pos = _s->positions[i][j];
			h += std::abs(pos / N - i) + std::abs(pos % N - j);
		}
	return h;
}

double FifteenPuzzle::cFunction(void* th, void* s1, void* s2)
{
	// Cost for any move is 1
	return 1.0;
}

bool FifteenPuzzle::isGoal(void* th, void* s)
{
	State* _s = (State*)s;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (_s->positions[i][j] != (i * N + j))
				return false;
	return true;
}

bool FifteenPuzzle::isEqual(void* th, void* s1, void* s2)
{
	State* _s1 = (State*)s1;
	State* _s2 = (State*)s2;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (_s1->positions[i][j] != _s2->positions[i][j])
				return false;
	return true;
}

bool FifteenPuzzle::getSolution()
{
	try
	{
		std::list<void*>* solution = search->nextSolution();
		std::vector<State*>* vec = new std::vector<State*>();
		for (auto it = solution->begin(); it != solution->end(); it++)
			vec->push_back((State*)*it);
		delete solution;
		solutions.push_back(vec);
		return true;
	}
	catch (...)
	{
		return false;
	}

}
