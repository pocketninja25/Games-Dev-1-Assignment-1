//Includes and defines for memory leak detection
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>

#include "CList.h"

int main()
{
	COrderedList* openList = new COrderedList(CCoords::Compare);
	list<CCoords*> closedList;

	CCoords* foo1 = new CCoords(0, 5, 1, 10);
	CCoords* foo2 = new CCoords(0, 4, 2, 20);
	CCoords* foo3 = new CCoords(0, 3, 3, 30);
	CCoords* foo4 = new CCoords(0, 2, 4, 40);
	CCoords* foo5 = new CCoords(0, 1, 5, 50);

	openList->Push(foo3);
	openList->Push(foo2);
	openList->Push(foo4);
	openList->Push(foo5);
	openList->Push(foo1);

	for (int i = 0; i < 5; i++)
	{
		cout << openList->GetBack()->GetCost() << endl;
		openList->PopBack();

	}

	system("pause");	
	_CrtDumpMemoryLeaks();
}