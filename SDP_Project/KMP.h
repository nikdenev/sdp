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

#pragma once

#include "HuffmanTree.h"

class KMP
{
public:
	///
	///creates the table for the str string 
	///so that the kmp algorithm can be performed 
	///
	KMP(const char* str);

	///
	///sets the position of the place where goNext() 
	///should search for match to 0
	///
	void startSearch();

	///
	///if with the next character ch you have found the string
	///it returns true
	///else 
	///if the next character from the string matches with ch
	///the current position of the string is incremented
	///else
	///the current position of the string is set to 
	///the largest prefix which is postfix of the word starting from
	///the beginning of the string to the current postion
	///
	bool goNext(char ch);

private:
	void buildTable();

private:
	char string[SEARCHED_STRING_MAX_LENGTH];
	short table[SEARCHED_STRING_MAX_LENGTH];
	short length;
	short currentPosition;
};

