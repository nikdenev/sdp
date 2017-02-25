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

#include "AVL_KV.h"

#include "Helpers.h"

void run(const char* fileName)
{
	AVL_KV<int, std::string> avl;

	buildPerfectTree(fileName, avl);

	//execute the requests
	doRequests(avl);
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Problem with main argoments!\n";
		return 1;
	}

	run(argv[1]);

	return 0;
}






































/*
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <random>
#include <set>


void makeFile(const char* fileName, int arr[], const char* strings[], int size)
{
	std::ofstream out(fileName);

	for (int i = 0; i < size; ++i)
	{
		out.write((const char*)(&arr[i]), sizeof(int));

		int temp;
		temp = (int)strlen(strings[i]);

		out.write((const char*)(&temp), sizeof(int));

		out.write(strings[i], temp);
	}
}

void makeFile(const char* fileName, std::vector<int> arr, std::vector<std::string> strings, int size)
{
	std::ofstream out(fileName, std::ios::binary);

	assert(arr.size() == size);
	assert(strings.size() == size);

	for (int i = 0; i < size; ++i)
	{
		out.write((const char*)(&arr[i]), sizeof(int));

		assert(strings[i].size() > 0);

		int temp;
		temp = (int)strlen(strings[i].c_str());

		out.write((const char*)(&temp), sizeof(int));

		if (temp)
			out.write(strings[i].c_str(), temp);
	}
}

void genArr(std::vector<int>& arr, int size)
{
	std::default_random_engine generator;

	std::uniform_int_distribution<int> distribution(-size/3, size/3);

	for (int i = 0; i < size; ++i)
	{
		arr.push_back(distribution(generator));
	}
}

void genStrings(std::vector<std::string>& s, int size)
{
	std::default_random_engine generator;

	std::uniform_int_distribution<int> distribution(33, 127);

	for (int i = 0; i < size; ++i)
	{
		int len = distribution(generator)%31 + 1;

		std::string temp;

		for (int j = 0; j < len; ++j)
		{
			temp.push_back((char)distribution(generator));
		}

		s.push_back(temp);
	}
}

std::string genString(int size)
{
	std::default_random_engine generator;

	std::uniform_int_distribution<int> distributionChar(33, 127);

	std::string temp;

	for (int i = 0; i < size; ++i)
	{
		temp.push_back((char)distributionChar(generator));
	}

	return temp;
}

void makeInFile(const char* fileName , int size)
{
	std::ofstream out(fileName);

	std::default_random_engine generator;

	std::uniform_int_distribution<int> distributionChar(33, 127);

	std::uniform_int_distribution<int> distributionInt(-size/2, size/2);

	srand(0);

	for (int i = 0; i < size; ++i)
	{
		int choose = rand() % 4;

		int key;
		std::string data;

		if (choose == 0)
		{
			key = distributionInt(generator);

			data = genString(distributionChar(generator)%31 + 1);

			out << "add ";
			out << key << ' ';
			out << data << '\n';
		}
		else if (choose == 1)
		{
			key = distributionInt(generator);

			data = genString(distributionChar(generator) % 31 + 1);

			out << "remove ";
			out << key << ' ';
			out << data << '\n';
		}
		else if (choose == 2)
		{
			key = distributionInt(generator);

			out << "removeall ";
			out << key << '\n';
		}
		else
		{
			key = distributionInt(generator);

			data = genString(distributionChar(generator) % 31 + 1);

			out << "search ";
			out << key << ' ';
			out << data << '\n';
		}
	}
}

void TEST_IS_IT_PERFECT_2()
{
	int size = 2000;

	std::vector<int> arr;

	genArr(arr, size);

	std::vector<std::string> strings;

	genStrings(strings, size);

	const char* fileName = "file.bin";

	makeFile(fileName, arr, strings, size);

	AVL_KV<int, std::string> avl;

	buildPerfectTree(fileName, avl);

	assert(avl.isWeightBalanced());
	assert(avl.isHeightBalanced());

	makeInFile("test.txt", size);

	doRequests(avl);
}

unsigned getCount(int arr[])
{
	unsigned cnt = 0;
	for (int i = 0; i < 20000;++i)
	{
		if (arr[i] > 0)
			++cnt;
	}

	return cnt;
}

void add(int x, int arr[])
{
	if (x < 0)
		x += 20000;

	++arr[x];
}

void rem(int x, int arr[])
{
	if (x < 0)
		x += 20000;

	--arr[x];
}

void TEST_AVL_KV_1()
{
	int initSize = 3000;

	std::default_random_engine gen;

	std::uniform_int_distribution<int> dist(0, initSize/2);

	AVL_KV<int, int> AVL_KV;

	int Count[30000];
	memset(Count, 0, sizeof(Count));

	int temp;

	for (int i = 0; i < initSize; ++i)
	{
		try
		{
			if (i % 100 == 0)
				std::cout << i << std::endl;

			AVL_KV.insert(temp = dist(gen), 0);
			add(temp, Count);
		}
		catch (...)
		{
			--i;
		}
	}

	assert(AVL_KV.getSize() == getCount(Count));
	
	srand(0);

	int changesSize = 3*initSize;
	
	for (int i = 0; i < changesSize; ++i)
	{
		if (rand() & 1)
		{
			try
			{
				AVL_KV.insert(temp = dist(gen), 0);
				add(temp, Count);
			}
			catch (...)
			{
				--i;
			}
		}
		else
		{
			try
			{
				AVL_KV.remove(temp = dist(gen), 0);
				rem(temp, Count);
			}
			catch (...)
			{
				--i;
			}
		}
	}

	assert(AVL_KV.getSize() == getCount(Count));
}

void TEST_AVL_KV_2()
{
	AVL_KV<int, int> AVL_KV;

	AVL_KV.insert(1, 0);
	AVL_KV.insert(2, 0);
	AVL_KV.insert(3, 0);
	AVL_KV.insert(4, 0);
	AVL_KV.insert(5, 0);
	AVL_KV.insert(6, 0);
	AVL_KV.insert(7, 0);
	AVL_KV.insert(8, 0);
	AVL_KV.insert(9, 0);

	AVL_KV.remove(2, 0);
	
	AVL_KV.insert(6, 312);

	AVL_KV.remove(6, 0);
}

void AVL_KV_COPY_CLEAR()
{
	AVL_KV<int, int> avl;

	avl.insert(9, 0);
	avl.insert(6, 0);
	avl.insert(16, 0);
	avl.insert(4, 0);
	avl.insert(8, 0);
	avl.insert(11, 0);
	avl.insert(40, 0);
	avl.insert(30, 0);
	avl.insert(50, 0);
	avl.insert(2, 0);
	avl.insert(5, 0);
	avl.insert(7, 0);
	avl.insert(10, 0);
	avl.insert(14, 0);
	avl.insert(1, 0);
	avl.insert(3, 0);
	avl.insert(12, 0);
	avl.insert(15, 0);
	avl.insert(25, 0);
	avl.insert(35, 0);
	avl.insert(45, 0);
	avl.insert(22, 0);
	avl.insert(26, 0);

	AVL_KV<int, int> AVL_KV2;

	AVL_KV2.insert(65, 0);

	AVL_KV2 = avl;

	assert(AVL_KV2.getSize() == avl.getSize());

	avl.clear();
	AVL_KV2.clear();

	assert(AVL_KV2.getSize() == 0);
}

int main()
{
	TEST_IS_IT_PERFECT_2();

	return 0;
}
*/