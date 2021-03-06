//----------------------------------------------------------------------------
// File:	fleet.cpp
// 
// Functions: 
//	  setShipInfo()	
//	  allocMem() 
//	  deleteMem()
//	  printShip() 
//	  printGrid() 
//	  initializePlayer() 
//	  setships()	      
//	  saveGrid()
//	  getGrid()
//	  getCoord()
//	  validLocation() 
//	  header() 
//	  endBox() 
//----------------------------------------------------------------------------
#include "fleet.h"
//----------------------------------------------------------------------------
// YOU ARE RESPONSIBLE FOR CORRECT HEADERS -- one for each function
// include the definitions for each of the non-template functions
//    declared in fleet.h 
//----------------------------------------------------------------------------

const char* shipNames[SHIP_SIZE_ARRAYSIZE] =
{ "No Ship", "Mine Sweeper", "Submarine", "Frigate",
	"Battleship", "Aircraft Carrier" };
const int TOTALPIECES = 17; // total pieces in all ships

//----------------------------------------------------------------------------
// Function:	setShipInfo()
// Title:		Set ShipInfo
// Description:
//				Sets struct ShipInfo fields
// Programmer:	Lael Smith
//			    Anthony Waddell
// 
// Date:		01/31/17
//
// Version:		1.0
// 
// Environment:	Hardware: Intel(R) Core(TM) i5-6500 CPU @ 3.20GHz (4 CPUs), 
//				~3.2GHz
//				Software:   MS Windows 10 
//				Compiles under Microsoft Visual C++ 2015
//
// Called By:
//
// Parameters:	shipInfoPtr: ShipInfo *; pointer to the ShipInfo to be set
//				name: Ship;	enumerated name of type of ship; default: NOSHIP
//				orientation: Direction;	enumerated direction; 
//				default: HORIZONTAL
//				row: unsigned short; row-coordinate in grid; default: 0
//				col: unsigned short; column-coordinate in grid; default: 0
// 
// Returns:		void
//
// History Log:
//				12/20/05	PB completed v 0.1
//				01/27/17	LS	completed v0.2
//   
//----------------------------------------------------------------------------
void setShipInfo(ShipInfo * shipInfoPtr, Ship name, Direction orientation,
	unsigned short row, unsigned short col)
{
	shipInfoPtr->m_orientation = orientation;
	shipInfoPtr->m_name = name;
	shipInfoPtr->m_bowLocation.m_row = row;
	shipInfoPtr->m_bowLocation.m_col = col;
	shipInfoPtr->m_piecesLeft = shipSize[name];
}

//----------------------------------------------------------------------------
// Function:	allocMem()
// Title:		Allocate Memory
// Description:
//				allocates memory for current grids
// Programmer:	Paul Bladek
//			    Anthony Waddell
//				Lael Smith
//				
// modified by:	
// 
// Date:		01/31/17
//
// Version:	1.2
// 
// Environment: Hardware: Intel(R) Core(TM) i5-6500 CPU @ 3.20GHz (4 CPUs), 
//				~3.2GHz
//              Software: OS: Windows 10 
//              Compiles under Microsoft Visual C++ 2015
//
// Calls:		deleteMem()
//
// Called By:	main()
//
// Parameters:	players: Player[]; 	the array of 2 players in the game
//				size: char;	'	S' or 'L' (small or large)
// 
// Returns:	void
//
// History Log:
//		12/20/05	PB completed v 1.0 
//		09/13/06	PB completed v 1.01
//		01/27/17	LS	completed v1.2
//
//----------------------------------------------------------------------------
void allocMem(Player players[], char size)
{
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;

	try
	{
		for (short i = 0; i < NUMPLAYERS; ++i)
		{
			//-------------------------------------------------
			// set the pointers to NULL, then allocate the
			// memory for each row in each grid
			//--------------------------------------------------
			players[i].m_gameGrid[0] = nullptr;
			players[i].m_gameGrid[0] = new Ship*[numberOfRows];
			players[i].m_gameGrid[1] = nullptr;
			players[i].m_gameGrid[1] = new Ship*[numberOfRows];
			for (short j = 0; j < numberOfRows; ++j)
			{
				players[i].m_gameGrid[0][j] = nullptr;
				players[i].m_gameGrid[0][j] = new Ship[numberOfCols];
				players[i].m_gameGrid[1][j] = nullptr;
				players[i].m_gameGrid[1][j] = new Ship[numberOfCols];


				for (short k = 0; k < numberOfCols; ++k)
				{
					players[i].m_gameGrid[0][j][k] = NOSHIP;
					players[i].m_gameGrid[1][j][k] = NOSHIP;
				} // end for k
			} // end for j
		} // end for i
	}
	catch (exception e)
	{
		deleteMem(players, size);
		cerr << "exception: " << e.what() << endl;
		cout << "shutting down" << endl;
		cin.ignore(BUFFER_SIZE, '\n');
		exit(EXIT_FAILURE);
	}
}

//----------------------------------------------------------------------------
// Function:	deleteMem()
// Title:		Delete Memory
// Description:
//				Safely deletes memory for grids
// Programmer:  Paul Bladek
//			    Anthony Waddell
//				Lael Smith
// 
// Date:		01/31/17
//
// Version:		0.3
//				1.0
// 
// Environment: Hardware: Intel(R) Core(TM) i5-6500 CPU @ 3.20GHz (4 CPUs),
//				~3.2GHz
//              Software: OS: Windows 10 
//              Compiles under Microsoft Visual C++ 2015
//
// Calls:
//
// Called By:	main()
//				allocMem()
//
// Parameters:	players: Player[]; 	the array of the 2 Players
//				size: char;	'	S' or 'L'
// 
// Returns:	void
//
// History Log:
//				12/20/05	PB completed v 0.1
//				01/12/17	AW added delete code block
//				01/27/17	LS	completed v0.3
//   
//----------------------------------------------------------------------------
void deleteMem(Player players[], char size)
{
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	// delete[] in reverse order of allocMem()
	// be sure to check if the memory was allocated (!nullptr) BEFORE deleting
	for (int i = 0; i < NUMPLAYERS; i++)
	{
		for (int j = 0; j < numberOfRows; j++)
		{
			if (players[i].m_gameGrid[i][j] != nullptr)
			{
				delete[] players[i].m_gameGrid[0][j];
				delete[] players[i].m_gameGrid[1][j];
			}
		}
		delete[] players[i].m_gameGrid[0];
		delete[] players[i].m_gameGrid[1];
	}
}

//----------------------------------------------------------------------------
// Function:	printShip()
// Title:		Print Ship 
// Description:
//				Print grid element for the proper ship
// Programmer:	Paul Bladek
// 
// Date:		9/12/06
//
// Version:		1.1
// 
// Environment: Hardware: Intel(R) Core(TM) i5-6500 CPU @ 3.20GHz (4 CPUs),
//				~3.2GHz 
//              Software: OS: Windows 10 
//              Compiles under Microsoft Visual C++ 2015
//
// Output:		three characters representing one ship to sout
//
// Called By:	printGrid()
//
// Parameters:	sout: ostream&;	the stream to print to
//				thisShip: Ship;	the Ship to be printed in the grid format
// 
// Returns:	void
//
// History Log: 
//		09/12/06	PB comleted v 1.0
//		01/27/17	LS	completed v1.1
//     
//----------------------------------------------------------------------------
void printShip(ostream & sout, Ship thisShip)
{
	sout << ' ';
	switch (thisShip)
	{
	case NOSHIP: sout << ' ';
		break;
	case MINESWEEPER: sout << 'M';
		break;
	case SUB: sout << 'S';
		break;
	case FRIGATE: sout << 'F';
		break;
	case BATTLESHIP: sout << 'B';
		break;
	case CARRIER: sout << 'C';
		break;
	case HIT: sout << 'H';
		break;
	case MISSED: sout << MISS;
		break;
	default: sout << 'X';
	}
	sout << VERT;
}

//----------------------------------------------------------------------------
// Function:	printGrid()
// Title:		Print Ship 
// Description:
//				Print grid element for the proper ship
// Programmer:
// 
// Date:		1/31/17
//
// Version:		1.0
// 
// Environment: Hardware: Intel(R) Core(TM) i5-6500 CPU @ 3.20GHz (4 CPUs),
//				~3.2GHz
//              Software: OS: Windows 10 
//              Compiles under Microsoft Visual C++ 2015
//
// Output:		a single grid to sout
//
// Calls:		printShip()
//
// Called By:	main()
//				setships()
//				saveGrid()
//
// Parameters:	sout: ostream&;	the stream to print to
//				grid: Ship**;	the 2-D array of Ships 
//				size: char;	'S' or 'L'
// 
// Returns:		void
//
// History Log:
//				12/20/05 PB completed v 0.1
//				1/31/17 AW LS completed v 1.0
//     
//----------------------------------------------------------------------------
void printGrid(ostream& sout, Ship** grid, char size)
{
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;
	char c = 'A';
	long originalformat = sout.flags();
	system("cls");
	for (short j = 1; j <= numberOfCols; ++j)
		sout << setw(3) << j;
	sout << endl;

	for (int i = 0; i < numberOfRows; i++)
	{
		sout << left << c;
		for (int j = 0; j < numberOfCols; j++)
		{
			printShip(sout, grid[i][j]);
		}
		c += 1;
		sout << endl << HORIZ;
		for (int k = 0; k < numberOfCols; k++)
		{
			sout << HORIZ << HORIZ << CR;
		}
		sout << endl;

	}
	sout.flags(originalformat);
}

//----------------------------------------------------------------------------
// Function:	initializePlayer()
// Title:		Initialize Player 
// Description:
//				sets initial values for m_ships and m_piecesLeft
// Programmer:	Paul Bladek
// 
// Date:		01/31/17
//
// Version:		1.0
// 
// Environment: Hardware: Intel(R) Core(TM) i5-6500 CPU @ 3.20GHz (4 CPUs),
//				~3.2GHz 
//              Software: OS: Windows 10 
//              Compiles under Microsoft Visual C++ 2015
//
// Calls:
//
// Called By:	main()
//
// Parameters:	playerPtr: Player*;	a pointer to the current Player
// 
// Returns:		void
//
// History Log: 
//				9/12/06 PB comleted v 1.0
//     
//----------------------------------------------------------------------------
void initializePlayer(Player* playerPtr)
{
	for (short i = 0; i < SHIP_SIZE_ARRAYSIZE; i++)
		setShipInfo(playerPtr->m_ships + i, static_cast<Ship>(i));

	playerPtr->m_piecesLeft = TOTALPIECES;
}

//----------------------------------------------------------------------------
// Function:	setships()
// Title:		Set Ships 
// Description:
//				Allows user to put ships in grid
// Programmer:	Paul Bladek
// modified by:	Lael Smith 
//				Anthony Waddell
// 
// Date:		01/31/17
//
// Version:		0.6
// 
// Environment: Hardware: Intel(R) Core(TM) i5-6500 CPU @ 3.20GHz (4 CPUs),
//				~3.2GHz
//              Software: OS: Windows 10 
//              Compiles under Microsoft Visual C++ 2015
//
// Input:		location and orientation using getCoord from cin
//
// Output:		prompts to cout
//
// Calls:		printGrid()
//				safeChoice()
//				getCoord()
//				saveGrid()
//
// Called By:	main()
//
// Parameters:	players: Player[];	the array of 2 players 
//				size: char;		'S' or 'L'
//				whichPlayer: short;	the player number (0 or 1)
// 
// Returns:		void
//
// History Log: 
//				9/12/06		PB		comleted v0.5
//				01/27/17	AW LS	completed v0.6
//     
//----------------------------------------------------------------------------
void setships(Player players[], char size, short whichPlayer)
{
	char input = 'V';
	char ok = 'Y';
	char save = 'N';

	ostringstream outSStream;
	Cell location = { 0, 0 };
	for (short j = 1; j < SHIP_SIZE_ARRAYSIZE; j++)
	{
		printGrid(cout, players[whichPlayer].m_gameGrid[0], size);
		outSStream.str("");
		outSStream << "Player " << whichPlayer + 1 << " Enter "
			<< shipNames[j] << " orientation";
		input = safeChoice(outSStream.str(), 'V', 'H');
		players[whichPlayer].m_ships[j].m_orientation
			= (input == 'V') ? VERTICAL : HORIZONTAL;
		cout << "Player " << whichPlayer + 1 << " Enter " << shipNames[j] <<
			" bow coordinates <row letter><col #>: ";
		players[whichPlayer].m_ships[j].m_bowLocation = getCoord(cin, size);
		location = players[whichPlayer].m_ships[j].m_bowLocation;
		// if ok
		if (!validLocation(players[whichPlayer], j, size))
		{
			cout << "invalid location. Press <enter>";
			cin.get();
			j--; // redo
			continue;
		}

		players[whichPlayer].m_ships[j].m_name = (Ship)j; // need this ship

		if (input == 'V')
		{
			for (int i = 0; i < (Ship)j[shipSize]; i++)
			{
				players[whichPlayer].
					m_gameGrid[0][location.m_row + i][location.m_col] 
					= (Ship)j;
			}
			printGrid(cout, players[whichPlayer].m_gameGrid[0], size);
			outSStream.str("");
			outSStream << "Player " << whichPlayer + 1 << " " << shipNames[j]
				<< " OK?";
			ok = safeChoice(outSStream.str(), 'Y', 'N');
		}
		else if (input = 'H')
		{
			for (int i = 0; i < (Ship)j[shipSize]; i++)
			{
				players[whichPlayer].
					m_gameGrid[0][location.m_row][location.m_col + i]
					= (Ship)j;
			}
			printGrid(cout, players[whichPlayer].m_gameGrid[0], size);
			outSStream.str("");
			outSStream << "Player " << whichPlayer + 1 << " " << shipNames[j]
				<< " OK?";
			ok = safeChoice(outSStream.str(), 'Y', 'N');
		}
	}// end for j
	save = safeChoice("\nSave starting grid?", 'Y', 'N');
	if (save == 'Y')
	{
		system("cls");
		saveGrid(players, whichPlayer, size);
	}
}

//----------------------------------------------------------------------------
// Function:	saveGrid()
// Title:		Save Grid 
// Description:
//				Saves the ship grid to a file
// Programmer:
// 
// Date:		01/31/17
//
// Version:		0.2
// 
// Environment: Hardware: Intel(R) Core(TM) i5-6500 CPU @ 3.20GHz (4 CPUs),
//				~3.2GHz 
//              Software: OS: Windows 10 
//              Compiles under Microsoft Visual C++ 2015
//
// Output:		grid to specified file
//
// Calls:		printGrid()
//
// Called By:	setships()
//
// Parameters:	player: sPlayer[];	the array of 2 players
//				whichPlayer: short; the player number (0 or 1) 
//				size: char;	'S' or 'L'
// 
// Returns:	void
//
// History Log:
//				12/20/05	PB		completed v0.1
//				01/27/17	AW LS	completed v0.2
//     
//----------------------------------------------------------------------------
void saveGrid(Player players[], short whichPlayer, char size)
{
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;
	// your code goes here ...

	string fileName;
	string extension = ".shp";

	cout <<
		"Please enter the name of the file to save to (.shp will be added): ";
	cin >> fileName;
	fileName.append(extension);
	ofstream fout(fileName.c_str());

	if (fout.is_open())
	{
		fout << size << endl;
		for (int i = 0; i < numberOfRows; i++)
		{
			for (int j = 0; j < numberOfCols; j++)
			{
				fout << (int)players[whichPlayer].m_gameGrid[0][i][j];
			}
			fout << endl;
		}
		fout << endl;
		fout.close();
	}
	else
	{
		cerr << "Could not open file to save grids to." << endl;
	}
	cout << endl << fileName << " saved." << endl;
	cin.ignore(BUFFER_SIZE, '\n');
	cin.get();
	system("cls");
}


//----------------------------------------------------------------------------
// Function:	getGrid()
// Title:		GetGrid 
// Description:
//				Reads grid from a file and properly sets the ships
// Programmer:	Paul Bladek
// modified by:	
// 
// Date:		01/31/17
//
// Version:		0.6
// 
// Environment: Hardware: Intel(R) Core(TM) i5-6500 CPU @ 3.20GHz (4 CPUs),
//				~3.2GHz
//              Software: OS: Windows 10 
//              Compiles under Microsoft Visual C++ 2015
//
// Input:		grid from specified file
//
// Output:		prompts to cout
//
// Calls:
//
// Called By:	main()
//
// Parameters:	players: Player[];	the array of 2 players
//				whichPlayer: short;	the player number  (0 or 1) 
//				size: char;		'S' or 'L'
//				string fileName: the name of the file to be opened for reading
// 
// Returns:		bool -- 	true if the file is opened and read;
//				false otherwise
//
// History Log: 
//				9/12/06 PB comleted v 0.5
//				01/27/17	AW LS	completed v0.6
//     
//----------------------------------------------------------------------------
bool getGrid(Player players[], short whichPlayer, char size, string fileName)
{
	string line;
	ifstream ifs;
	Ship ship = NOSHIP;
	short shipCount[SHIP_SIZE_ARRAYSIZE] = { 0 };
	char cell = ' ';
	char fsize = 'A';
	int location = 0;
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;

	try
	{
		ifs.open(fileName.c_str());
		if (!ifs)
		{
			cout << "could not open file " << fileName << endl;
			cin.ignore(BUFFER_SIZE, '\n');
			return false;
		}
	}
	catch (exception e)
	{
		cout << "could not open file " << fileName << endl;
		cin.ignore(BUFFER_SIZE, '\n');
		return false;
	}
	// your code goes here ...
	ifs >> fsize;
	if (fsize == size)
	{
		for (int i = 0; i < numberOfRows; i++)
		{
			for (int j = 0; j < numberOfCols; j++)
			{
				ifs.get(cell);
				location = (int)(cell - 48);
				switch (location)
				{
				case 0:
					ship = NOSHIP;
					players[whichPlayer].m_gameGrid[0][i][j] = ship;
					break;
				case 1:
					ship = MINESWEEPER;
					players[whichPlayer].m_gameGrid[0][i][j] = ship;
					break;
				case 2:
					ship = SUB;
					players[whichPlayer].m_gameGrid[0][i][j] = ship;
					break;
				case 3:
					ship = FRIGATE;
					players[whichPlayer].m_gameGrid[0][i][j] = ship;
					break;
				case 4:
					ship = BATTLESHIP;
					players[whichPlayer].m_gameGrid[0][i][j] = ship;
					break;
				case 5:
					ship = CARRIER;
					players[whichPlayer].m_gameGrid[0][i][j] = ship;
					break;
				default:
					j--;

				}
			}
		}
	}
	return true;
}

//----------------------------------------------------------------------------
// Function:	getCoord()
// Title:		Get Coordinates 
// Description:
//				Returns a cell with coordinates set by user
// Programmer:	Paul Bladek
// 
// Date:		01/31/17
//
// Version:		1.1
// 
// Environment: Hardware: Intel(R) Core(TM) i5-6500 CPU @ 3.20GHz (4 CPUs),
//				~3.2GHz
//              Software: OS: Windows 10 
//              Compiles under Microsoft Visual C++ 2015
//
// Input:		cell coordinates (in the form "A13" from sin
//
// Output:		prompts to cout
//
// Calls:		none
//
// Called By:	main()
//				setships()
//
// Parameters:	sin: istream&;	the stream to read from
//				size: char;	'S' or 'L'
// 
// Returns:		Cell location -- a cell containing the input coordinates
//
// History Log: 
//				9/12/06 PB comleted v 1.0
//				01/27/17 AW LS completed 1.1
//     
//----------------------------------------------------------------------------
Cell getCoord(istream& sin, char size)
{
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;
	char highChar = static_cast<char>(numberOfRows - 1) + 'A';
	char row = 'A';
	short col = 0;
	Cell location = { 0, 0 };
	do
	{
		col = 0;
		cout << "Row must be a letter from A to " << highChar
			<< " and column must be  from 1 to " << numberOfCols << ": ";
		while ((row = toupper(sin.get())) < 'A' || row > highChar)
		{
			sin.ignore(BUFFER_SIZE, '\n');
			cout << "Row must be a letter from A to " << highChar
				<< " and column must be  from 1 to " << numberOfCols << ": ";
		}
		sin >> col;
		if (!sin)
			sin.clear();
		sin.ignore(BUFFER_SIZE, '\n');
	} while (col < 1 || col > numberOfCols);

	location.m_col = col - 1;
	location.m_row = static_cast<short>(row - 'A');
	return location;
}

//----------------------------------------------------------------------------
// Function:	validLocation()
// Title:		Valid Location 
// Description:
//				Can the ship legitimately go there?
// Programmer:  Paul Bladek
//				Anthony Waddell
//				Lael Smith
// 
// Date:		01/31/17
//
// Version:		0.2
// 
// Environment: Hardware: Intel(R) Core(TM) i5-6500 CPU @ 3.20GHz (4 CPUs)
//				~3.2GHz 
//              Software: OS: Windows 10 
//              Compiles under Microsoft Visual C++ 2015
//
// Calls:		None
//
// Called By:	SetShips()
//
// Parameters:	player: const Player&;	a reference to the specific Player
//				shipNumber: short;	the number of the ship (1 - 5)
//					in the array player.m_ships[]
//				size: char;		'S' or 'L'
// 
// Returns:		bool -- true if the ship would not go off the edge
//				or cross another ship;
//				false otherwise
//
// History Log:
//				12/20/05	PB completed v 0.1
//				01/27/17	AW LS completed v0.2
//     
//----------------------------------------------------------------------------
bool validLocation(const Player& player, short shipNumber, char size)
{
	short numberOfRows = (toupper(size) == 'L') ? LARGEROWS : SMALLROWS;
	short numberOfCols = (toupper(size) == 'L') ? LARGECOLS : SMALLCOLS;
	bool valid = true;

	Cell bowLocation = player.m_ships[shipNumber].m_bowLocation;
	if (player.m_ships[shipNumber].m_bowLocation.m_col >= numberOfCols)
	{
		valid = false;
	}

	if (player.m_ships[shipNumber].m_bowLocation.m_row >= numberOfRows)
	{
		valid = false;
	}
	if (player.m_ships[shipNumber].m_orientation == VERTICAL && valid == true)
	{
		if (player.m_ships[shipNumber].m_bowLocation.m_row 
			+ (player.m_ships[shipNumber].m_piecesLeft - 1) >= numberOfRows)
		{
			valid = false;
		}
	}
	if (player.m_ships[shipNumber].m_orientation ==
		HORIZONTAL && valid == true)
	{
		if (player.m_ships[shipNumber].m_bowLocation.m_col
			+ (player.m_ships[shipNumber].m_piecesLeft - 1) >= numberOfCols)
		{
			valid = false;
		}
	}
	if (player.m_ships[shipNumber].m_orientation == VERTICAL && valid == true)
	{
		for (int i = 0; i < player.m_ships[shipNumber].m_piecesLeft; i++)
		{
			if (player.m_gameGrid[0][player.m_ships[shipNumber]
				.m_bowLocation.m_row + i][player.m_ships[shipNumber]
				.m_bowLocation.m_col] != NOSHIP)
			{
				valid = false;
			}
		}
	}
	if (player.m_ships[shipNumber].m_orientation == HORIZONTAL && valid 
		== true)
	{
		for (int i = 0; i < player.m_ships[shipNumber].m_piecesLeft; i++)
		{
			if (player.m_gameGrid[0][player.m_ships[shipNumber].m_bowLocation
				.m_row][player.m_ships[shipNumber]
				.m_bowLocation.m_col + i] != NOSHIP)
			{
				valid = false;
			}
		}
	}

	return valid;
}

//----------------------------------------------------------------------------
// Function:	header()
// Title:		header 
// Description:
//				Prints opening graphic
// Programmer:	Paul Bladek
//				Lael Smith
//				Anthony Waddell
// 
// Date:		9/12/06
//
// Version:		1.1
// 
// Environment: Hardware: Intel(R) Core(TM) i5-6500 CPU @ 3.20GHz (4 CPUs),
//				~3.2GHz
//              Software: OS: Windows 10 
//              Compiles under Microsoft Visual C++ 2015
//
// Output:	
//
// Calls:		boxTop()
//				boxLine()
//				boxBottom()
//
// Called By:	main()
//
// Parameters:	sout: ostream&;	the stream to print to
// 
// Returns:	void
//
// History Log: 
//				9/12/06 PB comleted v 1.0
//				01/10/17 AW LS updated boxline v 1.1
//     
//----------------------------------------------------------------------------
void header(ostream& sout)
{
	const string empty;
	const string sink("SINK THE FLEET!");
	const string names("By DeathRule");
	const string by("Edmonds Community College CS 132");
	boxTop(sout, BOXWIDTH);
	boxLine(sout, empty, BOXWIDTH);
	boxLine(sout, sink, BOXWIDTH, 'C');
	boxLine(sout, empty, BOXWIDTH);
	boxLine(sout, names, BOXWIDTH, 'C');
	boxLine(sout, empty, BOXWIDTH);
	boxLine(sout, empty, BOXWIDTH);
	boxLine(sout, by, BOXWIDTH, 'C');
	boxLine(sout, empty, BOXWIDTH);
	boxBottom(sout, BOXWIDTH);
}

//----------------------------------------------------------------------------
// Function:	endBox()
// Title:		End Box 
// Description:
//				prints closinging graphic
// Programmer:	Paul Bladek
// 
// Date:		9/12/06
//
// Version:		1.0
// 
// Environment: Hardware: Intel(R) Core(TM) i5-6500 CPU @ 3.20GHz (4 CPUs),
//				~3.2GHz
//              Software: OS: Windows 10 
//              Compiles under Microsoft Visual C++ 2015
//
// Output:	
//
// Calls:		boxTop()
//				boxLine()
//				boxBottom()
//
// Called By:	main()
//
// Parameters:	player: short; the number of the winner (0 or 1)
// 
// Returns:		void
//
// History Log: 
//				9/12/06 PB comleted v 1.0
//     
//----------------------------------------------------------------------------
void endBox(short player)
{
	const string empty;
	ostringstream msg;
	msg << "Congratulations player " << player + 1 << "!";
	boxTop(cout, BOXWIDTH);
	boxLine(cout, empty, BOXWIDTH);
	boxLine(cout, msg.str(), BOXWIDTH, 'C');
	boxLine(cout, empty, BOXWIDTH);
	boxBottom(cout, BOXWIDTH);
}