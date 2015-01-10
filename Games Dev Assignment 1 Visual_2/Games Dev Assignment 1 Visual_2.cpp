// Games Dev Assignment 1 Visual.cpp: A program using the TL-Engine
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <TL-Engine.h>
#include <sstream>
#include <fstream>
#include "AStar.h"
#include "FloorTile.h"
#include "CLocationMarker.h"
using namespace tle;

const EKeyCode g_QUIT = Key_Escape;
const EKeyCode g_CONTINUE = Key_Return;

float g_FrameTime = 0.0f;

string GetMapPrefix()
{
	string prefix = "";
	cout << "Enter a map prefix to load: ";
	cin >> prefix;
	return prefix;
}

string CombineStrings(string str1 = "", string str2 = "", string str3 = "")
{
	stringstream combineStream;
	combineStream << str1 << str2 << str3;
	return combineStream.str();
}

int Main()
{
	std::ifstream inFileStream;
	std::ofstream outFileStream;
	
	bool loadNewMap = true;		//Indicates whether a new map will be loaded upon engine closure

	CAStar* pathfinding = new CAStar();

	while (loadNewMap)
	{
		string mapFolder = ".//Media//Maps//";
		string mapSuffix = "Map.txt";
		string CoordsSuffix = "Coords.txt";
		string prefix = "";

		prefix = GetMapPrefix();

		while (!pathfinding->LoadMapAndCoords(CombineStrings(mapFolder, prefix, mapSuffix), CombineStrings(mapFolder, prefix, CoordsSuffix), inFileStream))
		{
			cout << "ERROR: Map does not exist " << endl << endl;
			mapSuffix = "Map.txt";
			CoordsSuffix = "Coords.txt";
			prefix = "";

			prefix = GetMapPrefix();
		}

		cout << endl;
		pathfinding->DisplayMap();
		pathfinding->FindPath();
		pathfinding->DisplayPath();
		cout << endl << endl;
		pathfinding->SavePath("output.txt", outFileStream);

		// Create a 3D engine (using TLX engine here) and open a window for it
		I3DEngine* gameEngine = New3DEngine(kTLX);
		gameEngine->StartWindowed();

		// Add default folder for meshes and other media
		gameEngine->AddMediaFolder(".//Media");

		/**** Set up your scene here ****/

		IMesh* cubeMesh = gameEngine->LoadMesh("cube.x");
		CFloorTile* floorModels[g_MAP_COLS][g_MAP_ROWS];
		pathfinding->CreateMapModels(floorModels, cubeMesh);

		CLocationMarker* startMarker = new CLocationMarker(cubeMesh, markerGreen, pathfinding->GetStartX(), pathfinding->GetStartY());
		CLocationMarker* endMarker = new CLocationMarker(cubeMesh, markerRed, pathfinding->GetEndX(), pathfinding->GetEndY());

		ICamera* debugCam = gameEngine->CreateCamera(kFPS, 45.0f, 105.0f, 45.0f);
		debugCam->SetMovementSpeed(25.0f);
		debugCam->SetRotationSpeed(50.0f);
		debugCam->RotateX(90.0f);



		// The main game loop, repeat until engine is stopped
		while (gameEngine->IsRunning())
		{
			// Draw the scene
			gameEngine->DrawScene();
			g_FrameTime = gameEngine->Timer();

			/**** Update your scene each frame here ****/
			if (gameEngine->KeyHit(g_QUIT))
			{
				gameEngine->Stop();
				loadNewMap = false;
			}
			if (gameEngine->KeyHit(g_CONTINUE))
			{
				gameEngine->Stop();
			}

			if (pathfinding->PathFound())
			{

			}

		}	//End of game Loop


		gameEngine->RemoveCamera(debugCam);
		delete startMarker;
		delete endMarker;

		for (int i = 0; i < g_MAP_COLS; i++)
		{
			for (int j = 0; j < g_MAP_ROWS; j++)
			{
				delete floorModels[i][j];
			}
		}

		gameEngine->RemoveMesh(cubeMesh);
		gameEngine->Delete();

	}	//End of new map load loop
	
	delete pathfinding;

	return 0;
}

void main()
{
	Main();

	_CrtDumpMemoryLeaks();

}
