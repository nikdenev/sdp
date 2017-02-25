#pragma once

#include "Helpers.h"

const double EPS = 1E-9;

enum Operation
{
	ADDITION,
	SUBTRACTION,
	MULTIPLICATION,
	DIVISION,
	OPERATIONS_COUNT,
	UNVALID_OPERATION
};

class BinaryOperator
{
public:
	BinaryOperator() : 
		character(0),
		operandsCount(0)
	{}

	BinaryOperator(char character, unsigned char operandsCount) :
		character(character),
		operandsCount(operandsCount)
	{}

	void setCharacter(char ch)
	{
		this->character = ch;
	}

	void setOperandsCount(unsigned char operandsCount)
	{
		this->operandsCount = operandsCount;
	}

	char getCharacter() const
	{
		return this->character;
	}

	unsigned char getOperandsCount() const
	{
		return this->operandsCount;
	}

private:
	char character;
	unsigned char operandsCount;
};

class Operator
{
public:
	Operator();
	
	void set(char character, char operation, bool isLeftAssociative);

	char getCharacter() const;

	Operation getOperation() const;

	//true left-associative
	//false right-associative
	bool getAssociation() const;
	
private:
	char character;
	Operation operation;
	bool isLeftAssociative;
};

inline Operator::Operator() :
	character(0),
	operation(UNVALID_OPERATION),
	isLeftAssociative(false)
{}

Operation getOperationFromChar(char character);

inline void Operator::set(char character, char operation, bool isLeftAssociative)
{
	this->character = character;
	this->isLeftAssociative = isLeftAssociative;

	this->operation = getOperationFromChar(operation);
}

inline char Operator::getCharacter() const
{
	return this->character;
}

inline Operation Operator::getOperation() const {
	return this->operation;
}

inline bool Operator::getAssociation() const
{
	return this->isLeftAssociative;
}
