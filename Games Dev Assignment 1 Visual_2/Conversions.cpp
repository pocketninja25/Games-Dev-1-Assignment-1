// File:			Conversions.cpp
// Author:			Daniel Reynolds
// Last updated:	29/03/14

#include "Conversions.h"	//Include conversion header file

const float kPi = 3.141592654;	//Declare pi as a constant

float DegreesToRadians (float degrees)	//Converts degrees to radians
{
	return degrees * (kPi / 180);
}

float RadiansToDegrees (float radians)	//Converts radians to degrees
{
	return radians * (180 / kPi );
}

string LCase(string text)	//Converts a string into Lower Case
{
	//Count through the string and convert each character to lower case
	for (unsigned int i = 0; i < text.length(); i++)
	{
		text[i] = tolower(text[i]);
	}
	return text;	//Return converted string
}

string RemoveFileExtension(string text)	//Removes the file extension from the end of a string text
{
	bool dotFound = false;
	int dotPos = text.length();
	for(int i = text.length() - 1; i >= 0 && !dotFound; i--)	//Count from the back of the string until a '.' is found or the front of the string is reached
	{
		if(text[i] == '.')		//If the current character is a '.'
		{
			dotFound = true;	//Leave the loop
			dotPos = i;			//Set the position of the dot to the current position
		}
	}
	text.resize(dotPos);		//Resize the string so that the dot and each subsequent character are removed
	return text;				//Return the string
}