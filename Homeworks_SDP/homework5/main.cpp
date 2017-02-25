/**
*  
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Nikolay Denev
* @idnumber 45231
* @task 5
* @compiler VC
*
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>

//These two hash functions are not mine
//I have some idea why they work :)

uint32_t hash1(uint64_t key)
{
	key += ~(key << 32);
	key ^= (key >> 22);
	key += ~(key << 13);
	key ^= (key >> 8);
	key += (key << 3);
	key ^= (key >> 15);
	key += ~(key << 27);
	key ^= (key >> 31);

	return key;
}

uint32_t hash2(uint64_t key)
{
	key = (~key) + (key << 21);
	key = (key << 21) - key - 1;
	key = key ^ (key >> 24);
	key = (key + (key << 3)) + (key << 8);
	key = key ^ (key >> 14);
	key = (key + (key << 2)) + (key << 4);
	key = key ^ (key >> 28);
	key = key + (key << 31);

	return key;
}

// for x < 4000000000
bool is_prime(uint32_t x)
{
	for (uint32_t i = 2; i*i <= x; ++i)
	{
		if (x%i == 0)
			return false;
	}

	return true;
}

///
///inserts element in the hash table
///
bool insert_in_table(uint64_t* table, const uint32_t size, const uint64_t value)
{
	uint32_t h1 = hash1(value) % size;
	uint32_t h2 = hash2(value) % size;

	if (table[h1] == value || table[h2] == value)
		return true;

	if (!table[h1])
		table[h1] = value;
	else if (!table[h2])
		table[h2] = value;
	else
	{
		//try with position h1
		uint64_t curValue = table[h1];
		uint32_t curIndex = h1;
		uint32_t temp;

		table[h1] = value;
		while (curValue != value)
		{
			if ((temp = hash1(curValue) % size) == curIndex)
				temp = hash2(curValue) % size;

			if (temp == curIndex)
				break;

			curIndex = temp;

			if (!table[curIndex])
			{
				table[curIndex] = curValue;
				return true;
			}

			std::swap(table[curIndex], curValue);
		}

		//try with position h2
		curValue = table[h2];
		curIndex = h2;
		
		table[h2] = value;

		while (curValue != value)
		{
			if ((temp = hash1(curValue) % size) == curIndex)
				temp = hash2(curValue) % size;

			if (temp == curIndex)
				break;

			curIndex = temp;

			if (!table[curIndex])
			{
				table[curIndex] = curValue;
				return true;
			}

			std::swap(table[curIndex], curValue);
		}

		return false;
	}

	return true;
}

///
///inserts an array of elements in the hash table
///returns true if all insertions were successful 
///else returns false
///
bool insert_all_in_table(const uint64_t* arr, const uint32_t arrSize,
	uint64_t* table, const uint32_t size, uint32_t& zerosCount)
{
	for (uint32_t i = 0; i < arrSize; ++i)
	{
		if (!arr[i])
			zerosCount = 1;
		else if (!insert_in_table(table, size, arr[i]))
			return false;
	}

	return true;
}

///
///builds the hash table for a given size
///returns true if it can
///else returns false
///
bool build_hash_table_fixed_size(const uint64_t* arr, const uint32_t arrSize,
	uint64_t*& table, const uint32_t size, uint32_t& hasZero)
{
	hasZero = 0u;

	table = new uint64_t[size];

	memset(table, 0, size * sizeof(uint64_t));

	if (insert_all_in_table(arr, arrSize, table, size, hasZero))
		return true;

	delete[] table;

	return false;
}

///
///tries to build hash table
///first tries with size = 2 * arrSize
///second with 10 other sizes which are relatively prime with 2 * arrSize
///and third with 5*arrSize
///
bool build_hash_table(const uint64_t* arr, const uint32_t arrSize,
	uint64_t*& table, uint32_t& size, uint32_t& hasZero)
{
	try
	{
		size = arrSize * 2;
		if (build_hash_table_fixed_size(arr, arrSize, table, size, hasZero))
			return true;

		for (int i = 0; i < 10; ++i)
		{
			++size;
			while (!is_prime(size))
				++size;

			if (build_hash_table_fixed_size(arr, arrSize, table, size, hasZero))
				return true;
		}

		size = arrSize * 5;
		if (build_hash_table_fixed_size(arr, arrSize, table, size, hasZero))
			return true;

		return false;
	}
	catch (std::bad_alloc& ba)
	{
		return false;
	}
}

///
///returns the number of elements in the file
///
uint32_t get_elements_count(const std::string& fileName)
{
	std::ifstream in(fileName.c_str(), std::ios::binary);
	if (!in)
		throw "Can't open file for reading!";

	in.seekg(0, std::ios::end);

	uint32_t size = in.tellg();

	in.close();

	return size / sizeof(uint64_t);
}

///
///sorts files by size
///
void sort_by_size(std::pair<uint32_t, std::string>* fileNames, int count)
{
	for (uint32_t i = 0; i < count; ++i)
	{
		fileNames[i].first = get_elements_count(fileNames[i].second);
	}

	std::sort(fileNames, fileNames + count);
}

///
///returns an array of elements from the file
///
uint64_t* read_file(const std::pair<uint32_t, std::string>& file)
{
	std::ifstream in(file.second.c_str(), std::ios::binary);
	if (!in)
		throw "Can't open file for reading!";

	uint64_t* arr = new uint64_t[file.first];

	in.read((char*)arr, file.first * sizeof(uint64_t));

	in.close();

	return arr;
}

///
///builds the hash table
///if it succeed returns true
///else returns false
///
bool build(const std::pair<uint32_t, std::string>& file, 
	uint64_t*& table, uint32_t& size, uint32_t& hasZero)
{
	uint64_t* arr = read_file(file);

	if (!build_hash_table(arr, file.first, table, size, hasZero))
	{
		delete[] arr;
		return false;
	}

	delete[] arr;
	return true;
}

///
///makes the section of all files
///
void section_of_all(const std::pair<uint32_t, std::string>* files, 
	uint32_t filesCount, uint64_t* table, 
	const uint32_t size, uint32_t& hasZero)
{
	const uint8_t HELP_MASK[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
	const uint32_t MOD8_MASK = 7;

	uint32_t vectorSize = size / 8 + 1;
	uint8_t* bitVector = new uint8_t[vectorSize];
	uint32_t zeroInFile;

	for (uint32_t k = 1; k < filesCount;++k)
	{
		zeroInFile = 0u;
		memset(bitVector, 0, vectorSize);

		uint64_t* arr = read_file(files[k]);
		uint32_t arrSize = files[k].first;

		uint32_t index = 0;
		for (uint32_t i = 0; i < arrSize; ++i)
		{
			if (!arr[i])
				zeroInFile = 1u;
			else if (table[index = hash1(arr[i])%size] == arr[i])
			{
				bitVector[index >> 3] |= HELP_MASK[index & MOD8_MASK];
			}
			else if (table[index = hash2(arr[i])%size] == arr[i])
			{
				bitVector[index >> 3] |= HELP_MASK[index & MOD8_MASK];
			}
		}

		for (uint32_t i = 0; i < size; ++i)
		{
			if (table[i])
			{
				if (!(bitVector[i >> 3] & HELP_MASK[i & MOD8_MASK]))
					table[i] = 0ull;
			}
		}

		if (hasZero)
			hasZero = zeroInFile;

		delete arr;
	}

	delete[] bitVector;
}

///
///stores the result
///
void write_in_file(const char* fileName, uint64_t* table, 
	uint32_t size, const uint32_t hasZero)
{
	std::vector<uint64_t> result;

	if (hasZero)
		result.push_back(0ull);

	for (uint32_t i = 0; i < size; ++i)
	{
		if (table[i])
			result.push_back(table[i]);
	}

	std::ofstream out(fileName, std::ios::binary);
	if (!out)
		throw "Can't open file for writing!";

	if (result.size())
		out.write((char*)&result[0], result.size() * sizeof(uint64_t));
}

int main()
{
	std::pair<uint32_t, std::string>* files = nullptr;
	uint64_t* table = nullptr;
	try
	{
		uint32_t n;
		std::cin >> n;

		files = new std::pair<uint32_t, std::string>[n];

		for (uint32_t i = 0; i < n; ++i)
			std::cin >> files[i].second;

		sort_by_size(files, n);

		uint32_t hasZero = 0u;
		table = nullptr;
		uint32_t size;
		if (!build(files[0], table, size, hasZero))
			throw "Can't build the hash table!"; /*I need to have back up but i don't have*/

		section_of_all(files, n, table, size, hasZero);

		write_in_file("result.bin", table, size, hasZero);
	}
	catch (const char* error)
	{
		std::cerr << error << std::endl;
	}
	catch (std::bad_alloc& ba)
	{
		std::cerr << "Memory problem!" << std::endl;
	}

	delete[] table;
	delete[] files;
	
	return 0;
}