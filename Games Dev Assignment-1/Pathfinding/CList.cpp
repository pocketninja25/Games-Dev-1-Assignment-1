#include "CList.h"

COrderedList::COrderedList(CmpCoords iCompareFunction)
{
	mCompareFunction = iCompareFunction;
}

COrderedList::~COrderedList()
{
	
}

void COrderedList::Push(CCoords* pItem)
{
	bool inserted = false;

	for (list<CCoords*>::iterator it = mList.begin(); it != mList.end() && !inserted; it++)
	{
		
		if (mCompareFunction(pItem, *it))	//If iItem comes first
		{
			mList.insert(it, pItem);
			inserted = true;
		}
	}
	if (!inserted)	//If the element was not inserted by the above loop push it onto the back of the list as it must be the last item
	{
		mList.push_back(pItem);
	}
}

void COrderedList::Remove(CCoords* pItem)
{
	mList.remove(pItem);
}

CCoords* COrderedList::PopFront()
{
	CCoords* front = mList.front();
	mList.pop_front();
	return front;
}

bool COrderedList::Empty()
{
	return mList.empty();
}
