#include "CCoords.h"


CCoords::CCoords(CCoords* piParent, int iX, int iY, int iCost)
{
	mX = iX;
	mY = iY;
	mpParent = piParent;
	mCost = iCost;
}

CCoords::~CCoords()
{
	if (mpParent != 0)
	{
		delete mpParent;
	}
}

int CCoords::GetCost()
{
	return mCost;
}

bool CCoords::Compare(CCoords* p1, CCoords* p2)
{
	return (p1->mCost > p2->mCost);
}