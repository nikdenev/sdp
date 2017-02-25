#pragma once

#include <cassert>

#include "Operator.h"

class Pair
{
public:
	Pair() :
		isNumber(false),
		number(0.0)
	{}

	explicit Pair(double number) :
		isNumber(true),
		number(number)
	{}

	explicit Pair(char operation) :
		isNumber(false),
		number(0.0),
		operation(operation)
	{}

	void setToNumber(double number)
	{
		isNumber = true;
	
		this->number = number;
	}

	void setToOperation(char operation)
	{
		isNumber = false;
	
		this->operation = operation;
	}

	bool isItNumber() const { return this->isNumber; }

	double getNumber() const
	{
		if (!this->isNumber)
			assert(false);

		return this->number;
	}

	char getOperation() const
	{
		if (this->isNumber)
			assert(false);

		return this->operation;
	}

private:
	bool isNumber;
	double number;
	char operation;
};