#pragma once

#include <algorithm>

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
void quickSort(T* arr, int start, int end)
{
	int size = end - start + 1;

	if (size < 10)
	{
		if (size > 1)
			insertionSort(arr + start, size);
		return;
	}

	while (start < end)
	{
		int l = start - 1, r = end, p = (start + end) / 2;
		T pivot = arr[p];

		std::swap(arr[p], arr[end]);

		while (l < r)
		{
			while (arr[++l] < pivot);

			while ((l < r) && pivot < arr[--r]);

			std::swap(arr[l], arr[r]);
		}

		std::swap(arr[l], arr[end]);

		if (l - start < end - l)
		{
			quickSort(arr, start, l - 1);
			start = l + 1;
		}
		else
		{
			quickSort(arr, l + 1, end);
			end = l - 1;
		}
	}
}

template <class T>
void quickSort(T* start, T* end)
{
	quickSort(start, 0, end - start - 1);
}