#include "CList.h"

COrderedList::COrderedList(CmpCoords iCompareFunction)
{
	//Constructor w/ pointer to the compare function for CCoords class
	mCompareFunction = iCompareFunction;
}

COrderedList::~COrderedList()
{
	//Destructor
}

bool COrderedList::Push(CCoords* pItem)
{
	//Push an item onto its proper place on the list based on the compare function
	//Keeps the list ordered without constant sorting
	if (pItem != NULL)
	{
		bool inserted = false;

		for (list<CCoords*>::iterator it = mList.begin(); it != mList.end() && !inserted; it++)
		{
			if (mCompareFunction(pItem, *it))	//If iItem comes first
			{
				//Insert the item before the compared-with item
				mList.insert(it, pItem);
				inserted = true;
			}
		}
		//If the element was not inserted by the above loop push it onto the back of the list as it must be the last item
		if (!inserted)
		{
			mList.push_back(pItem);
		}
		return true;	//Function has succeeded in pushing the new item
	}
	return false;	//Cannot push a null pointer onto the list
}


void COrderedList::Remove(CCoords* pItem)
{
	//Remove all instances of this item from the list (using the list remove function)
	mList.remove(pItem);
}

CCoords* COrderedList::PopFront()
{
	//Pop an item from the list and return a pointer to the popped item
	CCoords* pFront = mList.front();
	mList.pop_front();
	return pFront;
}

bool COrderedList::IsEmpty()
{
	//Returns whether or not the list is empty
	return mList.empty();
}
