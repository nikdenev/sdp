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

struct Code;

class Archive
{
public:
	///
	///archives the file(folder) sorcePath
	///the resulting archive is stores in destPath file
	///
	///if for any reason the process fails the incomplete file
	///is deleted
	///
	static void archive(const char * sorcePath, const char * destPath);

private:
	///
	///creates the file in which is going to be stored the archived content 
	///
	static void helpArchive(const char * sorcePath, const char * destPath);

	///
	///opens the filePath file and writes it's name in the file
	///then it archives the file
	///
	static void archiveFile(std::ofstream & out, const char * filePath, 
		const char * fileName, size_t nameLength, size_t fileSize);

	///
	///archives the content read from the in stream and
	///writes the archived content in the out stream
	///
	static void archiveFile(std::ifstream & in, std::ofstream & out, size_t fileSize);

	///
	///uses dfs for traversing the tree of directories
	///
	static void archiveDirectory(std::ofstream & out, const char * currentDirrectory);

private:
	///
	///gets the frequency of each byte in the file
	///and stores it in the count array
	///
	static void getFrequency(const unsigned char * fileContent, 
		const size_t fileSize, size_t * const count);

	///
	///gets the Huffman prefix codes from the temporary build
	///Huffman tree build from the statistics of the file
	///and then serializes the Huffman tree
	///
	static void getHuffmanCodesAndSerialize(Code * const codes, 
		const unsigned char * fileContent, 
		const size_t fileSize, std::ofstream & out);

	///
	///gets the name of the file from the full name of the file
	///
	static void getFileName(char * const name, 
		unsigned char & length, const char * path);
};