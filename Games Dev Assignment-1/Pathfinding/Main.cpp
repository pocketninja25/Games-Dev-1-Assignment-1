//Includes and defines for memory leak detection
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <fstream>
#include "CList.h"
#include <string>

const int g_MAP_COLS = 10;							//Number of Columns (X Coordinates of the map)
const int g_MAP_ROWS = 10;							//Number of Rows (Y Coordinates of the map)

void DisplayMap(CCoords* pGrid[g_MAP_COLS][g_MAP_ROWS])		 
{
	//Display the map as it is written in the loaded .txt file
	//This is a debug tool to check the map has loaded correctly

	//Count bottom to top for rows and left to right for columns
	//because the bottom left of the map is (0, 0)
	cout << "Map Layout & Costs" << endl;

	for (int row = g_MAP_ROWS - 1; row >= 0; row--)
	{
		for (int col = 0; col < g_MAP_COLS; col++)
		{
			cout << pGrid[col][row]->GetCost();
		}
		cout << endl;
	}
	cout << endl;
}

bool LoadMap(CCoords* pGrid[g_MAP_COLS][g_MAP_ROWS], string fileName)
{
	//Attempts to load the map from file, returning false if it was unssucessful at loading

	//Create filestream
	ifstream mapFile = ifstream(fileName);

	if (!mapFile)	//If there is a problem with the filestream (failed to load file etc)
	{
		return false;
	}

	//Initialise counts for positioning the costs and emplacing them in the array
	int row = g_MAP_ROWS - 1;	//Filling the rows from bottom to top (bottom left is (0,0))
	int col = 0;				//Filling the columns from left to right

	//Input variables
	char inChar = NULL;			//Storage for the character from the file
	int convertedVal = NULL;	//Storage for the character being converted to an integer

	while (mapFile && mapFile >> inChar)	//Read from the file - will prevent the end of file marker being read as a number
	{
		convertedVal = inChar - '0';	//Convert the character read to an integer

		//Create new pointer of the position and inputted cost into the grid (using maths to emplace items into the grid by their row and column values)
		pGrid[col][row] = new CCoords(col, row, (ETraversalCost)convertedVal, nodeStandard);

		//Prepare the counting variables for the next pass
		col++;
		if (col >= g_MAP_COLS)
		{
			col = 0;
			row--;
		}

	}
	mapFile.close();
	return true;
}

bool LoadCoords(CCoords* pGrid[g_MAP_COLS][g_MAP_ROWS], string fileName)
{
	//Attempts to load the start and end coordinates from file, returning false if it was unssucessful at loading
	ifstream coordsFile = ifstream(fileName);

	if (!coordsFile)	//If there is a problem with the filestream (failed to load file etc)
	{
		return false;
	}
	
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
	return true;
}

void GetSuccessors(CCoords* pGrid[g_MAP_COLS][g_MAP_ROWS], CCoords* pCurrent, CCoords* pSuccessors[4])
{
	//Generates up to 4 new rules (providing they are possible) - otherwise returned rule is 0
	int x = pCurrent->GetX();
	int y = pCurrent->GetY();

	for (int i = 0; i < 4; i++)	//Initialise the rules pointers to 0
	{
		pSuccessors[i] = NULL;
	}
	
	//Make the rules pointers point at the four connecting nodes to current - providing the coordinates of the new nodes are in range
	if ((y + 1) < g_MAP_ROWS)
	{
		pSuccessors[0] = pGrid[x][y + 1];
	}
	if ((x + 1) < g_MAP_COLS)
	{
		pSuccessors[1] = pGrid[x + 1][y];
	}
	if ((y - 1) >= 0)
	{
		pSuccessors[2] = pGrid[x][y - 1];
	}
	if ((x - 1) >= 0)
	{
		pSuccessors[3] = pGrid[x - 1][y];
	}

	for (int i = 0; i < 4; i++)
	{
		if (pSuccessors[i] != NULL)	//Only perform additional checks for non null nodes
		{
			//Dont let current generate its own parent as a new node and don't let a node be generated if it is impassable (a wall)
			if (pSuccessors[i] == pCurrent->GetParent() || pSuccessors[i]->GetIndividualCost() == costWall)
			{
				//Reset the rule to null
				pSuccessors[i] = NULL;
			}
		}
	}
}

CCoords* AStar(COrderedList* pOpenList, list<CCoords*> &closedList, CCoords* pGrid[g_MAP_COLS][g_MAP_ROWS])
{
	//Performs a pathfinding algorithm (A*) to find the optimal destination from the start and finish points specified in the pGrid array
	//When successful returns the pointer to the start node (which can be followed using it's member variable child to discover the path
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

	pStartNode->TrySetParent(NULL);			//Set the parent of the start node to null
	pStartNode->TrySetManhatDist(pEndNode);	//Set (calculate) the manhattan distance of the start node with the end node
	pOpenList->Push(pStartNode);

	//Create pointers for use in algorithm
	CCoords* pCurrent = NULL;
	CCoords* pSuccessors[4] = { NULL };	//The 4 nodes generated from the current node
	bool goalFound = false;				//Whether or not a path to the end has been found
	bool parentChanged = false;			//Whether the set parent was successful

	do	//Keep going until a goal is found or there cannot be a goal (open list is empty)
	{
		pCurrent = pOpenList->PopFront();	//Pop the lowest score item from the list
		if (pCurrent == pEndNode)
		{
			goalFound = true;
		}
		else //Current is not the goal
		{
			GetSuccessors(pGrid, pCurrent, pSuccessors);	//Get pointers to the new rules
			for (int i = 0; i < 4; i++)	//For each rule that can match 'current'
			{
				parentChanged = false;	//Reset parentSet for each rule to be applied

				if (pSuccessors[i] != NULL)	//If the rule was successfully applied (is pointing at a node)
				{
					parentChanged = pSuccessors[i]->TrySetParent(pCurrent);	//parentSet now reflects whether the parent changed or not
					if (parentChanged)
					{
						//Remove instances of this pointer from the closed list
						closedList.remove(pSuccessors[i]);
						//Remove from the open list (if there) so that it can be inserted into the correct position without sorting the whole list
						pOpenList->Remove(pSuccessors[i]);
						//Add to the open list (in the correct position)
						pOpenList->Push(pSuccessors[i]);
					}
				}
			}
			closedList.push_back(pCurrent);
		}
	} while (!goalFound && !pOpenList->Empty());

	if (goalFound)
	{
		//Assemble path
		pEndNode->GeneratePath();

		//Send the start node (which contains the path to the end) and quit
		return pStartNode;
	}
	else	//Goal not found
	{
		return NULL;
	}

}

bool OutputPathToFile(CCoords* pStartNode, string fileName)
{
	ofstream outFile = ofstream(fileName);

	if (!outFile)
	{
		return false;
	}

	CCoords* pCurrent = pStartNode;

	while (outFile && pCurrent != NULL)
	{
		//Output Coordinates to file
		outFile << pCurrent->GetX() << ' ' << pCurrent->GetY() << endl;
		//Set new current value for next pass (next coordinate in sequence)
		pCurrent = pCurrent->GetChild();
	}
	outFile.close();
	return true;

}

void OutputPathToScreen(CCoords* pStartNode)
{
	CCoords* pCurrent = pStartNode;
	cout << "Pathfinding Solution - Nodes from beginning to end" << endl;
	while (pCurrent != NULL)
	{
		cout << "X: " << pCurrent->GetX() << " Y: " << pCurrent->GetY() << endl;
		pCurrent = pCurrent->GetChild();

	} 
}

int Main()	//Additional Main() function to let STL containers go out of scope and 'clean up' memory before memory leak detection in main
{
	COrderedList* pOpenList = new COrderedList(CCoords::Compare);
	list<CCoords*> closedList;

	CCoords* pGrid[g_MAP_COLS][g_MAP_ROWS] = { NULL };	//Initialise grid CCoords pointers all to 0 (NULL)
	
	//Load the map
	string mapFile = "mMap.txt";
	bool mapLoaded = LoadMap(pGrid, mapFile);
	if (!mapLoaded)
	{
		cout << "Error Loading the map file " << mapFile << endl;
	}
	else //Map loaded successfully
	{
		DisplayMap(pGrid);
		
		//Load the start/end coordinates
		string coordsFile = "mCoords.txt";
		bool coordsLoaded = LoadCoords(pGrid, coordsFile);
		if (!coordsLoaded)
		{
			cout << "Error Loading the map file " << coordsFile << endl;
		}
		else
		{
			//Run the A* algorithm
			CCoords* pSolution = AStar(pOpenList, closedList, pGrid);
			
			//Output the results of the A* algorithm
			if (pSolution != NULL)
			{
				OutputPathToScreen(pSolution);
				bool saved = false;
				saved = OutputPathToFile(pSolution, "output.txt");
				if (!saved)
				{
					cout << "Error saving to file";
				}
			}
			else
			{
				cout << "Pathfinding failed, there is no route between the provided coordinates" << endl;
			}
		}
	}
	

	//Clear aquired memory
	delete pOpenList;
	for (int i = 0; i < g_MAP_COLS; i++)
	{
		for (int j = 0; j < g_MAP_ROWS; j++)
		{
			delete pGrid[i][j];
		}
	}
	return 0;
}

int main()	//The default main, use this to call the functional Main, pause the output after the functionality has completed and perform a memory leak dump
{
	Main();
	system("pause");
	_CrtDumpMemoryLeaks();
}