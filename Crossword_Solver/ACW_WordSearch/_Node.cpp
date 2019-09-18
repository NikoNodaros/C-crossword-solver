#include "_Node.h"



_Node & _Node::operator=(_Node & other)
{
	// TODO: insert return statement here
	if (this != &other)
	{
		std::swap(_Letter, other._Letter);
		std::swap(_IsFinal, other._IsFinal);
	}
	return *this;
}

//Node::Node(const Node & other)
//{
//}



_Node::_Node(_Node * const parent, char const letter, bool const isfinal):
	_ParentNode(parent),
	_Letter(letter),
	_IsFinal(isfinal)
{
}


_Node::~_Node()
{
}
