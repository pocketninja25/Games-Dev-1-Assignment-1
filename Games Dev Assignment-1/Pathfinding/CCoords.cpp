#include "CCoords.h"

//::::::::::::::::::::::::::::Private::::::::::::::::::::::::::::://

int CCoords::CalcManhatDist(CCoords* pDestinationNode)
{
	//Returns the calculated manhattan distance of the current node to the provided destination node 
	int distance = 0;
	distance += abs(GetX() - pDestinationNode->GetX());
	distance += abs(GetY() - pDestinationNode->GetY());
	return distance;
}

//:::::::::::::::::::::::::::::Public::::::::::::::::::::::::::::://

CCoords::CCoords(int iX, int iY, int iIndividualCost, ENodeType iType)
{
	mX = iX;
	mY = iY;
	mpParent = NULL;
	mIndividualCost = iIndividualCost;
	mManHatDist = 0;		//Ideally the initial manhattan distance would be given by providing the end node - but this is not neccessarily known, instead set to 0 so it can be updated when the end is known
	mType = iType;
}

CCoords::~CCoords()
{
	/*if (mpParent != 0)
	{
		delete mpParent;
	}*/
}

int CCoords::GetCost()
{
	if (mpParent != NULL)
	{
		return (mpParent->GetCost() + mIndividualCost);
	}
	else
	{
		return mIndividualCost;
	}

}

int CCoords::GetManHatDist()
{
	return mManHatDist;
}


int CCoords::GetX()
{
	return mX;
}

int CCoords::GetY()
{
	return mY;
}

ENodeType CCoords::GetType()
{
	return mType;
}

int CCoords::GetScore()
{
	return GetCost() + GetManHatDist();
}

CCoords* CCoords::GetParent()
{
	return mpParent;
}

bool CCoords::TrySetParent(CCoords* pNewParent)
{
	if (pNewParent != NULL && pNewParent != this)	//Check that the potential new parent is not null and is not pointing to this object (circular)
	{
		if (mpParent == NULL)
		{
			mpParent = pNewParent;
			return true;
		}
		else //Current parent is not null - need to compare
		{
			if (mpParent->GetCost() > pNewParent->GetCost())
			{
				//New parent has a lower cost than current parent - replace it and return true (success)
				mpParent = pNewParent;
				return true;
			}
		}
	}
	
	
	return false;
}

bool CCoords::TrySetManhatDist(CCoords* pEndNode)
{
	if (pEndNode != NULL)
	{
		if (pEndNode->GetType() == nodeEnd)
		{
			mManHatDist = CalcManhatDist(pEndNode);
			return true;
		}
	}
	return false;
}

void CCoords::SetType(ENodeType iType)
{
	mType = iType;
}

bool CCoords::Compare(CCoords* p1, CCoords* p2)
{
	return (p1->GetScore() < p2->GetScore());
}