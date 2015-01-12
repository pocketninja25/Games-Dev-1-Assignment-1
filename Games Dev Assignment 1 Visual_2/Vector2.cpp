// File:	Vector2.cpp
// Author:	Daniel Reynolds
// Last Updated:	31/10/14

#include "Vector2.h"	//Include Vector2 header file

/*--------------------------------------Class - Vector2--------------------------------------*/

Vector2::Vector2(float xComponent, float zComponent)	//Constructor
{
	mXComp = xComponent;
	mZComp = zComponent;
}

Vector2::~Vector2()		//Destructor
{
}

float Vector2::GetX()					//Returns the X component of the vector
{
	return mXComp;
}

float Vector2::GetZ()					//Returns the Z component of the vector
{
	return mZComp;
}

void Vector2::SetX(float xComponent)	//Sets the X component of the vector
{
	mXComp = xComponent;
}

void Vector2::SetZ(float zComponent)	//Sets the Z component of the vector
{
	mZComp = zComponent;
}

/************************************End of Class - Vector2**********************************/


Vector2 CalculateDistanceVector2(Vector2 from, Vector2 to)	//Calculates and returns the vector from point 'from' to point 'to'
{
	//Subtract the position of 'from' from the position of 'to'
	return Vector2(to.GetX() - from.GetX(), to.GetZ() - from.GetZ());	//return vector
}

float CalculateVectorMagnitude2(Vector2 vect)				//Calculate the magnitude of the vector provided
{
	return sqrt(vect.GetX() * vect.GetX() + vect.GetZ() * vect.GetZ());	//Square root of the sum of the squares of the components of the vector
}

Vector2 FindUnitVector2(Vector2 vector)						//Returns the vector of unit length that points in the same direction as the provided vector
{
	float magnitude = CalculateVectorMagnitude2(vector);

	return Vector2(vector.GetX()/magnitude, vector.GetZ()/magnitude);

}

float DotProduct(Vector2 vectorA, Vector2 vectorB)
{
	return (vectorA.GetX() * vectorB.GetX()) + (vectorA.GetZ() * vectorB.GetZ());
}

bool IsInfront2(Vector2 facingVector, Vector2 distanceVector) //Provide the facing vector of the 'looking' object and the distance vector between the 'looking' object and the 'looked for' object
{
	float dotProd = DotProduct(facingVector, distanceVector);

	if (dotProd > 0)		//V dotted with W > 0 if angle between is < 90, this means the 'looked for' object is in front of the 'looking' object
	{
		return true;

	}

	return false;
}

float AngleBetweenVectors2(Vector2 vectorA, Vector2 vectorB)	//Returns an angle in degrees
{
	float acosAngle = DotProduct(vectorA, vectorB) / (CalculateVectorMagnitude2(vectorA) * CalculateVectorMagnitude2(vectorB));

	//Occasionally the value of acosAngle will return a value out of the range for this number, (-1 >= acosAngle <= 1)
	//This is merely a rounding error, to avoid this I have used this range check to reset the value within the range
	//If this code were not present it could cause a lot of error, the result of this function being NaN (not a number)
	if(acosAngle > 1.0f)
	{
		acosAngle = 1.0f;
	}
	else if(acosAngle < -1.0f)
	{
		acosAngle = -1.0f;
	}

	//If the magnitude of one of the vectors is 0. the function does not return a valid value, it returns IND (is not defined) or infinity
	//Mathematically this means that dotProd will also be 0. 0/0 is 1 so this will be the returned value, the following if statement
	//Is used to find if IND is returned
	if(acosAngle != acosAngle)
	{
		acosAngle = 1.0f;
	}


	return RadiansToDegrees(acosf(acosAngle));


}
