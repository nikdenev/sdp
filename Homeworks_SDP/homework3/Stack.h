/**
*  
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Nikolay Denev
* @idnumber 45231
* @task 3
* @compiler VC
*
*/

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

