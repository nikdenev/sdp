#pragma once

#include "DynamicArray.h"

template <class TYPE>
class Stack
{
public:
	Stack(size_t capacity = 0) :
		dynamicArray(capacity)
	{}

	void push(const TYPE& element)
	{
		this->dynamicArray.append(element);
	}

	void pop()
	{
		this->dynamicArray.removeLast();
	}

	const TYPE& top() const
	{
		return this->dynamicArray.getLast();
	}

	TYPE& top()
	{
		return this->dynamicArray.getLast();
	}

	bool isEmpty() const
	{
		return this->dynamicArray.isEmpty();
	}

	size_t getSize() const
	{
		return this->dynamicArray.getSize();
	}

	size_t getCapacity() const
	{
		return this->dynamicArray.getCapacity();
	}

	void clear()
	{
		this->dynamicArray.clear();
	}

private:
	DynamicArray<TYPE> dynamicArray;
};

