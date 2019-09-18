#include "WordSearch.h"
#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <tuple>
#include <stdlib.h>
#include <time.h>
using namespace std;

WordSearch::WordSearch(std::string const filename, std::string const dict, std::string const puzzle) :
	_OutPutFileName(filename),
	_DictionarySize(static_cast<unsigned long long>(0)),
	_PuzzleName(puzzle),
	_DictionaryName(dict),
	_ExpGrid(nullptr),
	_Dim(0),
	_NumberOfGridCellsVisited(0),
	_NumberOfDictEntriesVisited(0),
	_StartX(0),
	_StartY(0)
{
	
}

WordSearch::~WordSearch() {
	delete[]  _ExpGrid;
}

void WordSearch::ReadSimplePuzzle() {
	ifstream fileStream;
	fileStream.open(_PuzzleName);
	fileStream >> _Dim;
	_ExpGrid = new char[_Dim * _Dim];
	for (int i = 0; i < _Dim; i++)
	{
		for (int j = 0; j < _Dim; j++)
		{
			fileStream >> _ExpGrid[((_Dim * i) + j)];
		}
	}
	fileStream.close();
}

void WordSearch::ReadSimpleDictionary() {
	ifstream fileStream;
	fileStream.open(_DictionaryName);
	std::string line;
	while (fileStream >> line)
	{
		_DictionaryList.push_back(line);
	}
	fileStream.close();
	_DictionarySize = _DictionaryList.size();
	_FoundWords.reserve(_DictionarySize);
}

void WordSearch::ReadAdvancedPuzzle() const
{
	//cout << "Advanced Puzzle Not Implemented" << endl << endl;
}

void WordSearch::ReadAdvancedDictionary()
{
	ifstream fileStream;
	fileStream.open(_DictionaryName);
	bool nodeAlreadyExists; 
	std::string line;
	while (fileStream >> line)
	{
		++_DictionarySize;
		nodeAlreadyExists = false;

		for (Node * const primaryNode : _PrimaryNodes)
		{
			if (line[0] == primaryNode->_Letter)
			{
				nodeAlreadyExists = true;
				Node * parentNode = primaryNode;
				bool branching = false;
				
				int k = 1;
				for (int i = 0; i < parentNode->_SubNodes.size(); ++i)
				{
					if (line[k] == parentNode->_SubNodes[i]->_Letter)
					{
						parentNode = parentNode->_SubNodes[i];
						i = -1;
						++k;
						continue;
					}
					else if (i == parentNode->_SubNodes.size() - 1)
					{
						branching = true;
					}
				}
				if (branching) { BranchBuildier(string(line.begin() + k, (line.begin() + (line.size()))), parentNode); break; }
				else parentNode->_IsFinal = true;
				break;
			}
		}
		if(!nodeAlreadyExists)
		{
			_PrimaryNodes.push_back(new Node(nullptr, line[0], (line.size() <= 1)));
			Node * const lastNode = _PrimaryNodes[_PrimaryNodes.size() - 1]; 
			BranchBuildier(string(line.begin() + 1, (line.begin() + (line.size()))), lastNode);
		}
	}
	fileStream.close();
	_FoundNodes.reserve(_DictionarySize);
	ReadSimplePuzzle();
}

void WordSearch::SolvePuzzleSimple() {
	for (int i = 0; i < _DictionaryList.size(); ++i)
	{
		_NumberOfDictEntriesVisited++;
		string const word = _DictionaryList[i];
		char const secondChar = word[1];
		for (int j = 0; j < _Dim; ++j)
		{
			for (int k = 0; k < _Dim; ++k)
			{
				_NumberOfGridCellsVisited++;
				if (_ExpGrid[(_Dim * j) + k] == word[0])
				{
					_StartX = j; _StartY = k;
					int* const X = &j;
					int* const Y = &k;

					if (_ExpGrid[(_Dim * (j)) + (k - 1)] == secondChar)     { ValidateWord(*X, *Y - 1, 0, word); }// WW
					if (_ExpGrid[(_Dim * (j)) + (k + 1)] == secondChar)     { ValidateWord(*X, *Y + 1, 1, word); }// EE
					if (_ExpGrid[(_Dim * (j - 1)) + (k)] == secondChar)     { ValidateWord(*X - 1, *Y, 2, word); }// NN
					if (_ExpGrid[(_Dim * (j + 1)) + (k)] == secondChar)     { ValidateWord(*X + 1, *Y, 3, word); }// SS
					if (_ExpGrid[(_Dim * (j + 1)) + (k + 1)] == secondChar) { ValidateWord(*X + 1, *Y + 1, 4, word); }// SE
					if (_ExpGrid[(_Dim * (j + 1)) + (k - 1)] == secondChar) { ValidateWord(*X + 1, *Y - 1, 5, word); }// SW
					if (_ExpGrid[(_Dim * (j - 1)) + (k + 1)] == secondChar) { ValidateWord(*X - 1, *Y + 1, 6, word); }// NE
					if (_ExpGrid[(_Dim * (j - 1)) + (k - 1)] == secondChar) { ValidateWord(*X - 1, *Y - 1, 7, word); }// NW
					_NumberOfGridCellsVisited += 8;
				}
			}
		}
	}
}

void WordSearch::ValidateWord(int pX, int pY, int const pDirection, std::string const &pWord)
{
	int wordIndex = 1;
	for (int i = 0; i < pWord.size(); i++)
	{
		if (((pX > _Dim || pY > _Dim) || (pX < 0 || pY < 0)) && wordIndex < pWord.size()) { break; }
		if (wordIndex == pWord.size()) { _FoundWords.push_back(std::tuple<string, int, int, int>(pWord, _StartX, _StartY, pDirection)); break; }
		_NumberOfGridCellsVisited++;
		if (_ExpGrid[(_Dim * (pX) +pY)] == pWord[wordIndex])
		{
			switch (pDirection)
			{
				case 0: --pY;			  break;
				case 1: ++pY;			  break;
				case 2: --pX;			  break;
				case 3: ++pX;			  break;
				case 4: ++pX;		++pY; break;
				case 5: ++pX;		--pY; break;
				case 6: --pX;		++pY; break;
				case 7: --pX;		--pY; break;
				default:
					break;
			}
			++wordIndex;
		}
	}
}

void WordSearch::SolvePuzzleAdvanced()
{
	for (Node const * const firstNode : _PrimaryNodes)// For primary each node
	{
		++_NumberOfDictEntriesVisited;

		for (int j = 0; j < _Dim; ++j)// Loop Rows
		{
			for (int k = 0; k < _Dim; ++k)// Loop Columns
			{
				++_NumberOfGridCellsVisited;

				if (_ExpGrid[(_Dim * j) + k] == firstNode->_Letter)// If first node matches ( X , Y )
				{
					for (size_t n = 0; n < firstNode->_SubNodes.size(); ++n)// For nodes of first node
					{
						++_NumberOfDictEntriesVisited;

						Node const * const secondNode = firstNode->_SubNodes[n];

						_StartX = j; _StartY = k;
						int* const X = &j;
						int* const Y = &k;

						// If second node matches ( X , Y )
						if (_ExpGrid[(_Dim * (j)) + (k - 1)] == secondNode->_Letter) { ValidateNode(*X, *Y - 2, 0, secondNode); }// WW
						if (_ExpGrid[(_Dim * (j)) + (k + 1)] == secondNode->_Letter) { ValidateNode(*X, *Y + 2, 1, secondNode); }// EE
						if (_ExpGrid[(_Dim * (j - 1)) + (k)] == secondNode->_Letter) { ValidateNode(*X - 2, *Y, 2, secondNode); }// NN
						if (_ExpGrid[(_Dim * (j + 1)) + (k)] == secondNode->_Letter) { ValidateNode(*X + 2, *Y, 3, secondNode); }// SS
						if (_ExpGrid[(_Dim * (j + 1)) + (k + 1)] == secondNode->_Letter) { ValidateNode(*X + 2, *Y + 2, 4, secondNode); }// SE
						if (_ExpGrid[(_Dim * (j + 1)) + (k - 1)] == secondNode->_Letter) { ValidateNode(*X + 2, *Y - 2, 5, secondNode); }// SW
						if (_ExpGrid[(_Dim * (j - 1)) + (k + 1)] == secondNode->_Letter) { ValidateNode(*X - 2, *Y + 2, 6, secondNode); }// NE
						if (_ExpGrid[(_Dim * (j - 1)) + (k - 1)] == secondNode->_Letter) { ValidateNode(*X - 2, *Y - 2, 7, secondNode); }// NW
						_NumberOfGridCellsVisited += 8;
					}
				}
			}
		}
	}
}
void WordSearch::ValidateNode(int pX, int pY, int const pDirection, Node const * pNode)
{
	Node const * foundNode = (pNode->_IsFinal) ? (pNode) : nullptr;
	size_t numberOfSubNodes = pNode->_SubNodes.size();
	for (int n = 0;  n < numberOfSubNodes; ++n)// For subnodes
	{
		if (((pX > _Dim || pY > _Dim) || (pX < 0 || pY < 0))) { break; }// Break if cell is out of bounds
		++_NumberOfDictEntriesVisited;
		
		Node const * const subNode = pNode->_SubNodes[n];

		
		++_NumberOfGridCellsVisited;
		
		if (_ExpGrid[(_Dim * (pX) + pY )] == subNode->_Letter)
		{
			if (subNode->_IsFinal) { foundNode = subNode;}
			switch (pDirection)
			{
				case 0: --pY;			  break;
				case 1: ++pY;			  break;
				case 2: --pX;			  break;
				case 3: ++pX;			  break;
				case 4: ++pX;		++pY; break;
				case 5: ++pX;		--pY; break;
				case 6: --pX;		++pY; break;
				case 7: --pX;		--pY; break;
				default:
					break;
			}
			pNode = subNode;
			n = -1; 
			numberOfSubNodes = pNode->_SubNodes.size();
		}
	}
	if (foundNode) 
	{ _FoundNodes.push_back(std::tuple<Node const *, int, int, int>(foundNode, _StartX, _StartY, pDirection));}
}
void WordSearch::WriteResults(double const loadTime, double const solveTime)
{
	FileOutput(loadTime, solveTime);
	ConsoleOutput(loadTime, solveTime);
}
void WordSearch::FileOutput(double const loadTime, double const solveTime)
{
	if (_OutPutFileName == "output_advanced.txt")
	{
		ReadSimpleDictionary();
		for (auto const node : _FoundNodes)
		{
			string reverse = "";
			Node const * notPrimaryNode = get<0>(node);
			int x = get<1>(node), y = get<2>(node), dir = get<3>(node);
			while (notPrimaryNode)
			{
				reverse += notPrimaryNode->_Letter;
				notPrimaryNode = notPrimaryNode->_ParentNode;
			}
			string word(reverse.rbegin(), reverse.rend());
			_FoundWords.push_back(std::tuple<string, int, int, int>(word, x, y, dir));
		}
	}

	double const lTime = loadTime, sTime = solveTime;
	ofstream outFile;
	outFile.open(_OutPutFileName);
	outFile
		<< "NUMBER_OF_WORDS_MATCHED " << _FoundWords.size() << endl
		<< endl
		<< "WORDS_MATCHED_IN_GRID" << endl;
	for each (auto const word in _FoundWords)
	{

		outFile << get<1>(word) << " " << get<2>(word) << " " << get<0>(word) << endl;
	}

	outFile
		<< endl
		<< "WORDS_UNMATCHED_IN_GRID" << endl;
	for each (string const dictEntry in _DictionaryList)
	{
		bool isFound = false;
		for each (auto const foundWord in _FoundWords)
		{
			string word = get<0>(foundWord);
			if (word == dictEntry)
			{
				isFound = true;
				break;
			}
		}
		if (!isFound)outFile << dictEntry << endl;
	}
	outFile
		<< endl
		<< "NUMBER_OF_GRID_CELLS_VISITED" << " " << _NumberOfGridCellsVisited << endl
		<< endl

		<< "NUMBER_OF_DICTIONARY_ENTRIES_VISITED" << " " << _NumberOfDictEntriesVisited << endl
		<< endl

		<< "TIME_TO_POPULATE_GRID_STRUCTURE" << " " << lTime << endl
		<< endl

		<< "TIME_TO_SOLVE_PUZZLE" << " " << sTime << endl
		<< endl;
	outFile.close();
}



void WordSearch::ConsoleOutput(double const loadTime, double const solveTime)const
{
	cout
		<< (_OutPutFileName) << endl

		<< "Puzzle Size: " << _Dim << "x" << _Dim << endl
		<< endl


		<< "Load Time" << endl
		<< loadTime << endl
		<< endl


		<< "Solve Time" << endl
		<< solveTime << endl
		<< endl


		<< "Grid Cells Visited" << endl
		<< _NumberOfGridCellsVisited << endl
		<< endl

		<< "Dictionary Entries Visited" << endl
		<< _NumberOfDictEntriesVisited << endl
		<< endl

		<< "Words Found (" << _FoundWords.size() << "/" << _DictionarySize << "): " << endl
		<< endl;

	for each (auto const word in _FoundWords)
	{
		cout << "(" << get<1>(word) << ", " << get<2>(word) << ")" << "  "
			<< get<0>(word) << endl << endl;
	}
	std::vector<std::tuple<char, int, int>> lettersToPrint;
	int x = 0, y = 0;
	for (int k = 0; k < _FoundWords.size(); k++)
	{

		auto const entry = _FoundWords[k];

		x = get<1>(entry);
		y = get<2>(entry);


		for each (char const letter in get<0>(entry))
		{
			lettersToPrint.push_back(tuple<char, int, int>(letter, x, y));
			switch (get<3>(entry))
			{
				case 0: y--; break;
				case 1: y++; break;
				case 2: x--; break;
				case 3: x++; break;
				case 4: x++; y++; break;
				case 5: x++; y--; break;
				case 6: x--; y++; break;
				case 7: x--; y--; break;
				default:
					break;
			}
		}
	}
	HANDLE const hConsole = GetStdHandle(static_cast<unsigned long>(-11));// Gets the STD_OUTPUT_HANDLE

	for (int i = 0; i < _Dim; i++)
	{
		for (int j = 0; j < _Dim; j++)
		{
			bool letterFound = false;
			for (int k = 0; k < lettersToPrint.size(); k++)
			{
				auto const letterTuple = lettersToPrint[k];
				char const letter = get<0>(letterTuple);
				x = get<1>(letterTuple), y = get<2>(letterTuple);
				if (i == x && j == y)
				{
					if (!letterFound)
					{
						letterFound = true;
						SetConsoleTextAttribute(hConsole, 12);

						cout << "" << letter << " ";
						lettersToPrint.erase(lettersToPrint.begin() + k);
					}
					else
					{
						lettersToPrint.erase(lettersToPrint.begin() + k);
					}
					k--;
				}
			}


			if (!letterFound)
			{
				SetConsoleTextAttribute(hConsole, 7);
				cout << "" << _ExpGrid[((_Dim * i) + j)] << " ";
			}
		}
		cout << endl;
	}

	SetConsoleTextAttribute(hConsole, 7);

	cout
		<< endl
		<< "Press Return Key Twice to Continue/Exit" << endl;
	cin.get();
}
void WordSearch::CreateRandomPuzzle(int const pDim)const
{
	ofstream customPuzzle;
	customPuzzle.open("custom.txt");
	char c;
	srand(static_cast<unsigned int>(time(nullptr)));
	customPuzzle << pDim << endl;
	for (int i = 0; i < pDim; i++)
	{
		for (int j = 0; j < pDim; j++)
		{
			c = (rand() % 26 + 'a');
			customPuzzle << c;
		}
		customPuzzle << endl;
	}
	customPuzzle.close();
}

void WordSearch::BranchBuildier(std::string const &word, Node * const node) const
{
	Node * lastNode = node;
	for (int l = 0; l < word.size(); l++)
	{
		Node * const newNode = new Node(lastNode, word[l], (l == (word.size() - 1)) );
		lastNode->_SubNodes.push_back(newNode);
		lastNode = newNode;
	}
}
