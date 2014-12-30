//Includes and defines for memory leak detection
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <string>
#include "AStar.h"
using namespace std;

int Main()	//Additional Main() function to let STL containers go out of scope and 'clean up' memory before memory leak detection in main
{
	ifstream inStream;
	ofstream outStream;

	CAStar* pathfinding = new CAStar();

	string mapFile = "mMap.txt";
	string coordsFile = "mCoords.txt";
	string outputFile = "output.txt";

	bool loadSuccess = pathfinding->LoadMapAndCoords(mapFile, coordsFile, inStream);

	if (!loadSuccess)
	{
		if (!pathfinding->MapLoaded())
		{
			cout << "Error Loading the map file " << mapFile << endl;
		}
		else if (!pathfinding->CoordsLoaded())
		{
			cout << "Error Loading the coordinate file " << coordsFile << endl;
		}
	}
	else
	{
		pathfinding->DisplayMap();
		bool pathFound = pathfinding->FindPath();

		if (pathFound)
		{
			pathfinding->DisplayPath();
			bool pathSaved = pathfinding->SavePath(outputFile, outStream);
			if (!pathSaved)
			{
				cout << "Error saving to file " << outputFile << endl;
			}
		}
		else
		{
			cout << "Pathfinding failed, there is no route between the provided coordinates" << endl;
		}
	}

	delete pathfinding;
	return 0;
}

int main()	//The default main, use this to call the functional Main, pause the output after the functionality has completed and perform a memory leak dump
{
	Main();
	system("pause");
	_CrtDumpMemoryLeaks();
}