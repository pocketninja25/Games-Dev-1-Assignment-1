#include "CLocationMarker.h"

const string g_MarkerSkinNames[markerSize] = { "green.jpg", "red.jpg", "grey.jpg" };

CLocationMarker::CLocationMarker(IMesh* piMesh, EMarkerColour iColour, int xCoord, int yCoord)
{
	mpMesh = piMesh;
	mColour = iColour;

	float xWorldPos = CFloorTile::GetWorldXAt(xCoord);
	float yWorldPos = 0.0f;
	float zWorldPos = CFloorTile::GetWorldZAt(yCoord);
	mpModel = mpMesh->CreateModel(xWorldPos, yWorldPos, zWorldPos);
	mpModel->SetSkin(g_MarkerSkinNames[mColour]);
	mpModel->Scale(0.75f);
	mpModel->ScaleY(0.05f);
}

CLocationMarker::~CLocationMarker()
{
	mpMesh->RemoveModel(mpModel);
}