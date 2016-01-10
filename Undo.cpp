// Undo class implementation
// Adam Levy

#include "Undo.h"

Undo::Undo()
{
	first = NULL;
	position = NULL;
	numNodes = 0;
}

Undo::Undo( Sudoku board )
{
	numNodes = 1;
	first = new Node;
	position = first;
	first->board = board;
       	first->next = NULL;
	first->previous = first;
}

Undo::~Undo()
{
	deletePreviousNodes( first );

	first = NULL;
	position = NULL;
}

// recursiveluy delete nodes above given node
void Undo::deleteNextNodes( Node * start )
{
	if( NULL == start )
	{
		return;
	}

	else if( start->next == start )
	{
		delete start;
		numNodes--;
		return;
	}
	else
	{
		deleteNextNodes( start->next );
		delete start;
	}
}

// recursively delete nodes below given node
void Undo::deletePreviousNodes( Node * start )
{
	if( NULL == start )
	{
		return;
	}

	else if( start->previous == start )
	{
		delete start;
		numNodes--;
		return;
	}
	else
	{
		deletePreviousNodes( start->previous );
		delete start;
	}
}

// Returns the top of the list
Sudoku Undo::undo()
{
	Sudoku save;

	if( NULL != position )
	{
		save = position->board;
		position = position->previous;
	}

	return save;
}

// Adds a node
void Undo::addNode( Sudoku board )
{
	if( NULL == first )
	{
		first = new Node;
		first->board = board;
		first->next = first;
		first->previous = first;
		position = first;
		numNodes++;
		return;
	}
	else if( position == first )
	{
		position = NULL;
		position = new Node;
		position->board = board;
		position->next = position;
		position->previous = first;
		first = position;
		first->next = first;
		numNodes++;
		return;
	}
	else	
	{
		deleteNextNodes( position->next );
		first = position;
		position = NULL;
		position = new Node;
		position->board = board;
		position->next = position;
		position->previous = first;
		first = position;
		numNodes++;
		return;
	}
}
		

void Undo::printBoards()
{
	Node * ptr = first;

	do
	{
		ptr->board.print();
		ptr = ptr->previous;
	}while( ptr->previous != ptr );
}
