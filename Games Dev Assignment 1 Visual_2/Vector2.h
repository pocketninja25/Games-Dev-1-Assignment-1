// File:	Vector2.h
// Author:	Daniel Reynolds
// Last Updated:	31/10/14

#ifndef VECTOR2_H	//Header Guard
#define VECTOR2_H

#include <math.h>
#include "Conversions.h"

//Two dimensional vector
/*--------------------------------------Class - Vector2--------------------------------------*/

class Vector2
{
private:
	float mXComp;					//X component of vector
	float mZComp;					//Z component of vector

public:
	Vector2(float xComponent = 0, float zComponent = 0);	//Constructor
	~Vector2();												//Destructor

	float GetX();					//Returns the X component of the vector
	float GetZ();					//Returns the Z component of the vector
	void SetX(float xComponent);	//Sets a new X component for the vector
	void SetZ(float zComponent);	//Sets a new Z component for the vector
	
};

/************************************End of Class - Vector2**********************************/


Vector2 CalculateDistanceVector2(Vector2 from, Vector2 to);		//Calculates the vector between two points

float CalculateVectorMagnitude2(Vector2 vect);					//Calculate the magnitude of a vector

Vector2 FindUnitVector2(Vector2 vector);						//Returns the vector of unit length that points in the same direction as the provided vector

float DotProduct(Vector2 vectorA, Vector2 vectorB);

bool IsInfront2(Vector2 facingVector, Vector2 distanceVector);	//Provide the facing vector of the 'looking' object and the distance vector between the 'looking' object and the 'looked for' object
																//Tells if the 'facing vector' object is in front of the 'distance vector' object

float AngleBetweenVectors2(Vector2 vectorA, Vector2 vectorB);	//Returns the angle between any two provided vectors



#endif