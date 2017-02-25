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

#include <algorithm>

template <class T>
class Heap
{
public:
	Heap(const size_t capacity = 15) :
		size(0),
		capacity(capacity),
		heap(nullptr)
	{
		if (capacity)
			heap = new T[capacity + 1];
	}

	///
	///creates heap from the elements of the arr array 
	///O(n) time complexity
	///
	Heap(const T* arr, const size_t size) :
		size(size),
		capacity(size)
	{
		heap = new T[size + 1];

		for (size_t i = 1; i <= size; ++i)
			heap[i] = arr[i - 1];

		for (size_t i = size >> 1; i > 0; --i)
			siftDown(i);
	}

	~Heap()
	{
		delete[] heap;
	}

	Heap(const Heap& other) :
		size(0),
		capacity(0),
		heap(nullptr)
	{
		copy(other);
	}

	Heap& operator=(const Heap& other)
	{
		if (this != &other)
		{
			clear();
			copy(other);
		}

		return *this;
	}

	///
	///if there isn't enought allocated memory for the new element
	///the array which holds the elements is resized
	///O(log(n)) amortized time complexity
	///
	void insert(const T& element)
	{
		if (capacity == size)
			resize(2 * size + 1);

		heap[++size] = element;
		siftUp(size);
	}

	///
	///to perform this operation the heap must NOT be empty
	///
	const T& top() const
	{
		return heap[1];
	}

	///
	///to perform this operation the heap must NOT be empty
	///
	T getTop()
	{
		std::swap(heap[1], heap[size]);
		--size;
		siftDown(1);
		return heap[size + 1];
	}

	size_t getSize() const
	{
		return size;
	}

	bool isEmpty() const
	{
		return size == 0;
	}

	void clear()
	{
		delete[] heap;
		heap = nullptr;
		size = 0;
		capacity = 0;
	}

private:
	void siftDown(size_t index)
	{
		T temp = heap[index];
		size_t curIndex;

		while (index <= (size >> 1))
		{
			curIndex = index << 1;
			if (curIndex < size && heap[curIndex] < heap[curIndex + 1])
				++curIndex;
			
			if (!(temp < heap[curIndex]))
				break;

			heap[index] = heap[curIndex];
			index = curIndex;
		}
		
		heap[index] = temp;
	}

	void siftUp(size_t index)
	{
		T temp = heap[index];

		while (index != 1 && heap[index >> 1] < temp)
		{
			heap[index] = heap[index >> 1];
			index >>= 1;
		}

		heap[index] = temp;
	}

	void resize(size_t newCapacity)
	{
		T* newHeap = new T[newCapacity + 1];

		for (size_t i = 1; i <= size; ++i)
		{
			newHeap[i] = heap[i];
		}

		delete[] heap;
		heap = newHeap;
		capacity = newCapacity;
	}

	void copy(const Heap& other)
	{
		heap = new T[other.capacity + 1];
		size = other.size;
		capacity = other.capacity;

		for (size_t i = 1; i <= size; ++i)
		{
			heap[i] = other.heap[i];
		}
	}

private:
	T* heap;
	size_t size;
	size_t capacity;
};