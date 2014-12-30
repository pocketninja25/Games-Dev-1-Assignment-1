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
	IMesh* mMesh;
	IModel* mModel;

	EMarkerColour mColour;

public:
	CLocationMarker(IMesh* iMesh, EMarkerColour iColour, int xCoord, int yCoord);
	~CLocationMarker();

};

#endif