#ifndef CACTOR_H
#define CACTOR_H

#include <TL-Engine.h>
using namespace tle;

class CActor
{
private:
	IMesh* mMesh;
	IModel* mModel;

public:
	CActor(IMesh* iMesh);
	~CActor();

};

CActor::CActor(IMesh* iMesh, string pathFile)
{
	mMesh = iMesh;
	mModel = mMesh->CreateModel()
}

CActor::~CActor()
{
}

#endif