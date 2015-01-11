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
#include "CActor.h"
using namespace tle;

const EKeyCode g_QUIT = Key_Escape;
const EKeyCode g_CONTINUE = Key_Return;
const EKeyCode g_NEXTPLY = Key_Space;

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

	CAStar* pPathfinding = new CAStar();

	while (loadNewMap)
	{
		string mapFolder = ".//Media//Maps//";
		string mapSuffix = "Map.txt";
		string CoordsSuffix = "Coords.txt";
		string prefix = "";
		string outputFile = CombineStrings(mapFolder, "Output.txt");

		prefix = GetMapPrefix();

		while (!pPathfinding->LoadMapAndCoords(CombineStrings(mapFolder, prefix, mapSuffix), CombineStrings(mapFolder, prefix, CoordsSuffix), inFileStream))
		{
			cout << "ERROR: Map does not exist " << endl << endl;
			mapSuffix = "Map.txt";
			CoordsSuffix = "Coords.txt";
			prefix = "";

			prefix = GetMapPrefix();
		}

		cout << endl;
		pPathfinding->DisplayMap();
		//pPathfinding->FindPath();	//Full version of find path
		//pPathfinding->DisplayPath();
		//cout << endl << endl;
		//pPathfinding->SavePath(outputFile, outFileStream);

		// Create a 3D engine (using TLX engine here) and open a window for it
		I3DEngine* pGameEngine = New3DEngine(kTLX);
		pGameEngine->StartWindowed();

		// Add default folder for meshes and other media
		pGameEngine->AddMediaFolder(".//Media");
		/**** Set up your scene here ****/

		IMesh* pCubeMesh = pGameEngine->LoadMesh("cube.x");
		IMesh* pActorMesh = pGameEngine->LoadMesh("sphere.x");

		CFloorTile* pFloorModels[g_MAP_COLS][g_MAP_ROWS];
		pPathfinding->CreateMapModels(pFloorModels, pCubeMesh);

		CLocationMarker* pStartMarker = new CLocationMarker(pCubeMesh, markerGreen, pPathfinding->GetStartX(), pPathfinding->GetStartY());
		CLocationMarker* pEndMarker = new CLocationMarker(pCubeMesh, markerRed, pPathfinding->GetEndX(), pPathfinding->GetEndY());

		CActor* pPathFollower = NULL;

		ICamera* pDebugCam = pGameEngine->CreateCamera(kFPS, 45.0f, 105.0f, 45.0f);
		pDebugCam->SetMovementSpeed(25.0f);
		pDebugCam->SetRotationSpeed(50.0f);
		pDebugCam->RotateX(90.0f);

		// The main game loop, repeat until engine is stopped
		while (pGameEngine->IsRunning())
		{
			// Draw the scene
			pGameEngine->DrawScene();
			g_FrameTime = pGameEngine->Timer();

			/**** Update your scene each frame here ****/
			if (pGameEngine->KeyHit(g_QUIT))
			{
				pGameEngine->Stop();
				loadNewMap = false;
			}
			if (pGameEngine->KeyHit(g_CONTINUE))
			{
				pGameEngine->Stop();
			}

			if (pGameEngine->KeyHit(g_NEXTPLY))
			{
				if (pPathfinding->GetPathState() == pathUnfinished)
				{
					pPathfinding->UnfoldNextPly();
					pPathfinding->ViewListChanges(pFloorModels);
					if (pPathfinding->GetPathState() != pathUnfinished)	//If after unfolding ply of search the path is complete (or impossible) save and output the result
					{
						pPathfinding->DisplayPath();
					}
					if (pPathfinding->GetPathState() == pathFinished)
					{
						pPathfinding->SavePath(outputFile, outFileStream);
						pPathFollower = new CActor(pActorMesh, inFileStream, outputFile, 3.0f);
					}
				}
			}
			if (pPathFollower != NULL)	//Once the pathfollower is created (path has been created)
			{
				//Follow the path - update the follower
			}

			
		}	//End of game Loop


		pGameEngine->RemoveCamera(pDebugCam);
		delete pStartMarker;
		delete pEndMarker;
		delete pPathFollower;

		for (int i = 0; i < g_MAP_COLS; i++)
		{
			for (int j = 0; j < g_MAP_ROWS; j++)
			{
				delete pFloorModels[i][j];
			}
		}

		pGameEngine->RemoveMesh(pCubeMesh);
		pGameEngine->Delete();

	}	//End of new map load loop
	
	delete pPathfinding;

	return 0;
}

void main()
{
	Main();

	_CrtDumpMemoryLeaks();

}
