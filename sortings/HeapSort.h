#pragma once

template <class T>
inline void siftDown(int pos, T* start, int size)
{
	T temp = start[pos];

	int child;
	while ((child = (pos << 1) + 1) < size)
	{
		if (child + 1 < size && start[child] < start[child + 1])
			++child;

		if (temp < start[child])
			start[pos] = start[child];
		else
			break;

		pos = child;
	}

	start[pos] = temp;
}

template <class T>
void heapify(T* start, int size)
{
	for (int i = (size - 1) / 2; i >= 0; --i)
	{
		siftDown(i, start, size);
	}
}

template <class T>
void heapSort(T* start, int size)
{
	heapify(start, size);

	T temp;
	for (int i = size - 1; i > 0; --i)
	{
		temp = start[0];
		start[0] = start[i];
		start[i] = temp;
		siftDown(0, start, i);
	}
}

template <class T>
void heapSort(T* start, T* end)
{
	heapSort(start, end - start);
}