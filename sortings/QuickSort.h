#pragma once

#include <algorithm>

#include "MergeSort.h"

template <class T>
void QuickSort(T* arr, int start, int end)
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
			QuickSort(arr, start, l - 1);
			start = l + 1;
		}
		else
		{
			QuickSort(arr, l + 1, end);
			end = l - 1;
		}
	}
}

template <class T>
void QuickSort(T* start, T* end)
{
	QuickSort(start, 0, end - start - 1);
}

//It is NOT better
template <class T>
void BetterQuickSort(T* arr, int start, int end)
{
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

		p = l - start;
		r = end - l;
		if (p < r)
		{
			if (p < 10)
				insertionSort(arr + start, p);
			else
				QuickSort(arr, start, l - 1);


			start = l + 1;
		}
		else
		{
			if (r < 10)
				insertionSort(arr + l + 1, r);
			else
				QuickSort(arr, l + 1, end);
			
			end = l - 1;
		}
	}
}

template <class T>
void BetterQuickSort(T* start, T* end)
{
	BetterQuickSort(start, 0, end - start - 1);
}


template <class T>
void QuickHeapSort(T* arr, int start, int end)
{
	int size = end - start + 1;

	if (size < 32)
	{
		if (size > 1)
			heapSort(arr + start, size);
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
			QuickSort(arr, start, l - 1);
			start = l + 1;
		}
		else
		{
			QuickSort(arr, l + 1, end);
			end = l - 1;
		}
	}
}

template <class T>
void QuickHeapSort(T* start, T* end)
{
	QuickHeapSort(start, 0, end - start - 1);
}
