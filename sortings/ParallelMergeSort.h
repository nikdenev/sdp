#pragma once

#include <omp.h>

#include "MergeSort.h"

template <class T>
void my_parallel_merge_sort(T* start, T* end)
{
	const int size = end - start;

	int chuncksCount = size / CHUNCK_SIZE;

	int i;

#pragma omp parallel for
	for (i = 0; i < chuncksCount - 1; ++i)
	{
		insertionSort(start + CHUNCK_SIZE*i, CHUNCK_SIZE);
	}

	i = chuncksCount - 1;

	insertionSort(start + i*CHUNCK_SIZE, size - i*CHUNCK_SIZE);

	T* help = new T[size];

	int mergesCount;
	int blockSize = CHUNCK_SIZE;
	int doubleBlockSize = blockSize << 1;
	while (chuncksCount != 1)
	{
		mergesCount = chuncksCount >> 1;

		if (chuncksCount & 1)
		{
#pragma omp parallel for
			for (i = 0; i < mergesCount; ++i)
			{
				merge(start + i * doubleBlockSize, 
					blockSize, blockSize, help + i * doubleBlockSize);
			}
		}
		else
		{
#pragma omp parallel for
			for (i = 0; i < mergesCount - 1; ++i)
			{
				merge(start + i * doubleBlockSize, 
					blockSize, blockSize, help + i * doubleBlockSize);
			}

			i = mergesCount - 1;

			merge(start + i * doubleBlockSize, 
				blockSize, size - (2 * i + 1)*blockSize, help + i * doubleBlockSize);
		}

		chuncksCount -= mergesCount;
		blockSize <<= 1;
		doubleBlockSize <<= 1;
	}

	delete[] help;
}