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

#include "CircularBuffer.h"

CircularBuffer::CircularBuffer() :
	buffer(nullptr)
{}

CircularBuffer::~CircularBuffer()
{
	delete[] buffer;
}

void CircularBuffer::init(long long capacity)
{
	if (!buffer)
	{
		buffer = new char[capacity];

		this->capacity = capacity;

		start = end = current = 0;
		isGet = false;
	}
}

void CircularBuffer::append(std::ifstream& in)
{
	if (!in)
		return;

	isGet = false;

	if (start < current)
	{
		in.read(buffer + start, current - start);

		end = start + in.gcount();
	}
	else if (start > current)
	{
		in.read(buffer + start, capacity - start);

		if (in)
		{
			in.read(buffer, current);

			end = in.gcount();
		}
		else
		{
			end = (start + in.gcount()) % capacity;
		}
	}
	else
	{
		read(in);
	}

	start = current;
}

void CircularBuffer::read(std::ifstream& in)
{
	isGet = false;

	in.read(buffer, capacity);

	start = current = 0;

	end = in.gcount() % capacity;
}

long long CircularBuffer::getRead() const
{
	if (start < current)
	{
		return current - start;
	}
	else if (start > current)
	{
		return capacity - start + current;
	}
	else
	{
		if (isGet)
			return capacity;
		else
			return 0;
	}
}

char CircularBuffer::get(long long offset)
{
	if ((offset + current) % capacity == end && isGet)
		return '\0';

	if (current != end)
		isGet = true;

	return buffer[(current + offset) % capacity];
}

void CircularBuffer::goNext()
{
	current = (current + 1) % capacity;
}

void CircularBuffer::go(long long offset)
{
	current = (current + offset) % capacity;
}

long long CircularBuffer::getCapacity() const
{
	return capacity;
}