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

#include "Helpers.h"

#include <fstream>
#include <algorithm>

const long long UNVALID_INDEX = -1;

void uploadPhrases(std::ifstream& in, Trie& tree, long long& maxLengthPhrase)
{
	maxLengthPhrase = 0;

	in.seekg(0, std::ios::end);
	long long fileSize = in.tellg();
	in.seekg(0, std::ios::beg);

	if (!fileSize)
		throw "The file is empty!";

	char* buffer = new char[fileSize + 1];
	in.read(buffer, fileSize);
	buffer[in.gcount()] = '\0';

	in.close();

	char* p = buffer;
	long long cost;

	while (true)
	{
		long long size = 0;
		while (p[size] != '\n' && p[size])
			++size;

		if (size == 0)
			break;

		long long i = size - 1;

		while (i >= 0 && isWhiteSpace(p[i]))
			--i;

		while (i >= 0 && !isWhiteSpace(p[i]))
			--i;

		cost = atoi(p + i);

		while (i >= 0 && isWhiteSpace(p[i]))
			--i;

		p[i + 1] = '\0';

		maxLengthPhrase = std::max(i + 1, maxLengthPhrase);

		try
		{
			tree.insertPhrase(p, cost);
		}
		catch (std::bad_alloc& ba)
		{
			delete[] buffer;
			throw;
		}

		if (!p[size])
			break;
		p += size + 1;
	}

	delete[] buffer;
}

void initBuffer(std::ifstream& in, CircularBuffer& buffer)
{
	in.seekg(0, std::ios::end);
	long long size = in.tellg();
	in.seekg(0, std::ios::beg);

	long long capacity = 1;
	while (capacity < size << 1) capacity <<= 1;

	while (capacity > 0)
	{
		try
		{
			buffer.init(capacity);
		}
		catch (std::bad_alloc& ba)
		{
			capacity >>= 1;
			continue;
		}

		break;
	}

	if (capacity == 0)
		throw std::bad_alloc();
}

bool readPhrase(Trie& tree, CircularBuffer& buffer,
	long long& totalCost, long long& totalWordsCount)
{
	long long start = 0, endOfPhrase = UNVALID_INDEX;

	long long wordsCount = 0, wordsInPhrase = 0;

	long long costOfPhrase = 0;

	tree.startSearch();

	while (true)
	{
		char ch = buffer.get(start);

		if (isWhiteSpace(ch))
		{
			++wordsCount;

			ch = ' ';
		}
		else if (isBigLetter(ch))
		{
			ch += -'A' + 'a';
		}
		else if (!isLittleLetter(ch))
		{
			++wordsCount;

			if (tree.isWord())
			{
				endOfPhrase = start;
				costOfPhrase = tree.getCost();
				wordsInPhrase = wordsCount;
			}

			break;
		}

		if (ch == ' ' && tree.isWord())
		{
			endOfPhrase = start + 1;
			costOfPhrase = tree.getCost();
			wordsInPhrase = wordsCount;
		}

		if (!tree.next(ch))
			break;

		++start;
	}

	if (endOfPhrase == UNVALID_INDEX)
	{
		return false;
	}
	else
	{
		buffer.go(endOfPhrase);

		totalCost += costOfPhrase;
		totalWordsCount += wordsInPhrase;

		return true;
	}
}

double getFileFactor(std::ifstream& in, Trie& tree, long long maxPhraseLength)
{
	long long ans = 0;
	long long wordsCount = 0;

	CircularBuffer buffer;

	initBuffer(in, buffer);

	long long halfBufferCapacity = buffer.getCapacity() >> 1;

	if (halfBufferCapacity < maxPhraseLength)
		throw std::bad_alloc();

	buffer.read(in);

	bool flag = false;

	while (buffer.get() || in)
	{
		if (buffer.getRead() >= halfBufferCapacity && in)
		{
			buffer.append(in);

			if (flag)
			{
				while (isLetter(buffer.get()))
					buffer.goNext();

				if (!buffer.get())
					continue;
				else
					flag = false;

				while (buffer.get() && !isLetter(buffer.get()))
					buffer.goNext();
			}
		}

		if (!readPhrase(tree, buffer, ans, wordsCount))
		{
			if (isLetter(buffer.get()))
				++wordsCount;

			while (isLetter(buffer.get()))
				buffer.goNext();

			if (!buffer.get())
				flag = true;

			while (buffer.get() && !isLetter(buffer.get()))
				buffer.goNext();
		}
	}

	if (wordsCount)
		return (double)ans / wordsCount;				//!!!!!!!!!!!!!!!!!!!!!!!!!!
	return 0.0;
}