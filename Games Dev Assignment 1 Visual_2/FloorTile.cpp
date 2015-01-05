#include "FloorTile.h"

const std::string g_TileSkinNames[floorSize] = { "tiles1.jpg", "stone2.jpg", "wood1.jpg", "CueTip.jpg" };

const float CFloorTile::m_MODEL_WIDTH = 10.0f;

CFloorTile::CFloorTile(tle::IMesh* iMesh, EFloorType iType, int iX, int iY)
{
	mMesh = iMesh;
	mType = iType;
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

	mModel = mMesh->CreateModel(xPos, yPos, zPos);
	mModel->SetSkin(g_TileSkinNames[mType]);
	mModel->Scale(0.9f);
}

CFloorTile::~CFloorTile()
{
	mMesh->RemoveModel(mModel);
}


void CFloorTile::GetPosition(float &xPos, float &yPos, float &zPos)
{
	xPos = mModel->GetX();
	yPos = mModel->GetY();
	zPos = mModel->GetZ();
}

float CFloorTile::GetWorldX()
{
	return mModel->GetX();
}

float CFloorTile::GetWorldY()
{
	return mModel->GetY();
}

float CFloorTile::GetWorldZ()
{
	return mModel->GetZ();
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