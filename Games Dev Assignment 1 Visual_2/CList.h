#ifndef LISTS_H
#define LISTS_H

#include "CCoords.h"
#include <list>
using namespace std;

typedef bool (*CmpCoords)(CCoords* ptr1, CCoords* ptr2);

class COrderedList
{
	/*COrderedList class uses the STL container 'list' but limits access to it to basic requirements. It also allows any object pushed onto the 
	list to be 'automatically' inserted into the correct location based on the compare function ensuring the data is always properly sorted*/

private:
	list<CCoords*> mList;		//The underlying list used to hold the coordinate data
	CmpCoords mCompareFunction;	//A pointer to the function used to compare objects of the CCoords class - done this way to allow for changes in the method of comparison

public:
	COrderedList(CmpCoords iCompareFunction);	//Constructor w/ pointer to the compare function for CCoords class
	~COrderedList();							//Destructor
	
	bool Push(CCoords* pItem);		//Push an item onto its proper place on the list based on the compare function
	void Remove(CCoords* pItem);	//Remove all instances of this item from the list (using the list remove function)
	void Clear();					//Remove all items from the list

	CCoords* PopFront();			//Pop an item from the list and return a pointer to the popped item

	bool IsEmpty();					//Returns whether or not the list is empty
};

#endif