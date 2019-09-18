#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
using namespace std;

#include "WordSearch.h"
#include "ACW_Wrapper.h"

int main(int, char **) {
	int numOfTests = 1; //Increase to get a more accuarate solve time, but comment out line 435 in WordSearch.cpp
	ACW_Wrapper wrapper("log.txt");
	std::string const output = "output.txt";
	
	std::vector<string> sizes = { "9","15","25","50" };
	std::string const categories[4] = {"        Grid Cells Visited","Dictionary Entries Checked","             Load Time (s)","            Solve Time (s)"};
	
	//Grid Cells, Dict Entries, Load Time, Solve Time
	std::vector<vector<double>> simpleResults;
	std::vector<vector<double>> advancedResults;
	
	
	for (int i = 0; i < sizes.size(); ++i) 
	{

		const string dict = "dictionary" + sizes[i] + ".txt";
		const string puzzle = "wordsearch_grid" + sizes[i] + ".txt";
		
		std::vector<double> loadTimes;
		std::vector<double> solveTimes;

		double avgLoadTime = 0, avgSolveTime = 0;
		double entries = 0, cells = 0;

		for (int i = 0; i < numOfTests; i++)
		{

			// Test simple algorithm
			{
				WordSearch ws("output_simple.txt", dict, puzzle);

				wrapper.startTimer();
				ws.ReadSimplePuzzle();
				ws.ReadSimpleDictionary();
				const double loadTime = wrapper.stopTimer();

				wrapper.startTimer();
				ws.SolvePuzzleSimple();
				const double solveTime = wrapper.stopTimer();
				ws.WriteResults(loadTime, solveTime);
				loadTimes.push_back(loadTime);
				solveTimes.push_back(solveTime);
				entries = ws._NumberOfDictEntriesVisited;
				cells = ws._NumberOfGridCellsVisited;
			}
		}
		for (double count : loadTimes)
		{
			avgLoadTime += count;
		}

		avgLoadTime = avgLoadTime / (loadTimes.size());
		for (double count : solveTimes)
		{
			avgSolveTime += count;

		}
		avgSolveTime = avgSolveTime / (solveTimes.size());

		std::vector<double> v = { cells, entries, avgLoadTime, avgSolveTime };
		simpleResults.push_back(v);

		avgLoadTime = 0; avgSolveTime = 0;
		loadTimes.clear();
		solveTimes.clear();

		for (int i = 0; i < numOfTests; i++)
		{

			// Test advanced algorithm
			{
				WordSearch ws("output_advanced.txt", dict, puzzle);

				wrapper.startTimer();
				ws.ReadAdvancedPuzzle();
				ws.ReadAdvancedDictionary();
				const double loadTime = wrapper.stopTimer();

				wrapper.startTimer();
				ws.SolvePuzzleAdvanced();
				const double solveTime = wrapper.stopTimer();

				ws.WriteResults(loadTime, solveTime);
				loadTimes.push_back(loadTime);
				solveTimes.push_back(solveTime);
				entries = ws._NumberOfDictEntriesVisited;
				cells = ws._NumberOfGridCellsVisited;
			}
		}
		avgSolveTime = avgSolveTime / (solveTimes.size());

		std::vector<double> a = { cells, entries, avgLoadTime, avgSolveTime };
		advancedResults.push_back(v);

		avgLoadTime = 0; avgSolveTime = 0;
		loadTimes.clear();
		solveTimes.clear();
	}

	ofstream testOutput;
	testOutput.open(output);

	testOutput << endl << '\t' << '\t' << '\t' << '\t' << '\t';
	
	for (string const size : sizes)
	{
		testOutput << size << '\t' << '\t';
	}
	testOutput 
		<< endl 
		<< endl
		<< '\t' <<"  Simple Algorithm"
		<< endl;
	
		
	for (size_t c = 0; c < 4; ++c)
	{

		testOutput << categories[c] << '\t' << '\t';
		for (auto const result : simpleResults)
		{
			testOutput << result[c] << '\t' << '\t';
		}
		testOutput << endl;
	}

	testOutput
		<< endl
		<< endl
		<< '\t' << "Advanced Algorithm"
		<< endl;

	for (size_t c = 0; c < 4; ++c)
	{

		testOutput << categories[c] << '\t' << '\t';
		for (auto const result : advancedResults)
		{
			testOutput << result[c] << '\t' << '\t';
		}
		testOutput << endl;

	}
	return 0;
}