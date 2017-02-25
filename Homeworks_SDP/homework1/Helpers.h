#pragma once

#include <iostream>
#include <fstream>

#include "DynamicArray.h"

#include "Pair.h"
#include "Operator.h"

Operation getOperationFromChar(char character);

double executeOperation(double x1, double x2, Operation operation);

bool readFileWithOperators(const char* fileName, DynamicArray<Operator>& array);
;
void sortByCharacter(DynamicArray<Operator>& array);

int binarySearch(const DynamicArray<Operator>& array, char ch);

void print(DynamicArray<Pair>& expression);

Operation getOperation(char character, const DynamicArray<Operator>& operators);

bool isWhiteSpace(char ch);

bool getResult(
	const DynamicArray<Operator>& operators,
	DynamicArray<Pair>& postfixExpression,
	std::ifstream& stream,
	double& result);