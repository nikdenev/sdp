/**
*  
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Nikolay Denev
* @idnumber 45231
* @task 0
* @compiler VC
*
*/

#include <iostream>
#include <cstring>

#include "Archive.h"
#include "Unarchive.h"


int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Error! There are not enough main arguments" << std::endl;
		return 1;
	}

	if (!strcmp(argv[1], "-Pack") && argc == 4)
		Archive::archive(argv[2], argv[3]);
	else if (!strcmp(argv[1], "-Unpack") && argc == 4)
		Unarchive::unarchive(argv[2], argv[3]);
	else if (!strcmp(argv[1], "-List") && argc == 3)
		Unarchive::list(argv[2]);
	else if (!strcmp(argv[1], "-Search") && argc == 4)
		Unarchive::search(argv[2], argv[3]);
	else
	{
		std::cerr << "Error! Unknown command!" << std::endl;
		return 1;
	}

	return 0;
}