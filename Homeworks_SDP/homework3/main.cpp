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

#include "Helpers.h"

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cerr << "Error! There aren't enough main arguments!\n";
		return 1;
	}

	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cerr << "Problem with " << argv[1] << " file!" << std::endl;
		return 1;
	}

	Trie tree;
	long long maxPhraseLength;

	try
	{
		uploadPhrases(in, tree, maxPhraseLength);
	}
	catch (std::bad_alloc& ba)
	{
		std::cerr << "There isn't enough memory! Can't upload the phrases!\n";
		return 1;
	}
	catch (const char* error)
	{
		std::cerr << "The " << argv[1] << " file is empty!\n";
		return 1;
	}

	for (int i = 2; i < argc; ++i)
	{
		try
		{
			std::ifstream file(argv[i]);
			if (!file)
			{
				std::cerr << "Problem with " << argv[i] << " file!\n";
				continue;
			}

			std::cout << getFileFactor(file, tree, maxPhraseLength) << std::endl;
		}
		catch (std::bad_alloc& ba)
		{
			std::cerr << "There isn't enough memory!\n";
			return 1;
		}
	}

	return 0;
}