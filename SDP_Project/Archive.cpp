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

#include "Archive.h"

#include "dirent.h"
#include <sys/stat.h>

#include <cstdio>
#include <iostream>
#include <cstring>

#include "HuffmanTree.h"


void Archive::archive(const char * sorcePath, const char * destPath)
{
	try
	{
		helpArchive(sorcePath, destPath);
		return;
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

	remove(destPath);
}

///////////////////////////////////////////////////////////////

void Archive::helpArchive(const char* sorcePath, const char * destPath)
{
	std::ofstream out(destPath, std::ios::binary);
	if (!out)
	{
		std::cerr << "Error! Can't create " << destPath << " file!" << std::endl;
		return;
	}

	struct stat pathInfo;
	stat(sorcePath, &pathInfo);

	if (S_ISDIR(pathInfo.st_mode))
		archiveDirectory(out, sorcePath);
	else if (S_ISREG(pathInfo.st_mode))
	{
		unsigned char nameLength;
		char fileName[MAX_PATH_LENGTH];
		getFileName(fileName, nameLength, sorcePath);

		archiveFile(out, sorcePath, fileName, nameLength, pathInfo.st_size);
	}
	else
		throw "Error! Can't archive file with unknown type!";
}

void Archive::archiveFile(std::ofstream & out, const char * filePath, 
	const char * fileName, size_t nameLength ,size_t fileSize)
{
	std::ifstream in(filePath, std::ios::binary);
	if (!in)
		throw "Error! Can't open file for reading!";

	unsigned char type = 1;
	out.write((const char*)&type, sizeof(unsigned char));
	out.write((const char*)&nameLength, sizeof(unsigned char));
	out.write(fileName, nameLength);

	archiveFile(in, out, fileSize);
}

void Archive::archiveFile(std::ifstream & in, std::ofstream & out, size_t fileSize)
{
	out.write((const char*)&fileSize, sizeof(size_t));
	if (!fileSize)
		return;

	unsigned char* unarchivedContent = new unsigned char[fileSize];
	in.read((char*)unarchivedContent, fileSize);

	Code codes[MAX_BYTES_COUNT];
	unsigned char* archivedContent;
	try
	{
		getHuffmanCodesAndSerialize(codes, unarchivedContent, fileSize, out);

		archivedContent = new unsigned char[fileSize];
	}
	catch (...)
	{
		delete[] unarchivedContent;
		throw;
	}
	
	memset(archivedContent, 0, fileSize);

	size_t bytesOffset = 0;
	size_t bitsOffset = 0;
	for (size_t i = 0; i < fileSize; ++i)
	{
		const Code& code = codes[unarchivedContent[i]];
		size_t curBit = 0;

		while (curBit != code.length)
		{
			if (code.code[curBit >> 3] & BIT_MASK[curBit & MOD8_MASK])
				archivedContent[bytesOffset] |= BIT_MASK[bitsOffset];

			if (++bitsOffset == 8)
				bitsOffset = 0, ++bytesOffset;

			++curBit;
		}
	}

	bytesOffset += (bitsOffset ? 1 : 0);
	out.write((const char*)&bytesOffset, sizeof(size_t));
	out.write((const char*)archivedContent, bytesOffset);

	delete[] unarchivedContent;
	delete[] archivedContent;
}


void Archive::archiveDirectory(std::ofstream & out, const char * currentDirrectory)
{
	unsigned char type = 2;
	out.write((const char*)&type, sizeof(unsigned char));

	char path[MAX_PATH_LENGTH];
	unsigned char nameLength;
	getFileName(path, nameLength, currentDirrectory);

	out.write((const char*)&nameLength, sizeof(unsigned char));
	out.write(path, strlen(path));

	struct DIR* directoryPointer;
	struct dirent* currentEntry;
	struct stat fileInfo;

	directoryPointer = opendir(currentDirrectory);
	if (!directoryPointer)
		throw "Error! Can't open directory!";

	unsigned short filesCount = 0;
	while (currentEntry = readdir(directoryPointer))
		if (strncmp(currentEntry->d_name, ".", 2) && 
			strncmp(currentEntry->d_name, "..", 3))
			++filesCount;

	out.write((const char*)&filesCount, sizeof(unsigned short));

	rewinddir(directoryPointer);

	while (currentEntry = readdir(directoryPointer))
	{
		if (!strcmp(currentEntry->d_name, ".") || 
			!strcmp(currentEntry->d_name, ".."))
			continue;

		strcat(strcat(strcpy(path, currentDirrectory), "\\"), currentEntry->d_name);

		stat(path, &fileInfo);
		try
		{
			if (S_ISDIR(fileInfo.st_mode))
				archiveDirectory(out, path);
			else if (S_ISREG(fileInfo.st_mode))
				archiveFile(out, path, currentEntry->d_name,
					currentEntry->d_namlen, fileInfo.st_size);
			else
				throw "Error! Can't archive file with unknown type!";
		}
		catch (...)
		{
			closedir(directoryPointer);
			throw;
		}
	}

	closedir(directoryPointer);
}

///////////////////////////////////////////////////////////////

void Archive::getFrequency(const unsigned char * fileContent,
	const size_t fileSize, size_t * const count)
{
	for (size_t i = 0; i < fileSize; ++i)
		++count[fileContent[i]];
}

void Archive::getHuffmanCodesAndSerialize(Code * const codes,
	const unsigned char * fileContent, const size_t fileSize, 
	std::ofstream& out)
{
	size_t count[MAX_BYTES_COUNT];
	memset(count, 0, sizeof(count));
	getFrequency(fileContent, fileSize, count);

	HuffmanTree huffmanTree(count);

	huffmanTree.serialize(out);

	huffmanTree.getCodes(codes);
}

void Archive::getFileName(char * const name, unsigned char& length, const char * path)
{
	int pathLength = (int)strlen(path);
	int i = pathLength - 1;
	for (; i >= 0; --i)
	{
		if (path[i] == '\\' || path[i] == '/')
			break;
	}

	strcpy(name, path + i + 1);
	length = (unsigned char)(pathLength - i - 1);
}
