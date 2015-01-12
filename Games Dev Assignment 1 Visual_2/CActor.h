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

	SCoord(int iX, int iY)
	{
		mX = iX;
		mY = iY;
	}
	~SCoord()
	{
	}
};

class CActor
{
private:
	IMesh* mpMesh;
	IModel* mpModel;

	vector<SCoord*> mPath;
	unsigned int mPathPosition;
	bool mPathLoaded;

	CSphereCollision2* mpCollisionRadius;

	bool mIsMoving;

public:
	CActor(IMesh* piMesh, std::ifstream &fileStream, string pathFile, float modelHeight, IMesh* collisionMarkerMesh);
	~CActor();

	bool LoadPath(std::ifstream &fileStream, string pathFile);
	bool IsPathLoaded();

	CSphereCollision2* GetCollision();

	void Update(float frameTime);

};

#endif