#include "CActor.h"

void QuarterSplines(SPoint point1, SPoint point2, SPoint point3, SPoint point4,
	SPoint &quarter, SPoint &half, SPoint &threeQuarter)
{

	// calculate a quarter of the way along the set of points
	quarter.mX = point1.mX * -0.0703125f + point2.mX * 0.8671875f +
		point3.mX * 0.2265625f + point4.mX * -0.0234375f;
	quarter.mY = point1.mY * -0.0703125f + point2.mY * 0.8671875f +
		point3.mY * 0.2265625f + point4.mY * -0.0234375f;

	// calculate a half of the way along the set of points
	half.mX = point1.mX * -0.0625f + point2.mX * 0.5625f +
		point3.mX * 0.5625f + point4.mX * -0.0625f;
	half.mY = point1.mY * -0.0625f + point2.mY * 0.5625f +
		point3.mY * 0.5625f + point4.mY * -0.0625f;

	// calculate three-quarters of the way along the set of points
	threeQuarter.mX = point1.mX * -0.0234375f + point2.mX * 0.2265625f +
		point3.mX * 0.8671875f + point4.mX * -0.0703125f;
	threeQuarter.mY = point1.mY * -0.0234375f + point2.mY * 0.2265625f +
		point3.mY * 0.8671875f + point4.mY * -0.0703125f;
}

CActor::CActor(IMesh* piMesh, std::ifstream &fileStream, string pathFile, float modelHeight, IMesh* pCollisionMarkerMesh)
{
	mpMesh = piMesh;

	LoadPath(fileStream, pathFile);	//Initialises path and pathLoaded - also sets path position to the start of the list

	//Positions of the starting node of the path
	int startX = mPath[mPathPosition]->mX;
	int startY = mPath[mPathPosition]->mY;

	mpModel = mpMesh->CreateModel(CFloorTile::GetWorldXAt(startX), modelHeight, CFloorTile::GetWorldZAt(startY));	//Create model at start location

	mModelScale = 0.35f;
	mpModel->Scale(mModelScale);

	mpModelCollision = new CSphereCollision2(pCollisionMarkerMesh, Vector2(mpModel->GetX(), mpModel->GetZ()), 0.2f, 2.5f);
	mpModelCollision = new CSphereCollision2(pCollisionMarkerMesh, Vector2(mpModel->GetX(), mpModel->GetZ()), 0.0f, 1.0f);
	
	for (int i = 0; i < 5; i++)
	{
		mpSplinePoints[i] = NULL;
	}

	mIsMoving = false;
	mSplinePosition = 0;
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
	delete mpModelCollision;

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
	return mpModelCollision;
}

void CActor::Update(float frameTime)
{
	//if (mPathPosition < mPath.size())
	//{
	//	//Calculate the next spline
	//	if (!mIsMoving)	//Run as long as mPathPosition is not equal to the size of the vector (would be indexing within the vector) - mPathPosition is incremented when actor reaches its destination
	//	{
	//		for (int i = 0; i < 5; i++)
	//		{
	//			if (mpSplinePoints[i] != NULL)
	//			{
	//				delete mpSplinePoints[i];
	//				mpSplinePoints[i] = NULL;
	//			}
	//		}

	//		SCoord* pCoords[4];

	//		if (mPathPosition - 1 >= 0)
	//		{
	//			pCoords[0] = mPath[mPathPosition - 1];
	//		}
	//		else
	//		{
	//			pCoords[0] = mPath[mPathPosition];
	//		}
	//		pCoords[1] = mPath[mPathPosition];
	//		pCoords[2] = mPath[mPathPosition + 1];
	//		if (mPathPosition + 2 <= mPath.size())
	//		{
	//			pCoords[3] = mPath[mPathPosition + 2];
	//		}
	//		else
	//		{
	//			pCoords[3] = mPath[mPathPosition + 1];
	//		}

	//		SPoint* pPoints[4];
	//		for (int i = 0; i < 4; i++)
	//		{
	//			float x = CFloorTile::GetWorldXAt(pCoords[i]->mX);
	//			float y = CFloorTile::GetWorldZAt(pCoords[i]->mY);
	//			pPoints[i] = new SPoint(x, y);
	//		}

	//		QuarterSplines((*pPoints[0]), (*pPoints[1]), (*pPoints[2]), (*pPoints[3]),
	//			(*mpSplinePoints[2]), (*mpSplinePoints[3]), (*mpSplinePoints[4]));

	//		mpSplinePoints[0] = new SPoint((*pPoints[1]));
	//		mpSplinePoints[4] = new SPoint((*pPoints[2]));

	//		for (int i = 0; i < 4; i++)
	//		{
	//			delete pPoints[i];
	//		}

	//		mpMovPointCollision->SetCentre(Vector2(mpSplinePoints[0]->mX, mpSplinePoints[0]->mY));
	//	}
	//	else
	//	{
	//		if (SphereToSphereCollision2(mpModelCollision, mpMovPointCollision))
	//		{
	//			mSplinePosition++;
	//			if (mSplinePosition < 5)
	//			{
	//				mpMovPointCollision->SetCentre(Vector2(mpSplinePoints[mSplinePosition]->mX, mpSplinePoints[mSplinePosition]->mY));
	//				//Look at the position - use collision to create a look at model then look at it and delete the lookat model
	//			}
	//			else
	//			{
	//				mIsMoving = false;
	//			}
	//		}
	//		mpModel->MoveZ(1.0f * frameTime);
	//	}
	//}

	


}