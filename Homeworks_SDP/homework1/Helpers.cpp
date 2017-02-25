#include "Helpers.h"

#include "Stack.h"

#include <cassert>

#define UNVALID_INDEX -1

Operation getOperationFromChar(char character)
{
	switch (character)
	{
	case '+':
		return ADDITION;
	case '-':
		return SUBTRACTION;
	case '*':
		return MULTIPLICATION;
	case '/':
		return DIVISION;
	default:
		return UNVALID_OPERATION;
	}

	assert(false);
}

double executeOperation(double x1, double x2, Operation operation)
{
	switch (operation)
	{
	case ADDITION:
		return x1 + x2;
	case SUBTRACTION:
		return x1 - x2;
	case MULTIPLICATION:
		return x1*x2;
	case DIVISION:
	{
		if (abs(x2) > EPS)
			return x1 / x2;
		else
			throw "Can't divide by 0!";
	}
	default:
		assert(false);
	}
}

bool readFileWithOperators(const char* fileName, DynamicArray<Operator>& array)
{
	std::ifstream stream(fileName);
	if (!stream)
		return false;

	char character;
	char operation;
	bool leftAssociative;

	Operator temp;

	while (stream)
	{
		stream >> character >> operation >> leftAssociative;

		if (!stream)
			break;

		temp.set(character, operation, leftAssociative);

		array.append(temp);
	}

	if (!stream && !stream.eof())
		return false;

	return true;
}

void sortByCharacter(DynamicArray<Operator>& array)
{
	size_t size = array.getSize();

	for (int i = 1; i < size; ++i)
	{
		for (int j = i; j > 0; --j)
		{
			if (array[j].getCharacter() > array[j - 1].getCharacter())
				break;

			//swap
			Operator temp = array[j];
			array[j] = array[j - 1];
			array[j - 1] = temp;
		}
	}
}

int binarySearch(const DynamicArray<Operator>& array, char ch)
{
	int left = 0, right = array.getSize() - 1;

	while (left <= right)
	{
		int middle = (right + left) / 2;

		char current = array[middle].getCharacter();

		if (current == ch)
			return middle;

		if (current > ch)
			right = middle - 1;
		else
			left = middle + 1;
	}

	return UNVALID_INDEX;
}

void print(DynamicArray<Pair>& expression)
{
	size_t size = expression.getSize();

	for (int i = 0; i < size; ++i)
	{
		if (expression[i].isItNumber())
			std::cout << expression[i].getNumber() << ' ';
		else
			std::cout << expression[i].getOperation() << ' ';
	}

	std::cout << '\n';
}

Operation getOperation(char character, const DynamicArray<Operator>& operators)
{
	int index = binarySearch(operators, character);

	if (index == UNVALID_INDEX)
		return UNVALID_OPERATION;

	return operators[index].getOperation();
}

bool isWhiteSpace(char ch)
{
	return ch == ' ' || ch == '\n' || ch == '\t'
		|| ch == '\v' || ch == '\f' || ch == 'r';
}

bool getResult(
	const DynamicArray<Operator>& operators,
	DynamicArray<Pair>& postfixExpression,
	std::ifstream& stream,
	double& result)
{
	Stack<BinaryOperator> operatorsStack(16);
	Stack<double> resultStack(16);

	int numbersCount = 0, operationsCount = 0;

	while (true)
	{
		stream >> std::ws;

		if (!stream.good())
			break;

		char temp = stream.peek();

		if (temp >= '0' && temp <= '9')
		{
			++numbersCount;

			if (operationsCount + 1 < numbersCount)
				return false;

			double number;
			
			stream >> number;

			postfixExpression.append(Pair(number));

			resultStack.push(number);

			if (!operatorsStack.isEmpty() && operatorsStack.top().getOperandsCount())
			{
				while (!operatorsStack.isEmpty() && operatorsStack.top().getOperandsCount())
				{
					postfixExpression.append(Pair(operatorsStack.top().getCharacter()));

					double secondOperand = resultStack.top();
					resultStack.pop();
					double firstOperand = resultStack.top();
					resultStack.pop();

					try
					{
						resultStack.push(executeOperation(
							firstOperand,
							secondOperand,
							getOperation(operatorsStack.top().getCharacter(), operators)));
					}
					catch (const char* error)
					{
						//for division by 0
						return false;
					}

					operatorsStack.pop();
				}

				if (!operatorsStack.isEmpty())
					operatorsStack.top().setOperandsCount(1);
			}
			else
			{
				if (!operatorsStack.isEmpty())
					operatorsStack.top().setOperandsCount(1);
			}
		}
		else
		{
			++operationsCount;

			if (operationsCount + 1 < numbersCount)
				return false;

			char ch;

			stream >> ch;

			if (!isWhiteSpace(stream.peek()))
				return false;

			Operation operation = getOperation(ch, operators);

			if (operation == UNVALID_OPERATION)
				return false;

			operatorsStack.push(BinaryOperator(ch, 0));
		}
	}

	if (operationsCount + 1 != numbersCount)
		return false;

	result = resultStack.top();
	resultStack.pop();

	if (!resultStack.isEmpty())
		return false;
	
	assert(operatorsStack.isEmpty());

	return true;
}

//////////////////////////////////////////////////////////////

/*
char* myStrtok(char* sorce, char delimiter, char*& next)
{
if (sorce == nullptr)
return nullptr;

while (*sorce != '\0' && *sorce == delimiter) ++sorce;

if (*sorce == '\0')
return nullptr;

char* result = sorce;

while (*sorce != '\0' && *sorce != delimiter) ++sorce;

if (*sorce == '\0')
next = nullptr;
else
next = sorce + 1;

*sorce = '\0';

return result;
}

bool stringToDouble(const char* str, double& result)
{
result = 0;

bool isNegative = false;

if (str[0] == '-')
isNegative = true;

size_t sizeBeforDot = 0;

while (str[sizeBeforDot] >= '0' && str[sizeBeforDot] <= '9')
{
++sizeBeforDot;
}

if (sizeBeforDot == 0 ||
(str[0] == '0' && (str[1] != '\0' && str[1] != '.')))
return false;

double temp = 1.0;

for (int i = sizeBeforDot - 1; i >= 0; --i)
{
result += (str[i] - '0')*temp;

temp *= 10;
}

if (str[sizeBeforDot] == '\0')
{
if (isNegative)
result *= -1.0;

return true;
}

if (str[sizeBeforDot] != '.' ||
(str[sizeBeforDot] == '.' && !(str[sizeBeforDot + 1] >= '0' &&
str[sizeBeforDot + 1] <= '9')))
return false;

++sizeBeforDot;

temp = 1.0 / 10.0;

while (str[sizeBeforDot] >= '0' && str[sizeBeforDot] <= '9')
{
result += (str[sizeBeforDot] - '0')*temp;

temp /= 10.0;

++sizeBeforDot;
}

if (str[sizeBeforDot] != '\0')
return false;

if (isNegative)
result *= -1.0;

return true;
}

const Operator& getOperator(char character, const DynamicArray<Operator>& operators)
{
int index = binarySearch(operators, character);

if (index == UNVALID_INDEX)
return Operator();

return operators[index];
}

/*
double getResult(
const DynamicArray<Operator>& operators,
DynamicArray<Pair>& postfixExpression,
std::ifstream& stream)
{
stream >> std::ws;

if (stream.eof())
throw "Error";

char temp = stream.peek();

if (temp >= '0' && temp <= '9')
{
double number;
stream >> number;

postfixExpression.append(Pair(number));

return number;
}

char ch;
stream >> ch;

Operation operation = getOperation(ch, operators);

if (operation == UNVALID_OPERATION || !isWhileSpace(stream.get()))
throw "Error";

double firstOperand = getResult(operators, postfixExpression, stream);
double secondOperand = getResult(operators, postfixExpression, stream);

postfixExpression.append(Pair(ch));

//devision by 0!!!!!!
return executeOperation(firstOperand, secondOperand, operation);
}
*/

//////////////////////////////////////////////////////////////