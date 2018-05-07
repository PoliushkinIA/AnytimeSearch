// AnytimeSearch.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "FifteenPuzzle.h"
#include "AStarRestarting.h"
#define N 3

int main()
{
	int start[N][N] =/*
					 {
		{ 8,	14,	12,	9 },
		{ 7,	5,	0,	13 },
		{ 10,	15,	1,	11 },
		{ 4,	2,	6,	3 } };//*/
		/*
	{ {3,  7, 11, 15},
 {2, 13,  9,  4},
 {5, 14,  6, 12},
 {8, 10,  1,  0} };//*/
	//*
	{ {8,	6,	3},
	{0,	7,	2},
	{4,	1,	5} };//*/
	/*
	{ {1, 0, 2},
	{ 3,	4,	5 },
	{ 6,	7,	8 } };//*/
	/*
	{ { 3,	8,	1 },
	{ 4,	2,	7 },
	{ 0,	6,	5 } };//*/
	/*
	{ { 0,	7,	2 },
	{ 6,	4,	8 },
	{ 3,	5,	1 } };//*/
	FifteenPuzzle* puzzle = new FifteenPuzzle(start);
	AStarRestarting* search = new AStarRestarting(puzzle, puzzle->adjacent, puzzle->hFunction, puzzle->cFunction, puzzle->isGoal, puzzle->isEqual, puzzle->start, 1.5);
	puzzle->search = search;
	auto startTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds total = std::chrono::milliseconds::zero();
	while (puzzle->getSolution())
	{
		auto it = puzzle->solutions.end();
		it--;
		auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
		/*for each (auto var in **it)
		{
			for (int i = 0; i < N; i++, std::cout << std::endl)
				for (int j = 0; j < N; j++)
					std::cout << var->positions[i][j] << ' ';
			std::cout << "---------------------------------" << std::endl;
		}*/
		std::cout << "Solving took " << duration.count() << "ms\n" << "Solution length is " << (*it)->size() << std::endl;
		total += duration;
		startTime = std::chrono::high_resolution_clock::now();
	}
	std::cout << "Total " << puzzle->solutions.size() << " solutions\n"
		<< "Total time: " << total.count() << "ms" << std::endl;
	_gettch();
	return 0;
}

