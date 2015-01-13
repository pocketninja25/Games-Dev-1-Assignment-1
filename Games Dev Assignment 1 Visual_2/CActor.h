#ifndef CACTOR_H
#define CACTOR_H

#include <TL-Engine.h>
#include <vector>
#include <fstream>
#include "FloorTile.h"
#include "Collisions.h"
using namespace tle;

//This class is not complete - it needs to do movement

struct SCoord
{
	int mX;
	int mY;

	SCoord(int iX = 0, int iY = 0)
	{
		mX = iX;
		mY = iY;
	}

};

struct SPoint
{
	float mX;
	float mY;

	SPoint(float iX, float iY)
	{
		mX = iX;
		mY = iY;
	}
};

void QuarterSplines(SPoint point1, SPoint point2, SPoint point3, SPoint point4,
	SPoint &quarter, SPoint &half, SPoint &threeQuarter);

class CActor
{
private:
	IMesh* mpMesh;
	IModel* mpModel;

	vector<SCoord*> mPath;
	unsigned int mPathPosition;
	SPoint* mpSplinePoints[5];
	unsigned int mSplinePosition;
	CSphereCollision2* mpMovPointCollision;

	bool mPathLoaded;

	CSphereCollision2* mpModelCollision;

	bool mIsMoving;

	float mModelScale;

public:
	CActor(IMesh* piMesh, std::ifstream &fileStream, string pathFile, float modelHeight, IMesh* collisionMarkerMesh);
	~CActor();

	bool LoadPath(std::ifstream &fileStream, string pathFile);
	bool IsPathLoaded();

	CSphereCollision2* GetCollision();

	void Update(float frameTime);

};

#endif