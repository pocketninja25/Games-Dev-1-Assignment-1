//Includes and defines for memory leak detection
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <fstream>
#include "CList.h"

const int g_MAP_ROWS = 10;
const int g_MAP_COLS = 10;
const int g_NUM_NODES = g_MAP_COLS * g_MAP_ROWS;

void DisplayMap(CCoords* pGrid[g_MAP_COLS][g_MAP_ROWS])
{
	//Display the map as it is written in the loaded .txt file 
	//This is for vanity purposes only - probably remove this before submitting - doesnt work unless are in order
	for (int row = 0; row < g_MAP_ROWS; row++)
	{
		for (int col = 0; col < g_MAP_COLS; col++)
		{
			cout << pGrid[col][row]->GetCost();
		}
		cout << endl;
	}
	cout << endl;
}

void LoadMap(CCoords* pGrid[g_MAP_COLS][g_MAP_ROWS], string fileName)
{
	//Create filestream
	ifstream mapFile = ifstream(fileName);

	//Counts for positioning the costs and emplacing them in the array
	int row = 0;
	int col = 0;

	//Input variables
	char inChar = NULL;
	int convertedVal = NULL;

	while (mapFile && mapFile >> inChar)
	{
		//mapFile >> inChar;			//Read from file
		convertedVal = inChar - '0';	//Convert the character read to an integer

		//Create new pointer of the position and inputted cost into the grid (using maths to emplace items into the grid by their row and column values)
		pGrid[col][row] = new CCoords(col, row, convertedVal, nodeStandard);

		//Prepare the counting variables for the next pass
		col++;
		if (col >= g_MAP_COLS)
		{
			col = 0;
			row++;
		}

	}
	mapFile.close();

}

void LoadCoords(CCoords* pGrid[g_MAP_COLS][g_MAP_ROWS], string fileName)
{
	ifstream coordsFile = ifstream(fileName);

	char inChX = NULL;
	char inChY = NULL;
	int x = 0;
	int y = 0;

	//Read in start coordinates and set the type of the read-in node to 'nodeStart'
	coordsFile >> inChX >> inChY;
	x = inChX - '0';
	y = inChY - '0';
	pGrid[x][y]->SetType(nodeStart);

	//Read in end coordinates and set the type of the read-in node to 'nodeEnd'
	coordsFile >> inChX >> inChY;
	x = inChX - '0';
	y = inChY - '0';
	pGrid[x][y]->SetType(nodeEnd);

	coordsFile.close();
}

void ApplyRule(CCoords* pGrid[g_MAP_COLS][g_MAP_ROWS], CCoords* current, CCoords* rules[4])
{
	//Generates up to 4 new rules (providing they are possible) - otherwise returned rule is 0
	int x = current->GetX();
	int y = current->GetY();

	for (int i = 0; i < 4; i++)	//Initialise the rules pointers to 0
	{
		rules[i] = NULL;
	}

	//Make the rules pointers point at the four connecting nodes to current - providing the numbers are in range
	if ((y + 1) < g_MAP_ROWS)
	{
		rules[0] = pGrid[x][y + 1];
	}
	if ((x + 1) < g_MAP_COLS)
	{
		rules[1] = pGrid[x + 1][y];
	}
	if ((y - 1) >= 0)
	{
		rules[2] = pGrid[x][y - 1];
	}
	if ((x - 1) >= 0)
	{
		rules[3] = pGrid[x - 1][y];
	}

	for (int i = 0; i < 4; i++)
	{
		if (rules[i] == current->GetParent())
		{
			rules[i] = 0;
		}
	}
}

CCoords* AStar(COrderedList* pOpenList, list<CCoords*> &closedList, CCoords* pGrid[g_MAP_COLS][g_MAP_ROWS])
{
	//Performs a pathfinding algorithm (A*) to find the optimal destination from the start and finish points specified in the pGrid array
	//When successful returns the pointer to the end node (which can be followed using it's mParent variable to discover the path
	//When not successful returns a null pointer (0)

	//Note the start and end nodes
	CCoords* pStartNode = NULL;
	CCoords* pEndNode = NULL;

	//Find the start and end nodes
	for (int i = 0; i < g_MAP_COLS; i++)
	{
		for (int j = 0; j < g_MAP_ROWS; j++)
		{
			if (pGrid[i][j]->GetType() == nodeStart)
			{
				pStartNode = pGrid[i][j];
			}
			else if (pGrid[i][j]->GetType() == nodeEnd)
			{
				pEndNode = pGrid[i][j];
			}
		}
	}
	pStartNode->TrySetParent(NULL);
	pStartNode->TrySetManhatDist(pEndNode);
	pOpenList->Push(pStartNode);

	CCoords* pCurrent = NULL;
	bool goalFound = false;
	CCoords* pRules[4] = { NULL };	//The 4 nodes generated from the current node

	bool parentChanged = false;	//Whether the set parent was successful

	do
	{
		pCurrent = pOpenList->PopFront();
		if (pCurrent == pEndNode)
		{
			goalFound = true;
		}
		else //Current is not the goal
		{
			ApplyRule(pGrid, pCurrent, pRules);	//Get pointers to the new rules
			for (int i = 0; i < 4; i++)	//For each rule that can match 'current'
			{
				parentChanged = false;	//Reset parentSet for each rule to be applied

				if (pRules[i] != NULL)	//If the rule was successfully applied (is pointing at a node)
				{
					parentChanged = pRules[i]->TrySetParent(pCurrent);	//parentSet now reflects whether the parent changed or not

					if (parentChanged)
					{
						//Remove instances of this pointer from the closed list
						closedList.remove(pRules[i]);
						//Remove from the open list (if there) so that it can be inserted into the correct position without sorting the whole list
						pOpenList->Remove(pRules[i]);
						//Add to the open list (in the correct position)
						pOpenList->Push(pRules[i]);
					}
				}
			}

			closedList.push_back(pCurrent);

		}
		


	} while (!goalFound && !pOpenList->Empty());

	if (goalFound)
	{
		//Construct the path and quit
		return pCurrent;
	}
	else //Goal not found
	{
		return NULL;
	}

}

int Main()	//Additional Main() function to let STL containers go out of scope and 'clean up' memory before memory leak detection in main
{
	COrderedList* pOpenList = new COrderedList( CCoords::Compare );
	list<CCoords*> closedList;

	CCoords* pGrid[g_MAP_COLS][g_MAP_ROWS] = { NULL };	//Initialise grid CCoords pointers all to 0 (NULL)
	
	LoadMap( pGrid, "mMap.txt" );
	LoadCoords( pGrid, "mCoords.txt" );

	DisplayMap( pGrid );
	
	CCoords* solution = AStar(pOpenList, closedList, pGrid);

	do
	{
		cout << "X: " << solution->GetX() << " Y: " << solution->GetY() << endl;
		solution = solution->GetParent();

	} while (solution != NULL);

	//Clear aquired memory
	delete pOpenList;
	for ( int i = 0; i < g_MAP_COLS; i++ )
	{
		for (int j = 0; j < g_MAP_ROWS; j++)
		{
			delete pGrid[i][j];
		}
	}
	return 0;
}

int main()
{
	Main();
	system( "pause" );
	_CrtDumpMemoryLeaks();
}