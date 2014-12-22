//Includes and defines for memory leak detection
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <fstream>
#include "CList.h"

const int g_MAP_ROWS = 10;
const int g_MAP_COLS = 10;

void DisplayMap(CCoords* grid[g_MAP_ROWS * g_MAP_COLS])
{
	//Display the map as it is written in the loaded .txt file 
	//This is for vanity purposes only - probably remove this before submitting
	for (int i = 0; i < 100; i++)
	{
		if (i % 10 == 0)
		{
			cout << endl;
		}
		cout << grid[i]->GetCost();
	}
	cout << endl;
}

void LoadMap(CCoords* grid[g_MAP_ROWS * g_MAP_COLS], string fileName)
{
	//Create filestream
	ifstream mapFile = ifstream(fileName);

	//Counts for positioning the costs and emplacing them in the array
	int row = 0;
	int col = 0;
	int count = 0;
	//Input variables
	char inChar = NULL;
	int convertedVal = NULL;

	while (mapFile && mapFile >> inChar)
	{
		//mapFile >> inChar;				//Read from file
		convertedVal = inChar - '0';	//Convert the character read to an integer

		//Create new pointer of the position and inputted cost into the grid (using maths to emplace items into the grid by their row and column values)
		grid[row * g_MAP_COLS + col] = new CCoords(0, row, col, convertedVal);

		if (mapFile)
		{
			//Prepare the counting variables for the next pass
			col++;
			if (col >= g_MAP_COLS)
			{
				col = 0;
				row++;
			}

		}
		
	}
	mapFile.close();

}

int Main()	//Additional Main() function to let STL containers go out of scope and 'clean up' memory before memory leak detection in main
{
	COrderedList* openList = new COrderedList(CCoords::Compare);
	list<CCoords*> closedList;

	CCoords* grid[g_MAP_ROWS * g_MAP_COLS] = { 0 };	//Initialise grid CCoords pointers all to 0
	
	LoadMap( grid, "mMap.txt" );


	for (int i = 0; i < 100; i++)
	{
		if (i % 10 == 0)
		{
			cout << endl;
		}
		cout << grid[i]->GetCost();
	}
	cout << endl;

	/*******TEMP********/
	CCoords* foo1 = new CCoords(0, 5, 1, 10);
	CCoords* foo2 = new CCoords(0, 4, 2, 20);
	CCoords* foo3 = new CCoords(0, 3, 3, 30);
	CCoords* foo4 = new CCoords(0, 2, 4, 40);
	CCoords* foo5 = new CCoords(0, 1, 5, 50);

	openList->Push(foo3);
	openList->Push(foo2);
	openList->Push(foo4);
	openList->Push(foo5);
	openList->Push(foo1);


	for (int i = 0; i < 5; i++)
	{
		//cout << openList->GetBack()->GetCost() << endl;
		//openList->PopBack();
	}

	/*******TEMP********/
	
	delete openList;

	for (int i = 0; i < (g_MAP_COLS * g_MAP_ROWS); i++)
	{
		delete grid[i];
	}

	return 0;
}

int main()
{
	Main();
	system("pause");
	_CrtDumpMemoryLeaks();
}