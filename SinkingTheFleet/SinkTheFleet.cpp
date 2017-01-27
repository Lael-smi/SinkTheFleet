//----------------------------------------------------------------------------
// File:	SinkTheFleetleet.cpp
// 
// Function:
//      main()
//----------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include "fleet.h"


using namespace std;
extern const char* shipNames[7];
//---------------------------------------------------------------------------------
// Function:	main()
// Title:		Set ShipInfo
// Description:
//				Runs the Sink the Fleet Game
// Programmer:	Paul Bladek
// modified by:
// 
// Date:		12/9/2010
//
// Version:		0.5
// 
// Environment: Hardware: i3 
//              Software: OS: Windows 7; 
//              Compiles under Microsoft Visual C++ 2013
//
// Input:		
//
// Output:		
//
// Calls:		initializePlayer()
//				allocMem()
//				safeChoice()
//				getGrid()
//				printGrid()
//				resetGrid()
//				setships()
//				header()
//				getCoord()
//				deleteMem()
//				endBox()
//
// Called By:	n/a
//
// Parameters:	void
// 
// Returns:		int -- EXIT_SUCCESS upon successful completion
//
// History Log: 
//				12/9/2010 PB completed v 0.5
//   
//---------------------------------------------------------------------------------
int main(void)
{
	short numRows = SMALLROWS;			// total number of rows in the array
	short numCols = SMALLCOLS;			// total number of columns in the array
	char again = 'N';
	char gridSize = 'S';
	char readFromFile = 'N';
	short whichPlayer = 0;
	bool gameOver = false;
	bool reshot = false;


	Cell coord = { 0, 0 };
	string message;
	string filename;
	Ship shipHit = NOSHIP;
	Player game[NUMPLAYERS];	// the two players in an array
	// other stuff ...

	do
	{
		system("cls");
		cout << endl;
		header(cout);
		gridSize = safeChoice("Which size grid to you want to use", 'S', 'L');

		if (gridSize == 'L')
		{
			numRows = LARGEROWS;
			numCols = LARGECOLS;
		}
		else
		{
			numRows = SMALLROWS;
			numCols = SMALLCOLS;
		}
		initializePlayer(game);
		initializePlayer(game + 1);
		// dynamically create the rows of the array
		allocMem(game, gridSize);
		for (whichPlayer = 0; whichPlayer < NUMPLAYERS; whichPlayer++)
		{
			// enter grid files or let users enter ships
			cout << "Player " << (whichPlayer + 1) << ", ";
			readFromFile = safeChoice("Would you like to read starting grid from a file?", 'Y', 'N');
			if (readFromFile == 'Y')
			{
				cout << "Please enter the name of the file you would like to load: ";
				cin >> filename;
				getGrid(game, whichPlayer, gridSize, filename);
				cin.ignore(BUFFER_SIZE, '\n');
			}
		
			else {
				setships(game, gridSize, whichPlayer);
			}

	

		}
		system("cls");
		header(cout);
		cout << "Press <ENTER> to start the battle . . .";
		cin.get();

		whichPlayer = 0;
		gameOver = false;
		while (!gameOver)
		{
			printGrid(cout, game[whichPlayer].m_gameGrid[1], gridSize);
			cout << "Player " << whichPlayer + 1 << ", Enter coordinates for firing:" << endl;
			coord = getCoord(cin, gridSize);
			shipHit = game[!whichPlayer].m_gameGrid[0][coord.m_row][coord.m_col];
			if (game[whichPlayer].m_gameGrid[1][coord.m_row][coord.m_col] == MISSED ||
				game[whichPlayer].m_gameGrid[1][coord.m_row][coord.m_col] == HIT)
			{
				cout << "You have already shot at that space, please shoot again";
				continue;
			}

			if (shipHit == NOSHIP)
			{
				game[whichPlayer].m_gameGrid[1][coord.m_row][coord.m_col] = MISSED;
				cout << "MISSED!" << endl;;
				cin.get();
			}
			else
			{
				game[whichPlayer].m_gameGrid[1][coord.m_row][coord.m_col] = HIT;
				cout << "HIT!" << endl;;
				
				game[!whichPlayer].m_ships[shipHit].m_piecesLeft -= 1;
				if (game[!whichPlayer].m_ships[shipHit].m_piecesLeft == 0)
				{
					cout << shipNames[shipHit] << " Has been Sunk!" << endl;;

				}
				game[!whichPlayer].m_piecesLeft -= 1;
				if (game[!whichPlayer].m_piecesLeft == 0)
				{
					gameOver = true;
					/*cout << "Player " << whichPlayer + 1 << " has sunken Player " << !whichPlayer + 1 << "'s fleet!" << endl;*/
					endBox(whichPlayer);
				}
				cout << "Press <ENTER>";
				cin.get();
				continue;
			}



			whichPlayer = !whichPlayer;  // switch players
		}

		again = safeChoice("Would you like to play again?", 'Y', 'N');
	} while (again == 'Y');

	return EXIT_SUCCESS;
}
