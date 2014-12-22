#include "CList.h"

COrderedList::COrderedList(CmpCoords iCompareFunction)
{
	mCompareFunction = iCompareFunction;
}

COrderedList::~COrderedList()
{
	for (list<CCoords*>::iterator it = mList.begin(); it != mList.end(); it++)
	{
		CCoords* foo = *it;
		delete foo;
	}
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

CCoords* COrderedList::GetBack()
{
	return mList.back();
}

void COrderedList::PopBack()
{
	mList.pop_back();
}