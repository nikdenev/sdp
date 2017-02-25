#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>

#include "Helpers.h"

#include "DynamicArray.h"

#include "Operator.h"
#include "Pair.h"

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Error!!!" << std::endl;

		return 0;
	}

	DynamicArray<Operator> operators;

	if (!readFileWithOperators(argv[2], operators))
	{
		std::cout
			<< "Error! Problem with "
			<< argv[2] << " file!" << std::endl;

		return 0;
	}

	sortByCharacter(operators);

	std::ifstream stream(argv[1]);
	if (!stream)
	{
		std::cout
			<< "Error! Problem with "
			<< argv[1] << " file!" << std::endl;

		return 0;
	}

	DynamicArray<Pair> postfixExpression;

	double result = 0;
	
	if(!getResult(operators, postfixExpression, stream,result))
	{
		std::cout << "Error" << std::endl;

		return 0;
	}
	
	print(postfixExpression);

	std::cout << std::fixed << std::setprecision(5);
	std::cout << result << std::endl;
	
	return 0;
}