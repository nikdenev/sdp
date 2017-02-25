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

#include "HuffmanTree.h"

#include <cstring>
#include <fstream>

#include "Heap.h"

HuffmanTree::HuffmanTree(const size_t * const count)
{
	struct Pair
	{
		Pair()
		{}
		Pair(size_t cnt, unsigned short index, unsigned char character) :
			cnt(cnt), index(index), character(character)
		{}

		size_t cnt;
		unsigned short index;
		unsigned char character;
		unsigned char unused;

		bool operator<(const Pair& other)
		{
			return other.cnt < cnt;
		}

	}tempNodes[MAX_BYTES_COUNT];

	size = 0;

	for (size_t i = 0; i < MAX_BYTES_COUNT; ++i)
	{
		if (count[i])
		{
			tempNodes[size] = Pair(count[i], (unsigned short)size, (unsigned char)i);
			++size;
		}
	}

	if (size == 1)
		tempNodes[1] = Pair(1, (unsigned short)(size++), tempNodes[0].character + 1);

	for (unsigned short i = 0; (size_t)i < size; ++i)
		tree[i].init(i, i, tempNodes[i].character, true);

	Heap<Pair> heap(tempNodes, size);

	while (heap.getSize() > 1)
	{
		size_t totalCnt = heap.top().cnt;
		unsigned short indexLeft = heap.getTop().index;
		totalCnt += heap.top().cnt;
		unsigned short indexRight = heap.getTop().index;

		tree[size].init(indexLeft, indexRight, 0, false);

		heap.insert(Pair(totalCnt, (unsigned short)(size++), 0));
	}

	root = (unsigned short)(size - 1);
}

HuffmanTree::HuffmanTree(std::ifstream & in)
{
	unsigned short treeSize;
	in.read((char*)&treeSize, sizeof(unsigned short));

	unsigned char arr[MAX_SERIALIZED_TREE_SIZE];
	in.read((char*)arr, treeSize);

	size = 0;
	int bytes = 0, bits = 0;
	helpDeserialize(arr, bytes, bits);
	root = 0;
}

void HuffmanTree::getCodes(Code * const codes) const
{
	helpToGetCodes(codes, Code(), root);
}

void HuffmanTree::serialize(std::ofstream & out) const
{
	unsigned char arr[MAX_SERIALIZED_TREE_SIZE];
	memset(arr, 0, sizeof(arr));
	int bytes = 0;
	int bits = 0;
	
	helpSerialize(arr, root, bytes, bits);

	unsigned short arrSize = bytes + (bits ? 1 : 0);

	out.write((const char*)&arrSize, sizeof(unsigned short));
	
	out.write((const char*)arr, arrSize);
}

void HuffmanTree::helpSerialize(unsigned char * arr, 
	int cur, int & bytesOffset, int & bitsOffset) const
{
	if (tree[cur].isLeaf)
	{
		arr[bytesOffset] |= BIT_MASK[bitsOffset];
		if (++bitsOffset == 8)
			bitsOffset = 0, ++bytesOffset;

		unsigned char character = tree[cur].character;
		int bitsCnt = 0;
		while (bitsCnt != 8)
		{
			if (character & BIT_MASK[bitsCnt])
				arr[bytesOffset] |= BIT_MASK[bitsOffset];

			if (++bitsOffset == 8)
				bitsOffset = 0, ++bytesOffset;

			++bitsCnt;
		}

		return;
	}

	if (++bitsOffset == 8)
		bitsOffset = 0, ++bytesOffset;

	helpSerialize(arr, tree[cur].left, bytesOffset, bitsOffset);
	helpSerialize(arr, tree[cur].right, bytesOffset, bitsOffset);
}

unsigned short HuffmanTree::helpDeserialize(unsigned char * arr,
	int & bytesOffset, int & bitsOffset)
{
	if (arr[bytesOffset] & BIT_MASK[bitsOffset])
	{
		if (++bitsOffset == 8)
			bitsOffset = 0, ++bytesOffset;

		unsigned char character = 0;
		int bitsCnt = 0;

		while (bitsCnt != 8)
		{
			if (arr[bytesOffset] & BIT_MASK[bitsOffset])
				character += BIT_MASK[bitsCnt];

			if (++bitsOffset == 8)
				bitsOffset = 0, ++bytesOffset;

			++bitsCnt;
		}

		tree[size].init((unsigned short)size, 
			(unsigned short)size, character, true);
		
		return size++;
	}

	if (++bitsOffset == 8)
		bitsOffset = 0, ++bytesOffset;

	int index = size++;

	unsigned short leftIndex = helpDeserialize(arr, bytesOffset, bitsOffset);
	unsigned short rightIndex = helpDeserialize(arr, bytesOffset, bitsOffset);

	tree[index].init(leftIndex, rightIndex, 0, false);

	return index;
}

void HuffmanTree::helpToGetCodes(Code * const & codes, Code curCode, int cur) const
{
	if (tree[cur].isLeaf)
	{
		codes[tree[cur].character] = curCode;
		return;
	}

	++curCode.length;
	helpToGetCodes(codes, curCode, tree[cur].left);

	--curCode.length;
	curCode.code[curCode.length >> 3] |= BIT_MASK[curCode.length & MOD8_MASK];
	++curCode.length;
	helpToGetCodes(codes, curCode, tree[cur].right);
}
