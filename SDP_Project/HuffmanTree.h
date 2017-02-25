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
#include <cstring>

#include "Unarchive.h"

#define SEARCHED_STRING_MAX_LENGTH 2048
#define MAX_PATH_LENGTH 256

#define MAX_BYTES_COUNT 256
#define MAX_NODES_COUNT 512
#define MAX_SERIALIZED_TREE_SIZE 320
#define MAX_CODE_LENGTH 32

const unsigned char BIT_MASK[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
const unsigned char MOD8_MASK = 7;

//Huffman prefix code
struct Code
{
	Code() :
		length(0)
	{
		memset(code, 0, sizeof(code));
	}

	unsigned char code[MAX_CODE_LENGTH];
	size_t length;
};

/*
*** How the Huffman tree is serialized?
* for each node in the tree:
* if the node is not leaf then it stores one 0 bit
* if the node is leaf then it stores one 1 bit
* and it's corresponding one byte 
* it's done by pre-order dfs 
*/

class HuffmanTree
{
	friend Unarchive;

private:
	struct Node
	{
		Node() 
		{}

		Node(unsigned short left, unsigned short right, 
			unsigned char ch, bool isLeaf) :
			left(left), right(right), character(ch), isLeaf(isLeaf)
		{}

		void init(unsigned short left, unsigned short right,
			unsigned char ch, bool isLeaf)
		{
			this->left = left;
			this->right = right;
			this->character = ch;
			this->isLeaf = isLeaf;
		}

		//the indices of left and rigth child of the node
		unsigned short left;
		unsigned short right;
		unsigned char character;
		bool isLeaf;
	};

public:
	///
	///builds the Huffman tree from the 
	///statistics stored in the count array
	///
	HuffmanTree(const size_t* const count);

	///
	///builds the Huffman tree from the serialized
	///Huffman tree stored in the in stream 
	///
	HuffmanTree(std::ifstream& in);

	///
	///stores the prefix Huffman codes in the
	///codes array
	///
	void getCodes(Code* const codes) const;

	///
	///serializes the Huffman tree on the out stream
	///
	void serialize(std::ofstream& out) const;

private:
	///
	///uses dfs to serialize the tree
	///
	void helpSerialize(unsigned char* arr,
		int cur, int& bytesOffset, int& bitsOffset) const;

	///
	///uses dfs to deserialize the tree
	///
	unsigned short helpDeserialize(unsigned char* arr,
		int& bytesOffset, int& bitsOffset);

	///
	///uses dfs for traversing the tree and stores the
	///codes in the corresponding indices of the codes array
	///
	void helpToGetCodes(Code* const& codes, Code curCode, int cur) const;

private:
	Node tree[MAX_NODES_COUNT];
	unsigned short size;
	unsigned short root;
};