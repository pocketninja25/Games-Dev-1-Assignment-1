#include "FloorTile.h"

const std::string g_TileSkinNames[floorSize][listSize] = { 
	{ "tiles1 - open.jpg",	"tiles1 - closed.jpg",	"tiles1.jpg",	"tiles1 - path.jpg"	},
	{ "stone2 - open.jpg",	"stone2 - closed.jpg",	"stone2.jpg",	"stone2 - path.jpg"	},
	{ "wood1 - open.jpg",	"wood1 - closed.jpg",	"wood1.jpg"	,	"wood1 - path.jpg"	},
	{ "CueTip - open.jpg",	"CueTip - closed.jpg",	"CueTip.jpg",	"CueTip - path.jpg"	} };

const float CFloorTile::m_MODEL_WIDTH = 10.0f;

CFloorTile::CFloorTile(tle::IMesh* piMesh, EFloorType iType, int iX, int iY)
{
	mpMesh = piMesh;
	mType = iType;
	mListState = listNone;
	mX = iX;
	mY = iY;

	float xPos = 0.0f;					//Centre the model on 0.0f - (assuming model is at (0, 0))
	float yPos = -m_MODEL_WIDTH / 2;	//Place the floor so the model's top is level with 0.0f
	float zPos = 0.0f;					//Centre the model on 0.0f - (assuming model is at (0, 0))

	if (mType == floorWall)	//Type is a wall, not a floor, must be raised - position so bottom of model is at 0.0f
	{
		yPos += m_MODEL_WIDTH;
	}

	xPos += mX * m_MODEL_WIDTH;			//Offset the x position by the x coordinate of the tile
	zPos += mY * m_MODEL_WIDTH;			//Offset the z position by the y coordinate of the tile (tile coordinate Y is global coordinate Z)

	mpModel = mpMesh->CreateModel(xPos, yPos, zPos);
	mpModel->SetSkin(g_TileSkinNames[mType][mListState]);
	mpModel->Scale(0.9f);
}

CFloorTile::~CFloorTile()
{
	mpMesh->RemoveModel(mpModel);
}


void CFloorTile::GetPosition(float &xPos, float &yPos, float &zPos)
{
	xPos = mpModel->GetX();
	yPos = mpModel->GetY();
	zPos = mpModel->GetZ();
}

float CFloorTile::GetWorldX()
{
	return mpModel->GetX();
}

float CFloorTile::GetWorldY()
{
	return mpModel->GetY();
}

float CFloorTile::GetWorldZ()
{
	return mpModel->GetZ();
}

float CFloorTile::GetWorldXAt(int xCoord)
{
											//Centre the position on 0.0f
	return xCoord * m_MODEL_WIDTH;			//Offset the x position by the x coordinate of the tile
}

float CFloorTile::GetWorldZAt(int yCoord)
{
											//Centre the position on 0.0f
	return yCoord * m_MODEL_WIDTH;			//Offset the y position by the y coordinate of the tile
}

void CFloorTile::SetListState(EListState newState)
{
	mListState = newState;
	mpModel->SetSkin(g_TileSkinNames[mType][mListState]);

}
