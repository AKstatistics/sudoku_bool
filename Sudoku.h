// Sudoku class definition
// Adam Levy
//
// The Sudoku class consists of a 9x9 2d array of ints denoting the values in the cells
// and a 9x9x10 3d array of bools which represent which values are possible in each cell.
// For each cell, there are 10 bool values. possible[r][c][0] == true denotes that the 
// cell was initialized as part of the puzzle (as opposed to being set as part of the
// solution). Each subsequent value in the possible array corresponds to whether that
// value is still a remaining possible input to that cell. For example, 
// possible[0][1][5] == true denotes that 5 is a possible value in the cell in row 0 column 1.
//
// An instance of the Sudoku class can be initialized with a vector of ints of length 81 or a string of
// digits 0-9 of length 81. Zeroes denote an empty cell. The cells are read in left to right, top to bottom.
//
// The Sudoku class can solve itself when it has been initialized to a valid solvable board.
// The Sudoku class solves itself using a handful of algorithms that deduce cell values from the
// possible inputs. These are called the deterministic functions. They are:
//
// fillSingles - fills in cells with only one possible value
// elimination - fills in cells where there is only one possible position for a value
// 		 within a row, column, or 3x3 subregion
// lockedCandidate - alters the possible values, does not change cells. If a value can only go in a particular
// 			row or column within a particular subregion then it is a locked candidate and can be
// 			eliminated as a possibility in the cells in that row or column not in the subregion.
//
// deterministic - This groups all the above functions together and runs them until no more changes can be made
// 			to the board using only those functions.
// recursiveSolver - This recursively runs the deterministic function and then makes a guess in the first empty cell. 
// 			This will solve all valid boards completely.
#ifndef SUDOKU_H
#define SUDOKU_H

#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
using std::vector;
using std::string;
using std::cout;
using std::endl;

struct Square
{
	int row;
	int col;
};

class Sudoku
{
	private:
		int cell[9][9];
		bool possible[9][9][10];
	public:
		Sudoku();
		Sudoku( const Sudoku & );
		Sudoku( string );
		Sudoku( vector<int> initial );
		
		void genInitPosVal();

		// cell mutator and accessor
		bool set( int row, int col, int value );
		int get( int row, int col );

		bool isValid( int row, int col, int value );
		bool isValid();
		bool isPossible( int row, int col, int value );
		bool isFixed( int row, int col );
		bool isSolved();
		
		friend bool operator==( const Sudoku &lhs, const Sudoku &rhs );
		friend bool operator!=( const Sudoku &lhs, const Sudoku &rhs ) { return !( lhs == rhs ); }
		Sudoku & operator=( const Sudoku &rhs );
		
		void fillSingles();
		void elimination();
		void lockedCandidates();
		void deterministic();
		bool recursiveSolver(int );
		Square makeGuess();

		// print functions
		void print();
		void print( int row, int col );
		void print( int );
		void printPos();
		void printFill();
};

#endif
