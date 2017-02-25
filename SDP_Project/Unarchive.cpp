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

#include "Unarchive.h"

#include <iostream>
#include <cstring>

#include <windows.h>

#include "HuffmanTree.h"
#include "KMP.h"

void Unarchive::unarchive(const char * sorcePath, const char * destPath)
{
	try
	{
		helpUnarchive(destPath, sorcePath);
	}
	catch (const char* error)
	{
		std::cerr << error << std::endl;
	}
	catch (std::bad_alloc)
	{
		std::cerr << "Error! There is not enought memory!" << std::endl;
	}
	catch (...)
	{
		std::cout << "Error!" << std::endl;
	}
}

void Unarchive::list(const char * sorcePath)
{
	try
	{
		helpList(sorcePath);
	}
	catch (const char* error)
	{
		std::cerr << error << std::endl;
	}
	catch (std::bad_alloc)
	{
		std::cerr << "Error! There is not enought memory!" << std::endl;
	}
	catch (...)
	{
		std::cout << "Error!" << std::endl;
	}
}

void Unarchive::search(const char * sorcePath, const char * string)
{
	try
	{
		helpSearch(sorcePath, string);
	}
	catch (const char* error)
	{
		std::cerr << error << std::endl;
	}
	catch (std::bad_alloc)
	{
		std::cerr << "Error! There is not enought memory!" << std::endl;
	}
	catch (...)
	{
		std::cout << "Error!" << std::endl;
	}
}

///////////////////////////////////////////////////////////////

void Unarchive::helpUnarchive(const char * destPath, const char * sorcePath)
{
	std::ifstream in(sorcePath, std::ios::binary);
	if (!in)
		throw "Error! Can't open file for reading!";

	unsigned char type, nameLength;
	in.read((char*)&type, sizeof(unsigned char));
	in.read((char*)&nameLength, sizeof(unsigned char));

	char path[MAX_PATH_LENGTH];
	strcpy(path, destPath);
	size_t pathLength = strlen(path);

	in.read(path + pathLength, nameLength);
	path[pathLength + nameLength] = '\0';

	if (type == 1)
		unarchiveFile(in, path);
	else
	{
		if (!CreateDirectoryA(path, NULL))
			throw "Error! Can't create directory!";

		unarchiveDirectory(in, path);
	}
}


void Unarchive::unarchiveFile(std::ifstream & in, const char * filePath)
{
	std::ofstream out(filePath, std::ios::binary);
	if (!out)
		throw "Error! Can't open file for writing!";

	try
	{
		unarchiveFile(in, out);
	}
	catch (...)
	{
		out.close();
		remove(filePath);
		throw;
	}
}


void Unarchive::unarchiveFile(std::ifstream & in, std::ofstream & out)
{
	size_t unarchivedSize;
	in.read((char*)&unarchivedSize, sizeof(size_t));
	if (!unarchivedSize)
		return;

	HuffmanTree huffmanTree(in);

	size_t archivedSize;
	in.read((char*)&archivedSize, sizeof(size_t));

	unsigned char* archivedContent = new unsigned char[archivedSize];
	in.read((char*)archivedContent, archivedSize);

	unsigned char* unarchivedContent;
	try
	{
		unarchivedContent = new unsigned char[unarchivedSize];
	}
	catch (std::bad_alloc)
	{
		delete[] archivedContent;
		throw;
	}

	size_t bytesOffset = 0;
	size_t bitsOffset = 0;

	size_t bytesCnt = 0;
	while (bytesCnt != unarchivedSize)
	{
		unarchivedContent[bytesCnt++] =
			getNextByte(archivedContent, bytesOffset, bitsOffset, huffmanTree);
	}

	out.write((const char*)unarchivedContent, unarchivedSize);

	delete[] archivedContent;
	delete[] unarchivedContent;
}


void Unarchive::unarchiveDirectory(std::ifstream & in, const char * currentDirrectory)
{
	unsigned short filesCount;
	in.read((char*)&filesCount, sizeof(unsigned short));

	char fullPath[MAX_PATH_LENGTH];
	strcat(strcpy(fullPath, currentDirrectory), "\\");

	size_t pathLength = strlen(fullPath);

	unsigned char type, nameLength;
	for (unsigned short i = 0; i < filesCount; ++i)
	{
		in.read((char*)&type, sizeof(unsigned char));

		in.read((char*)&nameLength, sizeof(unsigned char));
		in.read((char*)(fullPath + pathLength), nameLength);
		fullPath[pathLength + nameLength] = '\0';

		if (type == 1)
		{
			std::ofstream out(fullPath, std::ios::binary);
			if (!out)
				throw "Error! Can't open file for writing!";

			try
			{
				unarchiveFile(in, out);
			}
			catch (...)
			{
				out.close();
				remove(fullPath);
				throw;
			}
		}
		else if (type == 2)
		{
			if (!CreateDirectoryA(fullPath, NULL))
				throw "Error! Can't create directory!";

			unarchiveDirectory(in, strcat(fullPath, "\\"));
		}
	}
}

///////////////////////////////////////////////////////////////


void Unarchive::helpList(const char * sorcePath)
{
	std::ifstream in(sorcePath, std::ios::binary);
	if (!in)
		throw "Error! Can't open file for reading!";

	unsigned char type;
	in.read((char*)&type, sizeof(unsigned char));

	if (type == 1)
		listArchivedFile(in, 1);
	else
		listArchivedDirectory(in, 1);
}


void Unarchive::listArchivedFile(std::ifstream & in, int depth)
{
	unsigned char nameLength, buffer[MAX_PATH_LENGTH];
	in.read((char*)&nameLength, sizeof(unsigned char));
	in.read((char*)buffer, nameLength);
	buffer[nameLength] = '\0';

	unsigned short treeSize;
	size_t fileSize;

	in.read((char*)&fileSize, sizeof(size_t));
	if (fileSize)
	{
		in.read((char*)&treeSize, sizeof(unsigned short));
		in.seekg(treeSize, std::ios::cur);
		in.read((char*)&fileSize, sizeof(size_t));
		in.seekg(fileSize, std::ios::cur);
	}

	putTabs(depth);
	std::cout << buffer << std::endl;
}


void Unarchive::listArchivedDirectory(std::ifstream & in, int depth)
{
	unsigned char nameLength;
	char buffer[MAX_PATH_LENGTH];
	in.read((char*)&nameLength, sizeof(unsigned char));
	in.read((char*)buffer, nameLength);
	buffer[nameLength] = '\0';

	putTabs(depth - 1);
	std::cout << buffer << std::endl;

	unsigned short filesCount;
	in.read((char*)&filesCount, sizeof(unsigned short));

	for (int i = 0; i < filesCount; ++i)
	{
		unsigned char type;
		in.read((char*)&type, sizeof(unsigned char));

		if (type == 1)
			listArchivedFile(in, depth);
		else
			listArchivedDirectory(in, depth + 1);
	}
}

///////////////////////////////////////////////////////////////


void Unarchive::helpSearch(const char * sorcePath, const char * string)
{
	std::ifstream in(sorcePath, std::ios::binary);
	if (!in)
		throw "Error! Can't open file for reading!";

	KMP kmp(string);

	unsigned char type;
	in.read((char*)&type, sizeof(unsigned char));

	if (type == 1)
		searchInFile(in, kmp);
	else
		searchInDirectory(in, kmp);
}

void Unarchive::searchInFile(std::ifstream & in, KMP& kmp)
{
	unsigned char nameLength;
	char fileName[MAX_PATH_LENGTH];
	in.read((char*)&nameLength, sizeof(unsigned char));
	in.read((char*)&fileName, nameLength);
	fileName[nameLength] = '\0';

	size_t unarchivedSize;
	in.read((char*)&unarchivedSize, sizeof(size_t));
	if (!unarchivedSize)
		return;

	HuffmanTree huffmanTree(in);

	size_t archivedSize;
	in.read((char*)&archivedSize, sizeof(size_t));

	unsigned char* archivedContent = new unsigned char[archivedSize];
	in.read((char*)archivedContent, archivedSize);

	size_t bytesOffset = 0;
	size_t bitsOffset = 0;

	size_t bytesCnt = 0;
	while (bytesCnt != unarchivedSize)
	{
		if (kmp.goNext(getNextByte(archivedContent, bytesOffset,
			bitsOffset, huffmanTree)))
		{
			std::cout << fileName << std::endl;
			delete[] archivedContent;
			return;
		}

		++bytesCnt;
	}

	delete[] archivedContent;
}


void Unarchive::searchInDirectory(std::ifstream & in, KMP& kmp)
{
	unsigned char nameLength;
	in.read((char*)&nameLength, sizeof(unsigned char));
	in.seekg(nameLength, std::ios::cur);

	unsigned short filesCount;
	in.read((char*)&filesCount, sizeof(unsigned short));

	for (int i = 0; i < filesCount; ++i)
	{
		unsigned char type;
		in.read((char*)&type, sizeof(unsigned char));

		if (type == 1)
			searchInFile(in, kmp);
		else
			searchInDirectory(in, kmp);
	}
}

///////////////////////////////////////////////////////////////

unsigned char Unarchive::getNextByte(unsigned char * archivedContent,
	size_t & bytesOffset, size_t & bitsOffset, const HuffmanTree & huffmanTree)
{
	unsigned short currentIndex = huffmanTree.root;

	while (!huffmanTree.tree[currentIndex].isLeaf)
	{
		currentIndex = (
			(archivedContent[bytesOffset] & BIT_MASK[bitsOffset]) ?
			huffmanTree.tree[currentIndex].right :
			huffmanTree.tree[currentIndex].left);

		if (++bitsOffset == 8)
			bitsOffset = 0, ++bytesOffset;
	}

	return huffmanTree.tree[currentIndex].character;
}

void Unarchive::putTabs(int count)
{
	for (int i = 0; i < count; ++i)
		std::cout << '\t';
}