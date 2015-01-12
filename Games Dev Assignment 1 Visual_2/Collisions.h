// File:	Collisions.h
// Author:	Daniel Reynolds
// Last Updated:	27/04/14

//Definition of Collsion classes and collision detection functions
	//Class for 2D sphere collision
	//Class for 2D Axis alligned bounding box collision

#ifndef COLLISIONS_H	//Header Guard
#define COLLISIONS_H

#include "Vector2.h"		//Include 2D Math Vector class
#include "Conversions.h"	//Include conversion algorithms

#include <TL-Engine.h>
using namespace tle;

/*--------------------------------------Class - CSphereCollision2--------------------------------------*/
const int kNoSphereMarkers = 10;

class CSphereCollision2	//Sphere collision object
{
private:
	Vector2 mCentre;						//Position of the collision object
	float mRadius;						//Radius of the collision sphere
	IMesh* mpMarkerMesh;					//Mesh for the collision marker
	IModel* mpMarkers[kNoSphereMarkers];	//Objects to show the boundaries of the collision area
	bool mMarkersExist;					//Says whether or not the collision markers exist
	float mHeight;						//The height at which the collision markers will be drawn

public:
	CSphereCollision2(IMesh* pCollisionMarker, Vector2 centreOfSphere, float radiusOfSphere = 0, float markerHeight = 1.0f);	/*Constructor -
	This object can be used as a point collision object if given a radius of 0*/

	~CSphereCollision2();

	void SetCentre(Vector2 newCentre);	//Sets a new centre position for the collision sphere
	Vector2 GetCentre();				//Returns the current centre position of the collision sphere

	void SetRadius(float newRadius);	//Sets a new radius for the collision sphere
	float GetRadius();					//Returns the radius of the collision sphere

	void ToggleMarkers();				//Toggles the existance of the collision markers
};

/************************************End of Class - CSphereCollision2**********************************/



bool SphereToSphereCollision2(CSphereCollision2* objectA, CSphereCollision2* objectB);	//Algorithm that returns true if the position of two SphereCollisions intersect

#endif