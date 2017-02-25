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

#include <fstream>

class CircularBuffer
{
public:

	CircularBuffer();

	~CircularBuffer();

	///
	///allocates memory for the bufrer
	///
	void init(long long capacity);

	///
	///appends number of read bytes from the stream to the end of the buffer
	///
	void append(std::ifstream& in);

	///
	///reads capacity bytes from the stream and 
	///store them in the buffer starting from index 0
	///
	void read(std::ifstream& in);

	///
	///returns the number of read characters
	///
	long long getRead() const;

	///
	///gets the characters offset positions after the current position
	///
	char get(long long offset = 0);

	void goNext();

	///
	///moves the current position offset position forward
	///
	void go(long long offset);

	long long getCapacity() const;

private:
	CircularBuffer(const CircularBuffer& obj) {}
	void operator=(const CircularBuffer& obj) {}

private:

	char* buffer;
	long long capacity;

	///
	///start is the position from which we start to read the buffer
	///
	long long start, end, current;

	///
	///is true if the last operation was moving the current pointer forward
	///else false
	///
	bool isGet;
};