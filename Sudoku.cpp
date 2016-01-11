// Sudoku class implementation
// Adam Levy
#include "Sudoku.h"

// Default Constructor: Initializes all cells to 0 and all bools to false
Sudoku::Sudoku()
{
	for( int r = 0; r < 9; r++ )
	{
		for( int c = 0; c < 9; c++ )
		{
			cell[r][c] = 0;
			possible[r][c][0] = 0;
		}
	}

	// With no entries all cells will have TRUE == possible[r][c][value] 
	// for all possible values.
	genInitPosVal();
}

// Copy Constructor
Sudoku::Sudoku( const Sudoku & board )
{ 
	for( int r = 0; r < 9; r++ )
	{
		for( int c = 0; c < 9; c++ )
		{
			cell[r][c] = board.cell[r][c];
			for( int i = 0; i < 10; i++ )
			{
				possible[r][c][i] = board.possible[r][c][i];
			}
		}
	}
}

// =operator
Sudoku & Sudoku::operator=( const Sudoku &rhs )
{
	for( int r = 0; r < 9; r++ )
	{
		for( int c = 0; c < 9; c++ )
		{
			cell[r][c] = rhs.cell[r][c];
			for( int i = 0; i < 10; i++ )
			{
				possible[r][c][i] = rhs.possible[r][c][i];
			}
		}
	}

	return *this;
}


// Initialized Constructor: Accepts a string of length 81 of the values of the 
// 				board by row via left-right, top-down. 
Sudoku::Sudoku( string initial )
{
	// Default constructor to initialize everything to zero
	*this = Sudoku();

	// Invalid length => Default Constructor
	if( initial.length() != 81 )
	{
		return;
	}

	// cpp bs for converting strings to ints
	const char *t = initial.c_str();
	char temp;
	for( int r = 0; r < 9; r++ )
	{
		for( int c = 0; c < 9; c++ )
		{
			temp = t[r*9+c];
			cell[r][c] = atoi(&temp);
			
			if( cell[r][c] == 0 )
			{
				possible[r][c][0] = false;
			}
			else
			{
				possible[r][c][0] = true;
			}
		}
	}
 	genInitPosVal();
}

// Initialized Constructor: Accepts a vector of length 81
Sudoku::Sudoku( vector<int> initial )
{
	// Default constructor to initialize everything to zero
	*this = Sudoku();

	// Invalid length => Default Constructor
	if( initial.size() != 81 )
	{
		return;
	}

	for( int r = 0; r < 9; r++ )
	{
		for( int c = 0; c < 9; c++ )
		{
			cell[r][c] = initial[ r*9 + c ];
			if( cell[r][c] == 0 )
			{
				possible[r][c][0] = false;
			}
			else
			{
				possible[r][c][0] = true;
			}
		}
	}
	genInitPosVal();
}

// Cell value mutator, only allows values that are still marked as possible to be set.
bool Sudoku::set( int row, int col, int value )
{
    // check for valid value, row, and column
	if( 1 > value || 9 < value )
		return false;

	if( 0 > row || 8 < row )
		return false;
	
	if( 0 > col || 8 < col )
		return false;

    // if the cell is not empty, you cannot change its value
	if( 0 != cell[row][col] )
		return false;

    // if the value hasn't been eliminated from the possiblities
	if( true == possible[row][col][value] )
	{
		cell[row][col] = value;
	}
	else
	{
		return false;
	}

	// Adjust the possible values
	
	// No other values are possible in [row][col]
	for( int v = 1; v <= 9; v++ )
	{
		if( v != value )
			possible[row][col][v] = false;
	}
	
	// value is not possible anywhere else in the row
	for( int r = 0; r < 9; r++ )
	{
		if( r != row )
			possible[r][col][value] = false;
	}
	
	// value is not possible anywhere else in the col
	for( int c = 0; c < 9; c++ )
	{
		if( c != col )
			possible[row][c][value] = false;
	}

	// value is not possible anywhere else in the subregion
	int subR = row / 3;
	int subC = col / 3;

	for( int r = subR * 3; r < subR * 3 + 3; r++ )
	{
		for( int c = subC * 3; c < subC * 3 + 3; c++ )
		{
			if( r != row && c != col )
				possible[r][c][value] = false;
		}
	}
	
	return true;
}

// Cell value accessor
int Sudoku::get( int row, int col )
{
	if( row < 0 || row > 8 )
		return -1;
	
	if( col < 0 || col > 8 )
		return -1;
	
	return cell[row][col];
}

// This function is used to generate the initial possible values in Sudoku::genInitPosVal()
// Returns TRUE if the value does not have a row, column, or subregion conflict.
// Returns FALSE otherwise.
// NOTE: !!!!!   DIFFERENT THAN Sudoku::isPossible()   !!!!! 
// 	 This is just a basic check to see if the value does not immediately create 
// 	 a conflict. There may be values which have been marked as not possible 
// 	 ( i.e. FALSE == possible[row][col][val] ) by some other algorithm such as 
// 	 Sudoku::lockedCandidates() but still pass Sudoku::isValid(,,) 
bool Sudoku::isValid( int row, int col, int value )
{
    // check that value, row and col, are in range
	if( value < 1 || value > 9 )
		return false;

	if( row < 0 || row > 8 )
		return false;
	
	if( col < 0 || col > 8 )
		return false;

    // if the cell was specified in the initial puzzle
    //      just return whether the value is in the cell
	if( possible[row][col][0] == true )
	{
		if( cell[row][col] == value )
			return true;
		else
			return false;
	}

	// Check for row collisions
	for( int c = 0; c < 9; c++ )
	{	
		if( c != col && cell[row][ c ] == value )
			return false;
	}
	
	// Check for column collisions
	for( int r = 0; r < 9; r++ )
	{
		if( r != row && cell[ r ][col] == value )
			return false;
	}

	// Check for subregion collisions
	int subR = row / 3;
	int subC = col / 3;

	for( int r = subR * 3; r < subR * 3 + 3; r++ )
	{
		for( int c = subC * 3; c < subC * 3 + 3; c++ )
		{
			if( r != row && c != col &&
			    cell[ r ][ c ] == value )
				return false;
		}
	}

	return true;
}

// Accessor for possible[row][col][value]
// This returns false if the value has been set as not a possible value by some algorithm.
// NOTE: !!!!! DIFFERENT THAN Sudoku::isValid(,,) !!!!!
bool Sudoku::isPossible( int row, int col, int value )
{
    // check that value, row and col, are in range
	if( value < 1 || value > 9 )
		return false;

	if( row < 0 || row > 8 )
		return false;
	
	if( col < 0 || col > 8 )
		return false;

	return possible[row][col][value];
}

// This function initializes the possible[][][] bools by calling Sudoku::isValid()
// for each cell and value. This will overwrite the previous possible[][][] and all
// inferences that have been applied to the bools.
void Sudoku::genInitPosVal()
{
	for( int r = 0; r < 9; r++ )
	{
		for( int c = 0; c < 9; c++ )
		{
			for( int value = 1; value <= 9; value++ )
			{
				possible[r][c][value] = isValid( r, c, value );
			}
		}
	}
}

// A simple display function
void Sudoku::print()
{
	for( int r = 0; r < 9; r++ )
	{
		for( int c = 0; c < 9; c++ )
		{
			if( 0 == cell[r][c] )
			{
				cout << "   ";
			}
			else
			{
				cout << " " << cell[r][c] << " ";
			}
			if( c != 8 && c % 3 == 2 )
				cout << "|";
		}
		cout << endl;
		if( r != 8 && r % 3 == 2 )
		{
			cout << " ";
			for( int i = 0; i < 27; i++ )
				cout << "-";
			cout << endl;
		}
	}
	cout << endl;
}

void Sudoku::print(int row, int col )
{
	for( int r = 0; r < 9; r++ )
	{
		for( int c = 0; c < 9; c++ )
		{
			if( r == row && c == col )
			{
				cout << " * ";
			}
			else if( 0 == cell[r][c] )
			{
				cout << "   ";
			}
			else
			{
				cout << " " << cell[r][c] << " ";
			}
			if( c != 8 && c % 3 == 2 )
				cout << "|";
		}
		cout << endl;
		if( r != 8 && r % 3 == 2 )
		{
			cout << " ";
			for( int i = 0; i < 27; i++ )
				cout << "-";
			cout << endl;
		}
	}
	cout << endl;
}

// Accessor for possible[row][col][0]
// Returns TRUE if the cell value was part of the original puzzle ( i.e. non zero entry when Initialized Contructor was called )
// Returns FALSE if the cell value has not been set or was set after initialization.
bool Sudoku::isGiven( int row, int col )
{
	if( row < 0 || row > 8 )
		return false;
	
	if( col < 0 || col > 8 )
		return false;

	return possible[row][col][0];
}

// Returns TRUE if the board is solved, FALSE otherwise.
// Since invalid values can never be set to the board it is sufficient
// to check that there are no empty cells left.
bool Sudoku::isSolved()
{
	for( int r = 0; r < 9; r++ )
	{
		for( int c = 0; c < 9; c++ )
		{
			if( cell[r][c] == 0 )
				return false;
		}
	}

	return true;
}

// Comparison operator
// If a single cell or possible value is different then the boards are different.
bool operator==( const Sudoku & lhs, const Sudoku & rhs )
{
	for( int r = 0; r < 9; r++ )
	{
		for( int c = 0; c < 9; c++ )
		{
			if( lhs.cell[r][c] != rhs.cell[r][c] )
				return false;
			for( int v = 1; v <= 9; v++ )
			{
				if( lhs.possible[r][c][v] != rhs.possible[r][c][v] )
					return false;
			}
		}
	}

	return true;
}

// This algorithm fills any cells with only one remaining possible value. 
void Sudoku::fillSingles()
{

	for( int r = 0; r < 9; r++ )
	{
		for( int c = 0; c < 9; c++ )
		{
			// First check that the cell is empty
			if( cell[r][c] == 0 )
			{
                int count = 0;
                int saved = 0;
				// count the number of possible values
				// save the previous possible value
				for( int value = 1; value <= 9; value++ )
				{
					if( possible[r][c][value] == true )
					{
						count++;
						saved = value;
					}
                    if(count > 1)
                        break;
				}
				// If there is only one possibility, the set the cell
				if( 1 == count )
				{
					set( r, c, saved );
				}
			}
		}
	}
}

// This fills in values where there is only one cell where the value can go
// by row, column, and subregion
void Sudoku::elimination()
{
	int count;
	Square saved;
	for( int value = 1; value <= 9; value++ )
    {
        // Check rows
        count = 0;
        for( int r = 0; r < 9; r++ )
        {
            for( int c = 0; c < 9; c++ )
            {
                if( possible[r][c][value] )
                {
                    saved.row = r;
                    saved.col = c;
                    count++;
                }
                if( count > 1 )
                    break;
            }
            if( 1 == count )
                set( saved.row, saved.col, value );
        }

        // Check columns
        count = 0;
        for( int c = 0; c < 9; c++ )
        {
            for( int r = 0; r < 9; r++ )
            {
                if( possible[r][c][value] )
                {
                    saved.row = r;
                    saved.col = c;
                    count++;
                }
                if( count > 1 )
                    break;
            }
            if( 1 == count )
                set( saved.row, saved.col, value );
        }

        // Check sub regions
        for( int subR = 0; subR < 3; subR++ )
        {
            for( int subC = 0; subC < 3; subC++ )
            {
                count = 0;
                for( int r = subR * 3; r < subR * 3 + 3; r++ )
                {
                    for( int c = subC * 3; c < subC * 3 + 3; c++ )
                    {
                        if( possible[r][c][value] )
                        {
                            saved.row = r;
                            saved.col = c;
                            count++; 
                        }
                        if( count > 1 )
                            break;
                    }
                }
                if( 1 == count )
                    set( saved.row, saved.col, value );
            }
        }
    }
}

// This algorithm makes inferences about the possible values but does NOT
// make any changes to the cells.
//
// A locked candidate is a situation in sudoku when you know that a value must fall
// in a particular row or column within a particular subregion without knowing exactly
// which cell in that row or column within the subregion the value belongs to.
// Since the value MUST be in one of the cells in the subregion and since there is only
// one row or one column that value can be assigned to within that subregion, every cell
// in that row or column that is NOT in the subregion cannot have the value as a possibility.
void Sudoku::lockedCandidates()
{
	bool row[3];
	bool col[3];
	Square saved;
	for( int value = 1; value <= 9; value++ )
	{
		// By subregion
		for( int subR = 0; subR < 3; subR++ )
		{
			for( int subC = 0; subC < 3; subC++ )
			{
				// Reset row and col
				for( int i = 0; i < 3; i++ )
				{
					row[i] = false;
					col[i] = false;
				}

				// Move through the subregion
				for( int r = subR * 3; r < subR * 3 + 3; r++ )
				{
					for( int c = subC * 3; c < subC * 3 + 3; c++ )
					{
						if( possible[r][c][value] )
						{
							row[ r % 3 ] = true;
							col[ c % 3 ] = true;
							saved.row = r;
							saved.col = c;
						}
					}
				}
				
				// Check for locked candidates
				if( row[0] + row[1] + row[2] == 1 )
				{
					for( int cc = 0; cc < 9; cc++ )
					{
						if( cc < subC * 3 || cc >= subC * 3 + 3 )
							possible[saved.row][cc][value] = false;
					}
				}
				else if( col[0] + col[1] + col[2] == 1 )
				{
					for( int rr = 0; rr < 9; rr++ )
					{
						if( rr < subR * 3 || rr >= subR * 3 + 3 )
							possible[rr][saved.col][value] = false;
					}
				}
			}
		}
	}
}

// This displays all the possible values for all cells at once
void Sudoku::printPos()
{
	for( int r = 0; r < 9; r++ )
	{
		for( int sr = 0; sr < 3; sr++ )
		{
			for( int c = 0; c < 9; c++ )
			{
				for( int sc = 0; sc < 3; sc++ )
				{
					cout << possible[r][c][3 * sr + sc + 1] << " ";
				}
				if( c % 3 == 2 && c != 8 )
					cout << " |  ";
				else
					cout << "    ";
			}
			cout << endl;
		}
		if( r % 3 == 2 && r != 8 )
		{
			for( int i = 0; i < 85; i++ )
				cout << "-";
		}
		cout << endl;
	}
}

// This prints the sudoku board but places an '*' in place of values that
// the board was initialized with. This makes it easy to see the changes that
// have been made to the board to verify that the algorithms are filling in
// correct values.
void Sudoku::printFill()
{
	for( int r = 0; r < 9; r++ )
	{
		for( int c = 0; c < 9; c++ )
		{
			if( possible[r][c][0] == 1 )
				cout << " * ";
			else
				cout << " " << cell[r][c] << " ";
			if( c != 8 && c % 3 == 2 )
				cout << "|";
		}
		cout << endl;
		if( r != 8 && r % 3 == 2 )
		{
			cout << " ";
			for( int i = 0; i < 27; i++ )
				cout << "-";
			cout << endl;
		}
	}
	cout << endl;
}

// Overloaded print function. This allows you to look at the possible locations of a 
// particular value. When you supply print with an integer (1-9) this prints the sudoku 
// board with 0's in places where that value is not possible and prints the value where 
// it is possible. In other words this allows you to look at the possible locations of 
// a particular value.
void Sudoku::print( int value )
{
	for( int r = 0; r < 9; r++ )
	{
		for( int c = 0; c < 9; c++ )
		{
			if( possible[r][c][value] )
				cout << " " << value << " ";
			else
				cout << " " << 0 << " ";
			if( c != 8 && c % 3 == 2 )
				cout << "|";
		}
		cout << endl;
		if( r != 8 && r % 3 == 2 )
		{
			cout << " ";
			for( int i = 0; i < 27; i++ )
				cout << "-";
			cout << endl;
		}
	}
	cout << endl;
}

// Deterministic solves the board as much as possible using the "deterministic" algorithms
// The "deterministic" algorithms use a set of logical deductions on the possible values.
// This will completely solve boards which require no "guessing" at values to solve.
void Sudoku::deterministic()
{
	Sudoku oldBoard;
	while( oldBoard != *this )
	{
		oldBoard = *this;
		fillSingles();
		elimination();
		lockedCandidates();
	}
}

// Overloaded Sudoku::isValid() This refers to the state of the board. If there is a cell with no possible
// the board is not valid.
// Returns TRUE if all cells have at least one possible value left.
// Returns FALSE if there exists a cell with no possible values left ( i.e. at least one cell is wrong )
bool Sudoku::isSolvable()
{
	int sum = 0;
	for( int r = 0; r < 9; r++ )
	{
		for( int c = 0; c < 9; c++ )
		{
			for( int v = 1; v <= 9; v++ )
			{
				sum += possible[r][c][v];
			}
			if( sum == 0 )
			{
				return false;
			}
			sum = 0;
		}
	}

	return true;
}

// The recursive solver will solve any valid board. 
// Returns TRUE If the board is solved.
// Returns FALSE if the board is not solved and all appropriate guesses have been exhausted.
bool Sudoku::recursiveSolver( /*int depth*/ )
{
	Sudoku original = *this;
	Sudoku child;
	Square saved;
	int guess = 0;
	bool validGuess;

	// Solve the board as much as possible deterministically.
	deterministic();

	// Save a copy of the board so that changes made from invalid
	// guesses can be reverted to the original parent.
	Sudoku parent = *this;

//	// Display the depth and current board state.
//	cout << "depth: " << depth << endl;
//	print();

	// If the board is invalid return false.
	if( !isSolvable() )
	{
		return false;
	}

	// If the board is solved return true.
	if( isSolved() )
	{
		return true;
	}

	// If the board is still valid but not solved we need to make guesses.
	else
	{
		int iter = 0;
		do
		{
			// Make a guess of the lowest possible value in the first empty cell
			// Save the cell where the guess is.
			saved = makeGuess();

			// If we have exhausted the guesses in the first empty cell then
			// exit since this is no longer a valid board. 
			if( saved.row == -1 )
			{
				break;
			}

			// Save the value of the guess made in the "Square saved" cell.
			guess = cell[saved.row][saved.col];

			// The recursiveSolver() will return whether the board is solved after making that guess.
			validGuess = recursiveSolver( /*depth + 1*/ );

			// If it solved it return true.
			if( validGuess )
			{
				return true;
			}

			// If the board was not solved then the guess was not valid.
			// Revert the board to the parent.
			// Remove guess from the possible values in the saved cell.
			// Update parent so it remembers the guesses we have already tried.
			else
			{
				*this = parent;
				possible[saved.row][saved.col][guess] = false;
				parent = *this;
			}
		}while( !validGuess && saved.row != -1 && iter++ < 9 );
		// Some redundancies in the while loop exit criteria
		// Do this while the last guess was bad and there are still
		// more guesses to make.
		// Also never loop more than 9 guesses. ( redundant, should never catch )
	}

	// If we pass all of that then the board had all the guesses exhausted in a particular cell.
	// This means we have a cell with no possible values. 
	// Revert to the parent board.
	// Exit and pass back up to the calling function.
	// If this was called recursively then the next level down will make the next appropriate guess in 
	// the last cell where a guess was made.
	*this = original;
	return false;
}

// This sets the first empty cell to the least possible value and returns a Square with row 
// and col of  cell it edits. This function is used inside the recursiveSolver() function to 
// make guesses systematically.
// If the first empty cell has no possible values the Square that is returned will have -1 for
// row and col.
Square Sudoku::makeGuess()
{
	Square saved;
	saved.row = -1;
	saved.col = -1;

	// Only make a guess on valid unsolved boards.
	if( isSolvable() && !isSolved() )
	{
		// Find the first empty cell and save it.
		for( int r = 0; r < 9; r++ )
		{
			for( int c = 0; c < 9; c++ )
			{
				if( 0 == cell[r][c] )
				{
					saved.row = r;
					saved.col = c;
					break;
				}
			}
			if( -1 != saved.row )
				break;
		}

		// Loop through values 1-9
		for( int v = 1; v <= 9; v++ )
		{
			// Try to assign the value to the saved empty cell
			// Exit once it's set.
			// This assigns the lowest possible value.
			if( set( saved.row, saved.col, v ) )
			{
				break;
			}
		}
	}

	return saved;
}

