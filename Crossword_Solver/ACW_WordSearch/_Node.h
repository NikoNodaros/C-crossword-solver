#pragma once
//#include "WordSearch.h"
#include <vector>
#include<string>
class _Node
{
	friend class Wordsearch;

	std::vector<_Node *> _SubNodes;
	_Node * const _ParentNode;
	char _Letter;
	bool _IsFinal;
public:
	_Node & operator = (_Node & other);//Assignment operator
	//_Node(const Node & other);// Copy ctor
	_Node(_Node * const parent, char const letter, bool const isfinal);
	~_Node();
};

