#ifndef FLOORTILE_H
#define FLOORTILE_H

#include <TL-Engine.h>
using namespace tle;

enum EFloorType { floorWall, floorOpen, floorWood, floorWater, floorSize };	//floor size is not a valid state but is used as a const value to create arrays compatible with the enum values as an index
//Used to create a list of file names for each of the floor types
class CFloorTile
{
private:
	IMesh* mMesh;
	IModel* mModel;
	EFloorType mType;

	static const float m_MODEL_WIDTH;
	int mX;
	int mY;

public:
	CFloorTile(IMesh* iMesh, EFloorType iType, int iX, int iY);
	~CFloorTile();

	void GetPosition(float &xPos, float &yPos, float &zPos);
	float GetWorldX();
	float GetWorldY();
	float GetWorldZ();

	static float GetWorldXAt(int xCoord);
	static float GetWorldZAt(int yCoord);
};

#endif