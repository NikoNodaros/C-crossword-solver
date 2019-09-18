#pragma once
#include <string>
#include <vector>

class WordSearch {
	// Add your code here
	struct Node
	{
		std::vector<Node *> _SubNodes;
		Node * const _ParentNode;
		char _Letter;
		bool _IsFinal;
		Node & operator= (Node & obj) = delete;//Assignment operator
		Node(Node const& obj) = delete;// Copy ctor

	public:
		Node(Node * const parentNode, char const letter, bool const isFinal):
			_ParentNode(parentNode),
			_Letter(letter),
			_IsFinal(isFinal)
		{
		}
	};
	std::vector<std::string> _DictionaryList;
	std::vector<std::tuple<std::string, int, int, int>> _FoundWords;
	std::vector<Node *> _AllNodes;
	std::vector<Node *> _PrimaryNodes;
	std::vector<std::tuple<Node const* const, int, int, int>> _FoundNodes;

	std::string const _OutPutFileName;
	size_t _DictionarySize;
	
	std::string const _PuzzleName;
	std::string const  _DictionaryName;
	char* _ExpGrid;

	int _Dim;
	int _StartX = 0, _StartY = 0;
	
	WordSearch & operator= (WordSearch & obj) = delete;//Assignment operator
	WordSearch(WordSearch const & obj) = delete;// Copy ctor

public:
	explicit WordSearch(std::string const filename, std::string const dict, std::string const puzzle);
	~WordSearch();
	void ReadSimplePuzzle();
	void ReadSimpleDictionary();
	void ReadAdvancedPuzzle()const;
	void ReadAdvancedDictionary();
	void SolvePuzzleSimple();
	void SolvePuzzleAdvanced();
	void WriteResults(double const loadTime, double const solveTime);

	void ValidateNode(int pX, int pY, int const pDirection, Node const * pNode);
	void ValidateWord(int pX, int pY, int const pDirection, std::string const &pWord);
	void FileOutput(double const loadTime, double const solveTime);
	void ConsoleOutput(double const loadTime, double const solveTime) const;
	void CreateRandomPuzzle(int const pDim)const;
	void BranchBuildier(std::string const &word, Node * const arc) const;
	double _NumberOfGridCellsVisited = 0;
	double _NumberOfDictEntriesVisited = 0;
};
