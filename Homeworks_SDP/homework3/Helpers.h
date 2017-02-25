/**
*  
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Nikolay Denev
* @idnumber 45231
* @task 3
* @compiler VC
*
*/

#pragma once

#include <iostream>

#include "Trie.h"
#include "CircularBuffer.h"

inline bool isWhiteSpace(char ch)
{
	return ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r';
}

inline bool isLittleLetter(char ch)
{
	return ch >= 'a' && ch <= 'z';
}

inline bool isBigLetter(char ch)
{
	return ch >= 'A' && ch <= 'Z';
}

inline bool isLetter(char ch)
{
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

///
///reads each phrase from the stream and insert it in the tree
///
void uploadPhrases(std::ifstream& in, Trie& tree, long long& maxLengthPhrase);

///
///allocates memory for the buffer
///
void initBuffer(std::ifstream& in, CircularBuffer& buffer);

///
///if there is a phrase starting from the current position in the buffer
///(it gets the larger one) the current position
///becomes the end of that phrase phrase's cost is
///add to the totalCost and the number of words in the phrase are add to
///the totalWordsCount and the function returns true
///else return false
///
bool readPhrase(Trie& tree, CircularBuffer& buffer,
	long long& totalCost, long long& totalWordsCount);

///
///return the factor of the stream
///
///if there is nothing in the stream it returns 0
///
double getFileFactor(std::ifstream& in, Trie& tree, long long maxPhraseLength);