// File:			Collisions.cpp
// Author:			Daniel Reynolds
// Last updated:	27/04/14

/*Bug List*/
/*
Severity:
Class A: Program crashes or locks-up.

Class B: Major graphic, function or logic error.

Class C: Minor graphic, function or logic error.

Class D: Playability or other concern.

*/

//Creation of the bodies of the collision detection functions and the functions belonging to the Collision object Classes

#include "Collisions.h"	//Include collision header file

const float kMarkerScale = 20.0f;	//The scale size of collision markers so they are adequately visible

/*--------------------------------------Class - CSphereCollision2--------------------------------------*/

CSphereCollision2::CSphereCollision2(IMesh* pCollisionMarker, Vector2 centreOfSphere, float radiusOfSphere, float markerHeight)	//Constructor
{
	//Set the position and radius to the values provided
	mCentre = centreOfSphere;
	mRadius = radiusOfSphere;

	mpMarkerMesh = pCollisionMarker;
	
	mHeight = markerHeight;

	//Create markers or not depending on the markersOn boolean
	
	mMarkersExist = false;
}

CSphereCollision2::~CSphereCollision2()	//Destructor
{
	if(mMarkersExist)	//If the markers exist they must be destroyed, toggle markers does this already (no need to rewrite functionality)
	{
		ToggleMarkers();
	}
}

void CSphereCollision2::SetCentre(Vector2 newCentre)	//Replaces the current centre position with the provided position
{
	//Calculate the required movement to re-centre the collision - use current centre and new centre to reposition
	float xDiff = newCentre.GetX() - mCentre.GetX();
	float zDiff = newCentre.GetZ() - mCentre.GetZ();

	//If the markers exist they need to be moved alongside the centre
	if(mMarkersExist)
	{
		for(int i = 0; i < kNoSphereMarkers; i++)
		{
			mpMarkers[i]->Move(xDiff, 0.0f, zDiff);
		}
	}
	mCentre = newCentre;	//Update stored centre variable
}

Vector2 CSphereCollision2::GetCentre()	//Returns the current Centre position
{
	return mCentre;
}

void CSphereCollision2::SetRadius(float newRadius)	//Sets a new radius for the collision object 
{
	//Set the new Radius
	mRadius = newRadius;

	//If collision markers exist, remove them then replace them so their positions reflect the new radius
	if(mMarkersExist)
	{
		ToggleMarkers();	//Toggle markers to off
		ToggleMarkers();	//Toggle markers back on
	}
}

float CSphereCollision2::GetRadius()	//Returns the object radius
{
	return mRadius;
}

void CSphereCollision2::ToggleMarkers()	//Toggles the existence of the collision markers
{
	if(mMarkersExist)
	{
		//Remove models from memory
		for(int i = 0; i < kNoSphereMarkers; i++)
		{
			mpMarkerMesh->RemoveModel(mpMarkers[i]);
		}
	}
	else
	{	
		//Calculate positions for models based on how many cubes in a circle and circle based maths
		float degreeOfMarker = 0.0f;
		for(int i = 0; i < kNoSphereMarkers; i++)
		{
			mpMarkers[i] = mpMarkerMesh->CreateModel(cosf(DegreesToRadians(degreeOfMarker)) * mRadius + mCentre.GetX(), 
				mHeight, 
				sinf(DegreesToRadians(degreeOfMarker)) * mRadius + mCentre.GetZ());	//Create marker

			degreeOfMarker+=(360.0f/kNoSphereMarkers);	//Increase angle for new marker placement
			mpMarkers[i]->Scale(kMarkerScale);			//Scale the markers to the correct scale so they are visible
		}
	}
	mMarkersExist = !mMarkersExist;	//Adjust existence flag
}

/************************************End of Class - CSphereCollision2**********************************/



bool SphereToSphereCollision2(CSphereCollision2* pObjectA, CSphereCollision2* pObjectB)	/*Tests to see if the bounding cylinders supplied intersect
																						A sphere object will be treated as a point if the radius = 0*/
{
	float distBetweenPoints = 0.0f;	
	Vector2 vect = CalculateDistanceVector2(pObjectA->GetCentre(), pObjectB->GetCentre());
	
	distBetweenPoints = ( vect.GetX() * vect.GetX() ) + ( vect.GetZ() * vect.GetZ() );	//Calculates the magnitude of the vector between the two models (squared)
	
	return (distBetweenPoints <= (pObjectA->GetRadius() + pObjectB->GetRadius()) * (pObjectA->GetRadius() + pObjectB->GetRadius()));	//If the bounding radii of the objects intersect
}