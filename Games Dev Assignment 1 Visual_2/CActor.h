#ifndef CACTOR_H
#define CACTOR_H

#include <TL-Engine.h>
#include <vector>
#include <fstream>
#include "FloorTile.h"
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
	int mPathPosition;
	bool mPathLoaded;

	bool mIsMoving;

public:
	CActor(IMesh* piMesh, std::ifstream &fileStream, string pathFile, float modelHeight);
	~CActor();

	bool LoadPath(std::ifstream &fileStream, string pathFile);
	bool IsPathLoaded();

	void Update(float frameTime);

};

#endif