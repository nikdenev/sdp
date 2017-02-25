#pragma once

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <cassert>
#include <ctime>

const int CHUNCK_SIZE = 8;

template <class T>
inline void insertionSort(T* start, int size)
{
	int i, j;
	T temp;
	for (i = 1; i < size; ++i)
	{
		temp = start[i];
		for (j = i - 1; j >= 0; --j)
		{
			if (temp < start[j])
				start[j + 1] = start[j];
			else
				break;
		}
		start[j + 1] = temp;
	}
}

template <class T>
inline void merge(T* start, int size1, int size2, T* help)
{
	const int size_minus_1 = size1 + size2 - 1;
	const T* startRight = start + size1;

	int i;

	for (i = 0; i < size1; ++i)
		help[i] = start[i];

	for (i = 0; i < size2; ++i)
		help[size_minus_1 - i] = startRight[i];
	
	int l = 0, r = size_minus_1;

	while (l <= r)
	{
		if (help[r] < help[l])
			*start++ = help[r--];
		else
			*start++ = help[l++];
	}
}

template <class T>
void my_merge_sort(T* start, T* end)
{
	const int size = end - start;

	int chuncksCount = size / CHUNCK_SIZE;

	int i;

	for (i = 0; i < chuncksCount - 1; ++i)
	{
		insertionSort(start + CHUNCK_SIZE*i, CHUNCK_SIZE);
	}

	insertionSort(start + i*CHUNCK_SIZE, size - i*CHUNCK_SIZE);

	T* help = new T[size];

	int mergesCount;
	int blockSize = CHUNCK_SIZE;
	while (chuncksCount != 1)
	{
		mergesCount = chuncksCount >> 1;

		if (chuncksCount & 1)
		{
			for (i = 0; i < mergesCount; ++i)
			{
				merge(start + i * 2 * blockSize, blockSize, blockSize, help);
			}
		}
		else
		{
			for (i = 0; i < mergesCount - 1; ++i)
			{
				merge(start + i * 2 * blockSize, blockSize, blockSize, help);
			}

			merge(start + i * 2 * blockSize, blockSize, size - (2 * i + 1)*blockSize, help);
		}

		chuncksCount -= mergesCount;
		blockSize <<= 1;
	}

	delete[] help;
}