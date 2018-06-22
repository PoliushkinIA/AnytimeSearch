// AnytimeSearch.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "FifteenPuzzle.h"
#include "AStarRestarting.h"
#include "AStarContinuing.h"
#include "AStarRepairing.h"
#include "RBFSRestarting.h"
#define N 3

int main()
{
	int start[N][N] =/*
					 {
		{ 8,	14,	9,	12 },
		{ 7,	5,	0,	13 },
		{ 10,	15,	1,	11 },
		{ 4,	2,	6,	3 } };//*/
		/*
		{
		{ 12,	7,	8,	1 },
		{ 14,	11,	13,	6 },
		{ 15,	5,	3,	9 },
		{ 0,	10,	2,	4 } };//*/
		/*
	{ {3,  7, 11, 15},
 {2, 13,  9,  4},
 {5, 14,  6, 12},
 {8, 10,  1,  0} };//*/
	/*
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
	//*
	{ { 0,	7,	2 },
	{ 6,	4,	8 },
	{ 3,	5,	1 } };//*/
	
	FifteenPuzzle* puzzle;
	SearchBase<FifteenPuzzle, FifteenPuzzle::State>* search;
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	std::chrono::microseconds total;


	std::cout << "Restarting A*\n";
	puzzle = new FifteenPuzzle(start);
	search = new AStarRestarting<FifteenPuzzle, FifteenPuzzle::State>(puzzle, puzzle->start, 10);
	puzzle->search = search;
	startTime = std::chrono::high_resolution_clock::now();
	total = std::chrono::microseconds::zero();
	while (puzzle->getSolution())
	{
		auto it = puzzle->solutions.end();
		it--;
		auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
		/*for each (auto var in **it)
		{
			for (int i = 0; i < N; i++, std::cout << std::endl)
				for (int j = 0; j < N; j++)
					std::cout << var->positions[i][j] << ' ';
			std::cout << "---------------------------------" << std::endl;
		}//*/
		std::cout << "Solving took " << duration.count() << "us\n" << "Solution length is " << it->size() << '\n' << "Error bound is " << search->error << std::endl;
		total += duration;
		startTime = std::chrono::high_resolution_clock::now();
	}
	std::cout << "Total " << puzzle->solutions.size() << " solutions\n"
		<< "Total time: " << total.count() << "us" << std::endl;
	delete puzzle;
	std::cout << "---------------------------------\n\n";


	std::cout << "Continuing A*\n";
	puzzle = new FifteenPuzzle(start);
	search = new AStarContinuing<FifteenPuzzle, FifteenPuzzle::State>(puzzle, puzzle->start, 10);
	puzzle->search = search;
	startTime = std::chrono::high_resolution_clock::now();
	total = std::chrono::microseconds::zero();
	while (puzzle->getSolution())
	{
		auto it = puzzle->solutions.end();
		it--;
		auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
		/*for each (auto var in **it)
		{
		for (int i = 0; i < N; i++, std::cout << std::endl)
		for (int j = 0; j < N; j++)
		std::cout << var->positions[i][j] << ' ';
		std::cout << "---------------------------------" << std::endl;
		}*/
		std::cout << "Solving took " << duration.count() << "us\n" << "Solution length is " << it->size() << '\n' << "Error bound is " << search->error << std::endl;
		total += duration;
		startTime = std::chrono::high_resolution_clock::now();
	}
	std::cout << "Total " << puzzle->solutions.size() << " solutions\n"
		<< "Total time: " << total.count() << "us" << std::endl;
	delete puzzle;
	std::cout << "---------------------------------\n\n";


	std::cout << "Repairing A*\n";
	puzzle = new FifteenPuzzle(start);
	search = new AStarRepairing<FifteenPuzzle, FifteenPuzzle::State>(puzzle, puzzle->start, 10);
	puzzle->search = search;
	startTime = std::chrono::high_resolution_clock::now();
	total = std::chrono::microseconds::zero();
	while (puzzle->getSolution())
	{
		auto it = puzzle->solutions.end();
		it--;
		auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
		/*for each (auto var in **it)
		{
		for (int i = 0; i < N; i++, std::cout << std::endl)
		for (int j = 0; j < N; j++)
		std::cout << var->positions[i][j] << ' ';
		std::cout << "---------------------------------" << std::endl;
		}*/
		std::cout << "Solving took " << duration.count() << "us\n" << "Solution length is " << it->size() << '\n' << "Error bound is " << search->error << std::endl;
		total += duration;
		startTime = std::chrono::high_resolution_clock::now();
	}
	std::cout << "Total " << puzzle->solutions.size() << " solutions\n"
		<< "Total time: " << total.count() << "us" << std::endl;
	delete puzzle;
	std::cout << "---------------------------------\n\n";


	std::cout << "Anytime WRBFS\n";
	puzzle = new FifteenPuzzle(start);
	search = new RBFSRestarting<FifteenPuzzle, FifteenPuzzle::State>(puzzle, puzzle->start, 1);
	puzzle->search = search;
	startTime = std::chrono::high_resolution_clock::now();
	total = std::chrono::microseconds::zero();
	while (puzzle->getSolution())
	{
		auto it = puzzle->solutions.end();
		it--;
		auto endTime = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
		/*for each (auto var in **it)
		{
		for (int i = 0; i < N; i++, std::cout << std::endl)
		for (int j = 0; j < N; j++)
		std::cout << var->positions[i][j] << ' ';
		std::cout << "---------------------------------" << std::endl;
		}*/
		std::cout << "Solving took " << duration.count() << "us\n" << "Solution length is " << it->size() << '\n' << "Error bound is " << search->error << std::endl;
		total += duration;
		startTime = std::chrono::high_resolution_clock::now();
	}
	std::cout << "Total " << puzzle->solutions.size() << " solutions\n"
		<< "Total time: " << total.count() << "us" << std::endl;
	_gettch();
	return 0;
}

