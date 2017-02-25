/**
*  
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Nikolay Denev
* @idnumber 45231
* @task 0
* @compiler VC
*
*/

#include "KMP.h"

#include <cstring>

KMP::KMP(const char * str) :
	currentPosition(0)
{
	length = strlen(str);
	strcpy(string, str);
	buildTable();
}

void KMP::startSearch()
{
	currentPosition = 0;
}

bool KMP::goNext(char ch)
{
	if (string[currentPosition] == ch)
	{
		if (++currentPosition == length)
		{
			currentPosition = 0;
			return true;
		}
	}
	else
	{
		while (currentPosition = table[currentPosition])
		{
			if (string[currentPosition] == ch)
			{
				++currentPosition;
				break;
			}
		}
	}

	return false;
}

void KMP::buildTable()
{
	table[0] = table[1] = 0;

	short index = 2;
	short curIndex = 0;

	while (index < length)
	{
		if (string[curIndex] == string[index - 1])
			table[index++] = ++curIndex;
		else if (curIndex)
			curIndex = table[curIndex];
		else
			table[index++] = 0;
	}
}
