// README COMPILE INSTRUCTIONS
//
// Compile main.cpp Sudoku.cpp Undo.cpp at the same time with Sudoku.h and Undo.h in the same folder.

// Main program for Sudoku Game
// Adam Levy
//
// The main has several menus which allow the user to input a board, load an existing board, and then have the computer solve it or play the board in the game.
// The game play features an undo function which is implemented via a linked list.
#include "Sudoku.h"
#include "Undo.h"

using std::cin;

void clear();	// This clears the console so the app is contiguous for the viewer
void clearIN();	// clears the input buffer
void addBoard( Sudoku ** );	// launches the add board menu
void selectBoard( Sudoku ** );	// lanches the select board menu
bool digitsOnly( const string & input ); // used to validate user defined boards
void solve( Sudoku * );	// Solves the board and outputs results.
void play( Sudoku * );	// The game play function

int main()
{
	clear();
	Sudoku * board = NULL;
	cout << "Welcome to Sudoku! \n By Adam Levy " << endl;

	// Where do we want to get a board from?
	// user defined or preloaded
	int menuOption;
	for( ;; )
	{
		cout << "Select a menu option: " << endl;
		cout << " 1. Input a Sudoku board. " << endl;
		cout << " 2. Select an existing board. " << endl;

		if( cin >> menuOption && 0 < menuOption && menuOption < 3 )
		{
			break;
		}
		else
		{
			clear();
			clearIN();

			cout << "Invalid input. \n\n";
		}
	}

	clear();

	// call the menu function corresponding to the users choice

	switch( menuOption )
	{
		case 1:
			addBoard( &board );
			break;
		case 2:
			selectBoard( &board );
			break;
	}

	clear();
	
	// Once a board has been selected the user must decide
	// to play the board
	// or to have the computer solve it and print the solution
	for( ;; )
	{
		cout << "Your selected board: " << endl;
		board->print();
		cout << "\nSelect an option. " << endl;
		cout << " 1. Play the board. " << endl;
		cout << " 2. Auto-Solve. " << endl;

		if( cin >> menuOption && 0 < menuOption && menuOption < 3 )
		{
			break;
		}
		else
		{
			clear();
			clearIN();

			cout << "Invalid input. \n\n";
		}
	}

	clear();

	// Call the appropriate menu function
	switch( menuOption )
	{
		case 1:
			play( board );
			break;
		case 2:
			solve( board );
			break;
	}

	return 0;
}

void clear()
{
	cout << string( 100, '\n' );
	cout << "********** SUDOKU **********  \n\n";
}

void clearIN()
{
	cin.clear();
	cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
}

void addBoard( Sudoku ** board )
{
	string input;
	
	Sudoku exampleBoard( "900034050604700000000000900500080704000000000208010006003000000000002408010850009" );

	for( ;; )
	{
		cout << "EXAMPLE: " << endl;
		cout << "900034050604700000000000900500080704000000000208010006003000000000002408010850009\n" << endl;
		cout << "The above represents the following board. It is read in left to right, top to bottom. \nZeros represent empty cells. " << endl;
		exampleBoard.print();
		cout << "Please specify a board by pasting in a string in the above format: \n" << endl;

		// Boards must be of length 81 and digits only
		if( cin >> input && 81 == input.length() && digitsOnly( input ) )
		{
			*board = new Sudoku( input );
			break;
		}
		else
		{
			clear();
			clearIN();
			cout << "Invalid input. \n\n";
		}
	}
}

bool digitsOnly( const string & input )
{
	bool out = ( input.find_first_not_of( "0123456789" ) == std::string::npos );
	return out;
}

// There are 5 boards to choose from of varying difficulty
void selectBoard( Sudoku ** board )
{
	int menu;
	for( ;; )
	{
		cout << "Select a difficulty. " << endl;
		cout << " 1. * " << endl;
		cout << " 2. ** " << endl;
		cout << " 3. *** " << endl;
		cout << " 4. **** " << endl;
		cout << " 5. ***** " << endl;

		if( cin >> menu && 1 <= menu && menu <= 5 )
		{
			break;
		}
		else
		{
			clear();
			clearIN();
			cout << "Invalid input. \n\n";
		}
	}

	string strBoard;
	
	switch( menu )
	{
		case 1:
			strBoard = "302000800860000000000052010093240000080060020000019460040730000000000037007000201";
			break;
		case 2:
			strBoard = "000801003000020401004000070010200040000030000050008060020000500407060000800903000";
			break;
		case 3:
			strBoard = "300020700100604008900100400250000000000040000000000021003008007400309005001060004";
			break;
		case 4:
			strBoard = "150000000002000000004360007006024050000050000030190800700016400000000300000000018";
			break;
		case 5:
			strBoard = "800000000003600000070090200050007000000045700000100030001000068008500010090000400";
			break;
	}

	*board = new Sudoku( strBoard );
}

void solve( Sudoku * board )
{
	Sudoku save = *board;

	if( board->recursiveSolver( 0 ) )
	{
		cout << "Solved!" << endl;

		save.print();
		board->print();
	}

	else
	{
		cout << "Board not solvable. " << endl;
		*board = save;
		board->print();
	}
}

void play ( Sudoku * board )
{
	// Solve the board and have the original and solution on hand
	Sudoku original = *board;
	Sudoku solution = *board;
	solution.recursiveSolver(0);
	*board = original;
	
	Sudoku tempBoard;

	Undo undo;

	const char *t;

	int curRow;
	int curCol;

	bool validSet = 1;
	bool broke = 0;

	// Find the first uninitialized cell to place the cursor
	for( int r = 0; r < 9; r++ )
	{
		for( int c = 0; c < 9; c++ )
		{
			if( !board->isFixed(r, c) )
			{
				curRow = r;
				curCol = c;
				broke = 1;
				break;
			}
		}
		if( broke )
			break;

	}

	char input = ' ';
	int val;

	// main loop of the play function continues to pull 1 char at a time from cin.
	for( ;; )
	{
		clear();
		if( 'P' == input )
		{
			cout << "Solution: " << endl;
			solution.print();
			cout << endl;
		}
		if( !validSet )
		{
			cout << "Cannot assign value to cell. " << endl;
			cout << endl;
		}
		if( board->isSolved() )
		{
			cout << "Solved!" << endl;
			cout << endl;
		}

		validSet = 1;

		input = ' ';
		val = -1;

		cout << "Use W, A, S, D to move up, left, down, or right in the board. The current cell is denoted by *." << endl;
		cout << "Use 1-9 to set a value in a cell. Use U to undo. Use P to view the solution." << endl;
		cout << "Use Q to quit. Enter only 1 command at a time. Press [return] after each command to execute it.\n\n";

		board->print( curRow, curCol );

		cout << endl;
		
		if( !( cin >> input ) )
		{
			clear();
			clearIN();
		}
		
		if( isdigit(input) )
		{
			t = &input; 
		        val = atoi( t );
			if( 1 <= val && val <= 9 )
			{
				tempBoard = *board;
				validSet = board->set( curRow, curCol, val );
				// if we make a valid move add the previous board to the undo list
				if( validSet )
				{
					undo.addNode( tempBoard );
				}
				undo.printBoards();
			}
		}
		else
		{
			input = std::toupper( input );
		}

		// all of the cursor if statements continue to push the cursor in the 
		// specified direction until it reaches an uninitialized cell
		if( 'W' == input )
		{
			for( ; curRow > 0; )
			{
				curRow--;
				if( !board->isFixed( curRow, curCol ) )
				{
					break;
				}
			}
			if( 0 == curRow && board->isFixed( curRow, curCol ) )
			{
				curRow++;
			}
		}
		else if( 'A' == input )
		{
			for( ; curCol > 0;  )
			{
				curCol--;
				if( !board->isFixed( curRow, curCol ) )
				{
					break;
				}
			}
			if( 0 == curCol && board->isFixed( curRow, curCol ) )
			{
				curCol++;
			}
		}
		else if( 'S' == input )
		{
			for( ; curRow < 8;  )
			{
				curRow++;
				if( !board->isFixed( curRow, curCol ) )
				{
					break;
				}
			}
			if( 8 == curRow && board->isFixed( curRow, curCol ) )
			{
				curRow--;
			}
		}
		else if( 'D' == input )
		{
			for( ; curCol < 8;  )
			{
				curCol++;
				if( !board->isFixed( curRow, curCol ) )
				{
					break;
				}
			}
			if( 8 == curCol && board->isFixed( curRow, curCol ) )
			{
				curCol--;
			}
		}
		else if( 'U' == input )
		{
			*board = undo.undo();
		}
		else if( 'Q' == input )
		{
			break;
		}
	}
}
