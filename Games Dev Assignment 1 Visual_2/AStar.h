#ifndef ASTAR_H
#define ASTAR_H

#include "CList.h"
#include "CCoords.h"
#include <fstream>
#include "FloorTile.h"
#include "CLocationMarker.h"
#include <TL-Engine.h>
using namespace std;

const int g_MAP_COLS = 10;							//Number of Columns (X Coordinates of the map)
const int g_MAP_ROWS = 10;							//Number of Rows (Y Coordinates of the map)

class CAStar
{
private:
	COrderedList* mpOpenList;
	list<CCoords*> mClosedList;

	CCoords* mpGrid[g_MAP_COLS][g_MAP_ROWS];
	CCoords* mpStartNode;
	CCoords* mpEndNode;

	bool mMapLoaded;
	bool mCoordsLoaded;
	bool mPathFound;

	void ClearMap();
	void GetSuccessors(CCoords* pCurrent, CCoords* pSuccessors[4]);

public:
	CAStar();
	~CAStar();

	//Loading functions
	bool LoadMapAndCoords(string iMapFile, string iCoordsFile, std::ifstream &fileStream);

	//Algorithm
	bool FindPath();

	//Output functions
	bool DisplayMap();
	bool CreateMapModels(CFloorTile* models[g_MAP_COLS][g_MAP_ROWS], IMesh* tileMesh);
	void DisplayPath();
	bool SavePath(string fileName, std::ofstream &fileStream);

	//Get functions
	bool MapLoaded();
	bool CoordsLoaded();
	bool PathFound();
	int GetStartX();
	int GetStartY();
	int GetEndX();
	int GetEndY();
};


#endif