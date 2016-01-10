// Undo class definition
// This class fascilitates an undo function in the program
#ifndef UNDO_H
#define UNDO_H

#include "Sudoku.h"

struct Node
{
	Sudoku board;
	Node * previous;
	Node * next;
};

class Undo
{
	private:
		Node * first;
		Node * position;
		int numNodes;
		void deleteNextNodes( Node * );
		void deletePreviousNodes( Node * );

	public:
		Undo();
		Undo( Sudoku );	
		~Undo();

		int getNumNodes(){ return numNodes; }
		void addNode( Sudoku board );
		Sudoku undo();
		void printBoards();
};
	

#endif
