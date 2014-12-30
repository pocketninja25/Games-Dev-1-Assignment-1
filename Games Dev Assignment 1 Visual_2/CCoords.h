#ifndef CCOORDS_H
#define CCOORDS_H

#include <math.h>
#include <limits>
#include <iostream>
using namespace std;

enum ENodeType {nodeStart, nodeEnd, nodeStandard};
enum ETraversalCost {costWall, costOpen, costWood, costWater};

class CCoords
{
private:
	int mX;							//X coordinate of the node
	int mY;							//Y coordinate of the node
	CCoords* mpParent;				//Pointer to the node's 'parent' - where it came from
	CCoords* mpChild;				//Pointer to the node's 'child' - where it is going
	ETraversalCost mIndividualCost;	//The individual travel cost of the node
	int mManHatDist;				//The manhattan distance (heuristic) to the end node
	ENodeType mType;				//Whether the node is a start, end or standard node

	//Assumed values, can be stored but will be calculated instead
	//int mCost;			//The travel cost from the current fastest route to this node - implied storage - can be calculated based upon the parent's cumulative cost
	//int mScore;			//The combined manhattan distance and travel cost of the node - implied storage - can be calculated

	int CalcManhatDist(CCoords* pDestinationNode);	//Calculates the manhattan distance from this node to the provided end node
	int GetScore();									//Returns the 'score' of this node, it's combined cost and Manhattan distance
	void SetChild(CCoords* piChild);				//Sets the provided child pointer to the child of this node

public:
	CCoords(int iX = 0, int iY = 0, ETraversalCost iIndividualCost = costWall, ENodeType iType = nodeStandard);
	~CCoords();

	int GetCost();				//Returns the travel cost from the start node to this node
	int GetIndividualCost();	//Returns the individual travel cost of this node
	int GetX();					//Returns the x coordinate of this node
	int GetY();					//Returns the y coordinate of this node
	CCoords* GetParent();		//Returns the pointer to this node's parent node
	CCoords* GetChild();		//Returns the pointer to this node's child node
	ENodeType GetType();		//Returns the type of this node

	void GeneratePath();						//Generates the path of this node by filling in its ancestors child pointers
	bool TrySetParent(CCoords* pNewParent);		//Attempts to set the parent, if the new parent's travel cost is greater than the current parent's travel cost, returns false and does not change parent
	bool TrySetManhatDist(CCoords* pEndNode);	//Attempts to set the manhattan distance from the provided node to this node, fails if the provided node is not type 'end node'
	void SetType(ENodeType iType);				//Sets the type of the node to the provided type

	bool static Compare(CCoords* p1, CCoords* p2);	//Compares two instances of this class and returns true if p1 is 'before' p2
};

#endif