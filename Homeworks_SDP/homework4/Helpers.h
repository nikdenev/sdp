/**
*  
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Nikolay Denev
* @idnumber 45231
* @task 4
* @compiler VC
*
*/

#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "AVL_KV.h"

///
///stores the information from the file in the vector
///
void upload(std::ifstream& stream, std::vector<std::pair<int, char*> >& vector);

///
///inserts elements from the vector in special order in the AVL
///
void insertInTree(AVL_KV<int, std::string>& tree,
	const std::vector<std::pair<int, char*> >& vector);

///
///makes perfect tree from from the information in the file
///
void buildPerfectTree(const char* fileName, AVL_KV<int, std::string>& avl);

void add(AVL_KV<int, std::string>& avl, int key, std::string& data);

void remove(AVL_KV<int, std::string>& avl, int key, std::string& data);

void removeAll(AVL_KV<int, std::string>& avl, int key);

void search(AVL_KV<int, std::string>& avl, int key, std::string& data);

///
///execute requests
///
void doRequests(AVL_KV<int, std::string>& avl);
















