#include "CCoords.h"


CCoords::CCoords(CCoords* iParent, int iX, int iY, int iCost)
{
	mX = iX;
	mY = iY;
	mParent = iParent;
	mCost = iCost;
}

CCoords::~CCoords()
{
	if (mParent != 0)
	{
		delete mParent;
	}
}

int CCoords::GetCost()
{
	return mCost;
}

bool CCoords::Compare(CCoords* ptr1, CCoords* ptr2)
{
	return (ptr1->mCost > ptr2->mCost);
}