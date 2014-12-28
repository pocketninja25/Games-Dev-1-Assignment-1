//Includes and defines for memory leak detection
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include "AStar.h"
#include <string>

int Main()	//Additional Main() function to let STL containers go out of scope and 'clean up' memory before memory leak detection in main
{
	AStar* pathfinding = new AStar();

	string mapFile = "mMap.txt";
	string coordsFile = "mCoords.txt";
	string outputFile = "output.txt";

	if (!pathfinding->LoadMapAndCoords(mapFile, coordsFile))
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

		if (pathfinding->FindPath())
		{
			pathfinding->DisplayPath();
			if (!pathfinding->SavePath(outputFile))
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