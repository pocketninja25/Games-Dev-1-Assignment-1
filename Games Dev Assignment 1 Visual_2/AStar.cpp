#include "AStar.h"

void CAStar::GetSuccessors(CCoords* pCurrent, CCoords* pSuccessors[4])
{
	//Generates up to 4 new successors (providing they are possible) - otherwise returned successor is 0
	int x = pCurrent->GetX();
	int y = pCurrent->GetY();

	for (int i = 0; i < 4; i++)	//Initialise the successor pointers to 0
	{
		pSuccessors[i] = NULL;
	}

	//Make the successor pointers point at the four connecting nodes to current - providing the coordinates of the new nodes are in range
	if ((y + 1) < g_MAP_ROWS)
	{
		pSuccessors[0] = mpGrid[x][y + 1];
	}
	if ((x + 1) < g_MAP_COLS)
	{
		pSuccessors[1] = mpGrid[x + 1][y];
	}
	if ((y - 1) >= 0)
	{
		pSuccessors[2] = mpGrid[x][y - 1];
	}
	if ((x - 1) >= 0)
	{
		pSuccessors[3] = mpGrid[x - 1][y];
	}

	for (int i = 0; i < 4; i++)
	{
		if (pSuccessors[i] != NULL)	//Only perform additional checks for non null nodes
		{
			//Dont let current generate its own parent as a new node and don't let a node be generated if it is impassable (a wall)
			if (pSuccessors[i] == pCurrent->GetParent() || pSuccessors[i]->GetIndividualCost() == costWall)
			{
				//Reset the successor to null
				pSuccessors[i] = NULL;
			}
		}
	}
}

CAStar::CAStar()
{
	mpOpenList = new COrderedList(CCoords::Compare);
	mpGrid[g_MAP_COLS][g_MAP_ROWS] = { NULL };
	mMapLoaded = false;
	mCoordsLoaded = false;
	mPathFound = false;
	mpStartNode = NULL;
	mpEndNode = NULL;
}

CAStar::~CAStar()
{
	for (int i = 0; i < g_MAP_COLS; i++)
	{
		for (int j = 0; j < g_MAP_ROWS; j++)
		{
			delete mpGrid[i][j];
		}
	}
	delete mpOpenList;

}

bool CAStar::LoadMapAndCoords(string iMapFile, string iCoordsFile, std::ifstream &fileStream)
{
	//Attempts to load the map and coords from file, returning false if it was unssucessful at loading

	/************Map File************/

	//Create filestream
	fileStream.close();
	fileStream.clear();
	fileStream.open(iMapFile);

	if (!fileStream)	//If there is a problem with the filestream (failed to load file etc)
	{
		mMapLoaded = false;
		return false;
	}

	//Initialise counts for positioning the costs and emplacing them in the array
	int row = g_MAP_ROWS - 1;	//Filling the rows from bottom to top (bottom left is (0,0))
	int col = 0;				//Filling the columns from left to right

	//Input variables
	char inChar = NULL;			//Storage for the character from the file
	int convertedVal = NULL;	//Storage for the character being converted to an integer

	while (fileStream && fileStream >> inChar)	//Read from the file - will prevent the end of file marker being read as a number
	{
		convertedVal = inChar - '0';	//Convert the character read to an integer

		//Create new pointer of the position and inputted cost into the grid (using maths to emplace items into the grid by their row and column values)
		mpGrid[col][row] = new CCoords(col, row, (ETraversalCost)convertedVal, nodeStandard);

		//Prepare the counting variables for the next pass
		col++;
		if (col >= g_MAP_COLS)
		{
			col = 0;
			row--;
		}
	}
	fileStream.close();
	fileStream.clear();
	mMapLoaded = true;

	/************Coords File************/
	//Attempts to load the start and end coordinates from file, returning false if it was unssucessful at loading
	fileStream.open(iCoordsFile);

	if (!fileStream)	//If there is a problem with the filestream (failed to load file etc)
	{
		mCoordsLoaded = false;
		return false;
	}

	char inChX = NULL;
	char inChY = NULL;
	int x = 0;
	int y = 0;

	//Read in start coordinates and set the type of the read-in node to 'nodeStart'
	fileStream >> inChX >> inChY;
	x = inChX - '0';
	y = inChY - '0';
	mpGrid[x][y]->SetType(nodeStart);
	mpStartNode = mpGrid[x][y];

	//Read in end coordinates and set the type of the read-in node to 'nodeEnd'
	fileStream >> inChX >> inChY;
	x = inChX - '0';
	y = inChY - '0';
	mpGrid[x][y]->SetType(nodeEnd);
	mpEndNode = mpGrid[x][y];

	fileStream.close();
	fileStream.clear();

	mCoordsLoaded = true;
	return true;
}

bool CAStar::FindPath()
{
	bool goalFound = false;				//Whether or not a path to the end has been found
	if (mMapLoaded && mCoordsLoaded)
	{
		//Performs a pathfinding algorithm (A*) to find the optimal route from the start and finish points specified in the pGrid array
		//When successful builds the path from the child pointer to the end pointer which can be followed using start pointer's member variable child)
		//Returns whether or not a path could be found

		mpStartNode->TrySetParent(NULL);			//Set the parent of the start node to null
		mpStartNode->TrySetManhatDist(mpEndNode);	//Set (calculate) the manhattan distance of the start node with the end node
		mpOpenList->Push(mpStartNode);

		//Create pointers for use in algorithm
		CCoords* pCurrent = NULL;
		CCoords* pSuccessors[4] = { NULL };	//The 4 nodes generated from the current node
		bool parentChanged = false;			//Whether the set parent was successful

		do	//Keep going until a goal is found or there cannot be a goal (open list is empty)
		{
			pCurrent = mpOpenList->PopFront();	//Pop the lowest score item from the list
			if (pCurrent == mpEndNode)
			{
				goalFound = true;
			}
			else //Current is not the goal
			{
				GetSuccessors(pCurrent, pSuccessors);	//Get pointers to the successor nodes
				for (int i = 0; i < 4; i++)	//For each successor that can match 'current'
				{
					parentChanged = false;	//Reset parentChanged for each successor

					if (pSuccessors[i] != NULL)	//If the rule was successfully applied (is pointing at a successor node)
					{
						parentChanged = pSuccessors[i]->TrySetParent(pCurrent);	//parentSet now reflects whether the parent changed or not
						if (parentChanged)
						{
							//Remove instances of this pointer from the closed list
							mClosedList.remove(pSuccessors[i]);
							//Remove from the open list (if there) so that it can be inserted into the correct position without sorting the whole list
							mpOpenList->Remove(pSuccessors[i]);
							//Add to the open list (in the correct position)
							mpOpenList->Push(pSuccessors[i]);
						}
					}
				}
				mClosedList.push_back(pCurrent);
			}
		} while (!goalFound && !mpOpenList->IsEmpty());

		if (goalFound)
		{
			//Assemble path
			mpEndNode->GeneratePath();
		}
	}
	mPathFound = goalFound;
	return goalFound;
}

bool CAStar::DisplayMap()
{
	if (mMapLoaded)	//Can't output if there is no map to output
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
				cout << mpGrid[col][row]->GetCost();
			}
			cout << endl;
		}
		cout << endl;
		return true;
	}
	return false;
}

bool CAStar::CreateMapModels(CFloorTile* models[g_MAP_COLS][g_MAP_ROWS], IMesh* tileMesh)
{
	if (mMapLoaded)
	{
		for (int i = 0; i < g_MAP_COLS; i++)
		{
			for (int j = 0; j < g_MAP_ROWS; j++)
			{
				models[i][j] = new CFloorTile(tileMesh, (EFloorType)mpGrid[i][j]->GetIndividualCost(), i, j);
			}
		}
		return true;
	}
	return false;
}

void CAStar::DisplayPath()
{
	if (mPathFound)	//If a path from start to end has been found
	{
		CCoords* pCurrent = mpStartNode;
		cout << "Pathfinding Solution - Nodes from beginning to end" << endl;
		while (pCurrent != NULL)
		{
			cout << "X: " << pCurrent->GetX() << " Y: " << pCurrent->GetY() << endl;
			pCurrent = pCurrent->GetChild();
		}
	}
}

bool CAStar::SavePath(string fileName, std::ofstream &fileStream)
{
	if (mPathFound)
	{
		fileStream.close();
		fileStream.clear();
		fileStream.open(fileName);

		if (!fileStream)
		{
			return false;
		}

		CCoords* pCurrent = mpStartNode;

		while (fileStream && pCurrent != NULL)
		{
			//Output Coordinates to file
			fileStream << pCurrent->GetX() << ' ' << pCurrent->GetY() << endl;
			//Set new current value for next pass (next coordinate in sequence)
			pCurrent = pCurrent->GetChild();
		}

		fileStream.close();
		fileStream.clear();
		return true;
	}
	return false;
}

bool CAStar::MapLoaded()
{
	return mMapLoaded;
}

bool CAStar::CoordsLoaded()
{
	return mCoordsLoaded;
}

bool CAStar::PathFound()
{
	return mPathFound;
}

int CAStar::GetStartX()
{
	if (mMapLoaded && mCoordsLoaded)
	{
		return mpStartNode->GetX();
	}
	return 0;
}

int CAStar::GetStartY()
{
	if (mMapLoaded && mCoordsLoaded)
	{
		return mpStartNode->GetY();
	}
	return 0;
}

int CAStar::GetEndX()
{
	if (mMapLoaded && mCoordsLoaded)
	{
		return mpEndNode->GetX();
	}
	return 0;
}

int CAStar::GetEndY()
{
	if (mMapLoaded && mCoordsLoaded)
	{
		return mpEndNode->GetY();
	}
	return 0;
}