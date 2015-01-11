#ifndef CLOCATIONMARKER_H
#define CLOCATIONMARKER_H

#include "AStar.h"
#include "FloorTile.h"
#include <TL-Engine.h>
using namespace tle;

enum EMarkerColour { markerGreen, markerRed, markerGrey, markerSize };

class CLocationMarker
{
private:
	IMesh* mpMesh;
	IModel* mpModel;

	EMarkerColour mColour;

public:
	CLocationMarker(IMesh* piMesh, EMarkerColour iColour, int xCoord, int yCoord);
	~CLocationMarker();

};

#endif