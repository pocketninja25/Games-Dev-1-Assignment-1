#include "CActor.h"

CActor::CActor(IMesh* piMesh, std::ifstream &fileStream, string pathFile, float modelHeight, IMesh* pCollisionMarkerMesh)
{
	mpMesh = piMesh;

	LoadPath(fileStream, pathFile);	//Initialises path and pathLoaded - also sets path position to the start of the list

	//Positions of the starting node of the path
	int startX = mPath[mPathPosition]->mX;
	int startY = mPath[mPathPosition]->mY;

	mpModel = mpMesh->CreateModel(CFloorTile::GetWorldXAt(startX), modelHeight, CFloorTile::GetWorldZAt(startY));	//Create model at start location

	mpModel->Scale(0.35f);

	mpCollisionRadius = new CSphereCollision2(pCollisionMarkerMesh, Vector2(mpModel->GetX(), mpModel->GetZ()), 0.2f, 2.5f);

	mIsMoving = false;
}

CActor::~CActor()
{
	mpMesh->RemoveModel(mpModel);
	//Delete the path SCoord objects in the vector
	for (vector<SCoord*>::iterator it = mPath.begin(); it != mPath.end(); it++)
	{
		delete (*it);
	}
	//Delete the collision radius
	delete mpCollisionRadius;

}

bool CActor::LoadPath(std::ifstream &fileStream, string pathFile)
{
	if (mPathLoaded)
	{
		for (vector<SCoord*>::iterator it = mPath.begin(); it != mPath.end(); it++)
		{
			delete (*it);
		}

		mPath.clear();
	}


	fileStream.close();
	fileStream.clear();
	fileStream.open(pathFile);

	if (!fileStream)	//If there is a problem with the filestream (failed to load file etc)
	{
		mPathLoaded = false;
		return false;
	}

	char inChX = ' ';
	char inChY = ' ';
	int convertedX = NULL;
	int convertedY = NULL;

	//Assumes that if it can find a number, it is part of a pair
	while (fileStream && fileStream >> inChX)	//Read from the file - will prevent the end of file marker being read as a number
	{
		convertedX = inChX - '0';	//Convert the character read to an integer

		fileStream >> inChY;
		convertedY = inChY - '0';

		mPath.push_back(new SCoord(convertedX, convertedY));

	}
	mPathPosition = 0;
	mPathLoaded = true;
	return true;

}

CSphereCollision2* CActor::GetCollision()
{
	return mpCollisionRadius;
}

void CActor::Update(float frameTime)
{
	if (mPathPosition < mPath.size())	//Run as long as mPathPosition is not equal to the size of the vector (would be indexing within the vector) - mPathPosition is incremented when actor reaches its destination
	{
		//if ()

	}

}