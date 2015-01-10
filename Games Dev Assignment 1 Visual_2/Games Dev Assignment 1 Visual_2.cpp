// Games Dev Assignment 1 Visual.cpp: A program using the TL-Engine
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <TL-Engine.h>
#include <fstream>
#include "AStar.h"
#include "FloorTile.h"
#include "CLocationMarker.h"
using namespace tle;

const EKeyCode g_QUIT = Key_Escape;

float g_FrameTime = 0.0f;

int Main()
{
	std::ifstream inFileStream;
	std::ofstream outFileStream;
	
	CAStar* pathfinding = new CAStar();
	pathfinding->LoadMapAndCoords("dMap.txt", "dCoords.txt", inFileStream);
	pathfinding->DisplayMap();
	pathfinding->FindPath();
	pathfinding->DisplayPath();
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
		}
	}	//End of game Loop



	for (int i = 0; i < g_MAP_COLS; i++)
	{
		for (int j = 0; j < g_MAP_ROWS; j++)
		{
			delete floorModels[i][j];
		}
	}
	delete startMarker;
	delete endMarker;
	delete pathfinding;
	
	// Delete the 3D engine now we are finished with it
	gameEngine->Delete();

	return 0;
}

void main()
{
	Main();

	_CrtDumpMemoryLeaks();

}
