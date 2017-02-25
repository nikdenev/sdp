/**
*  
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Nikolay Denev
* @idnumber 45231
* @task 4
* @compiler VC
*
*/

#include "Helpers.h"

#include <fstream>
#include <cstring>
#include <algorithm>
#include <cassert>

#include "QuickSort.h"

std::ifstream myIn("test.txt");

//#define myIn std::cin

void upload(std::ifstream& stream, std::vector<std::pair<int, char*> >& vector)
{
	int key;
	int dataSize;
	char* str;

	while (true)
	{
		stream.read((char*)(&key), sizeof(int));
		stream.read((char*)(&dataSize), sizeof(int));

		if (!stream)
			break;

		assert(dataSize >= 0);

		str = new char[dataSize + 1];

		if (dataSize)
		{
			stream.read(str, dataSize);
			if (!stream)
			{
				delete str;
				break;
			}
		}

		str[dataSize] = '\0';

		vector.push_back(std::make_pair(key, str));
	}
}

void insertInTree(AVL_KV<int, std::string>& tree,
	const std::vector<std::pair<int, char*> >& vector)
{
	int size = vector.size();

	int* help = new int[size];
	memset(help, 0, sizeof(help[0])*size);

	int start = size;

	bool flag = true;

	while (flag)
	{
		flag = false;

		int old = start;

		int index;

		if (start)
		{
			flag = true;

			index = (start - 1) >> 1;

			tree.insert(vector[index].first, std::string(vector[index].second));

			help[index] = start;
			start = index;
		}

		int i = old;
		while (i < size)
		{
			old = help[i];

			if (help[i] - i > 1)
			{
				flag = true;

				index = (help[i] + i) >> 1;

				tree.insert(vector[index].first, std::string(vector[index].second));

				help[index] = help[i];
				help[i] = index;
			}

			i = old;
		}
	}

	delete[] help;
}

void buildPerfectTree(const char* fileName, AVL_KV<int, std::string>& avl)
{
	std::ifstream in(fileName, std::ios::binary);
	if (!in)
	{
		std::cout << "Error! Problem with " << fileName << " file!" << std::endl;
		return;
	}

	std::vector<std::pair<int, char*> > vector;

	upload(in, vector);
	in.close();

	if (!vector.empty())
	{
		quickSort(&vector[0], 0, vector.size() - 1);
		assert(std::is_sorted(vector.begin(), vector.end()));

		std::vector<std::pair<int, char*> > otherVector;

		unsigned vectorSize = vector.size();

		bool* checked = new bool[vectorSize];
		memset(checked, 0, sizeof(checked[0])*vectorSize);

		int index = 0;
		while (index < vectorSize)
		{
			otherVector.push_back(std::make_pair(
				vector[index].first, 
				vector[index].second));

			checked[index] = true;

			while (index + 1 < vectorSize &&
				vector[index].first == vector[index + 1].first)
			{
				++index;
			}

			++index;
		}

		insertInTree(avl, otherVector);

		for (int i = 0; i < vectorSize; ++i)
		{
			if (!checked[i])
			{
				avl.insert(vector[i].first, std::string(vector[i].second));
			}

			delete vector[i].second;
		}

		delete checked;
	}
}

void add(AVL_KV<int, std::string>& avl, int key, std::string& data)
{
	myIn >> std::ws;
	getline(myIn, data);

	avl.insert(key, data);
}

void remove(AVL_KV<int, std::string>& avl, int key, std::string& data)
{
	myIn >> std::ws;
	getline(myIn, data);

	if (avl.remove(key, data))
		std::cout << "true" << std::endl;
	else
		std::cout << "false" << std::endl;
}

void removeAll(AVL_KV<int, std::string>& avl, int key)
{
	std::cout << avl.removeAll(key) << std::endl;
}

void search(AVL_KV<int, std::string >& avl, int key, std::string& data)
{
	myIn >> std::ws;
	getline(myIn, data);

	if (avl.find(key, data))
		std::cout << "true" << std::endl;
	else
		std::cout << "false" << std::endl;
}

void doRequests(AVL_KV<int, std::string>& avl)
{
	char commandType[1024];
	int key;
	std::string data;

	while (myIn)
	{
		myIn >> commandType;
		if (!myIn)
			break;

		if (!(myIn >> key))
		{
			std::cout << "Error! Unvalid request!" << std::endl;
			return;
		}

		if (!strcmp(commandType, "add"))
		{
			add(avl, key, data);
		}
		else if (!strcmp(commandType, "remove"))
		{
			remove(avl, key, data);
		}
		else if (!strcmp(commandType, "removeall"))
		{
			removeAll(avl, key);
		}
		else if (!strcmp(commandType, "search"))
		{
			search(avl, key, data);
		}
		else
		{
			std::cout << "Unvalid request!" << std::endl;
		}
	}
}