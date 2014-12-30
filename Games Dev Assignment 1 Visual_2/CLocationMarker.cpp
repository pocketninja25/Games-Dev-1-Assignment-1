#include "CLocationMarker.h"

const string g_MarkerSkinNames[markerSize] = { "green.jpg", "red.jpg", "grey.jpg" };

CLocationMarker::CLocationMarker(IMesh* iMesh, EMarkerColour iColour, int xCoord, int yCoord)
{
	mMesh = iMesh;
	mColour = iColour;

	float xWorldPos = CFloorTile::GetWorldXAt(xCoord);
	float yWorldPos = 0.0f;
	float zWorldPos = CFloorTile::GetWorldZAt(yCoord);
	mModel = mMesh->CreateModel(xWorldPos, yWorldPos, zWorldPos);
	mModel->SetSkin(g_MarkerSkinNames[mColour]);
	mModel->Scale(0.75f);
	mModel->ScaleY(0.05f);
}

CLocationMarker::~CLocationMarker()
{
	mMesh->RemoveModel(mModel);
}