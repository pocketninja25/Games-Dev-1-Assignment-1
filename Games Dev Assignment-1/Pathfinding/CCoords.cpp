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

int CCoords::GetScore()
{
	//Returns the 'score' of this node, it's combined cost and Manhattan distance
	return GetCost() + mManHatDist;
}

void CCoords::SetChild(CCoords* piChild)
{
	//Sets the provided child pointer to the child of this node
	mpChild = piChild;
}

//:::::::::::::::::::::::::::::Public::::::::::::::::::::::::::::://
CCoords::CCoords(int iX, int iY, ETraversalCost iIndividualCost, ENodeType iType)
{
	//Initialise all of the variables of the class
	mX = iX;
	mY = iY;
	mpParent = NULL;
	mpChild = NULL;
	mIndividualCost = iIndividualCost;
	mManHatDist = 0;		//Ideally the initial manhattan distance would be given by providing the end node - but this is not neccessarily known, instead set to 0 so it can be updated when the end is known
	mType = iType;
}

CCoords::~CCoords()
{
	//Destructor
}

int CCoords::GetCost()
{
	//Returns the travel cost from the start node to this node
	//This function is recursive along the parentage of this object
	//Adds this object's individual cost to the combined cost of it's parent (which works out its value with this same method)
	if (mpParent != NULL)
	{
		return (mpParent->GetCost() + mIndividualCost);
	}
	else	//Termination condition - object is root, has no parent, it's total cost is just it's individual cost
	{
		return mIndividualCost;
	}

}

int CCoords::GetIndividualCost()
{
	//Returns the individual travel cost of this node
	return mIndividualCost;
}

int CCoords::GetX()
{
	//Returns the x coordinate of this node
	return mX;
}

int CCoords::GetY()
{
	//Returns the y coordinate of this node
	return mY;
}

CCoords* CCoords::GetParent()
{
	//Returns the pointer to this node's parent node
	return mpParent;
}

CCoords* CCoords::GetChild()
{
	//Returns the pointer to this node's child node
	return mpChild;
}

ENodeType CCoords::GetType()
{
	//Returns the type of this node
	return mType;
}

void CCoords::GeneratePath()
{
	//Generates the path of this node by filling in its ancestors child pointers
	CCoords* pCurrent = this;	//Start current off as the object this function was called from
	while (pCurrent->GetParent() != NULL)
	{
		//Update the child pointer of 'currents' parent
		pCurrent->GetParent()->SetChild(pCurrent);
		//Make current it's own parent so it's parent's child can be updated
		pCurrent = pCurrent->GetParent();
	}

}

bool CCoords::TrySetParent(CCoords* pNewParent)
{
	//Attempts to set the parent, if the new parent's travel cost is greater than the current parent's travel cost, returns false and does not change parent
	if (pNewParent != NULL && pNewParent != this)	//Check that the potential new parent is not null and is not pointing to this object (circular)
	{
		if (mpParent == NULL)
		{
			//The old parent was null, replace it and return true (success)
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
	
	//If function has not returned by now then it has failed to assign parent, return false to indicate this
	return false;
}

bool CCoords::TrySetManhatDist(CCoords* pEndNode)
{
	//Attempts to set the manhattan distance from the provided node to this node, fails if the provided node is not type 'end node'
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
	//Sets the type of the node to the provided type
	mType = iType;
}

bool CCoords::Compare(CCoords* p1, CCoords* p2)
{
	//Compares two instances of this class and returns true if p1 is 'before' p2 (Is before if has a lower score)
	return (p1->GetScore() < p2->GetScore());
}