// File:	Conversions.h
// Author:	Daniel Reynolds
// Last Updated:	29/03/14

//Definition of conversion algorithms and other general data manipulation tools

#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <string>
using namespace std;

float DegreesToRadians (float degrees);		//Converts degrees to radians

float RadiansToDegrees (float radians);		//Converts radians to degrees

string LCase(string text);					//Converts a string into Lower Case

string RemoveFileExtension(string text);	//Removes the file extension from the end of a string text

#endif