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

#include <fstream>

class KMP;
class HuffmanTree;

class Unarchive
{
public:
	///
	///unarchives the sorcePath file in the destPath folder
	///
	static void unarchive(const char* sorcePath, const char * destPath);

	///
	///lists all the files and folders in the sorcePath archive
	///
	static void list(const char* sorcePath);

	///
	///searches the string string in the all archived files in
	///sorcePath archive
	///
	static void search(const char* sorcePath, const char* string);

private:
	///
	///opens the sorcePath archive and then unarchive it 
	///
	static void helpUnarchive(const char * destPath, const char* sorcePath);

	///
	///creates the filePath file and then unarchive it
	///
	static void unarchiveFile(std::ifstream& in, const char* filePath);

	///
	///reads archived content from the in stream unarchives it
	///and then writes it in the out stream
	///
	static void unarchiveFile(std::ifstream& in, std::ofstream& out);

	///
	///uses dfs for traversing the tree of directories
	///
	static void unarchiveDirectory(std::ifstream& in, const char* currentDirrectory);

private:
	///
	///opens the sorcePath archive and lists all the files in it
	///
	static void helpList(const char* sorcePath);

	///
	///prints the name of the file pointed by the get pointer of
	///the stream in
	///
	static void listArchivedFile(std::ifstream& in, int depth);

	///
	///uses dfs for traversing the tree of directories
	///
	static void listArchivedDirectory(std::ifstream& in, int depth);

private:
	///
	///opens the sorcePath archive and searches for string string
	///in all files stored in the sorcePath archive
	///
	static void helpSearch(const char* sorcePath, const char* string);

	///
	///prints the name of the file pointed by the get pointer of
	///the stream in if it contains the string
	///
	static void searchInFile(std::ifstream& in, KMP& kmp);
	
	///
	///uses dfs for traversing the tree of directories
	///
	static void searchInDirectory(std::ifstream& in, KMP& kmp);

private:
	///
	///returns the byte corresponding to the next prefix Huffman code
	///
	static unsigned char getNextByte(unsigned char* archivedContent, size_t& bytesOffset,
		size_t& bitsOffset, const HuffmanTree& huffmanTree);

	///
	///prints count tabulations
	///
	static void putTabs(int count);
};

