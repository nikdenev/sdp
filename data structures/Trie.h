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

const int DIFFERENT_CHARS = 27;
const int INDEX_OF_SPACE = 26;

class Trie
{
private:

	struct Node
	{
		Node() : 
			isEndOfWord(false), cost(0)
		{
			for (int i = 0; i < DIFFERENT_CHARS; ++i)
				childs[i] = nullptr;
		}
		Node(bool isEnd, int cost) :
			isEndOfWord(isEnd), cost(cost)
		{
			for (int i = 0; i < DIFFERENT_CHARS; ++i)
				childs[i] = nullptr;
		}

		Node* childs[DIFFERENT_CHARS];
		bool isEndOfWord;
		int cost;
	};

public:
	Trie();
	~Trie();
	Trie(const Trie& obj);
	Trie& operator=(const Trie& obj);

	///
	///inserts a whole phrase in the tree
	///
	void insertPhrase(const char* word, int cost);

	///
	///if the phrase is found returns true and assign it's value to cost
	///else false
	///
	bool findPhrase(const char* phrase, int& cost) const;

	///
	///deletes all the nodes without the root
	///
	void clear();

public:

	///
	///use it when you want to begin a search for phrase character by character
	///
	///assigne the root to the current pointer
	///
	inline void startSearch();

	///
	///if there is a path from the current node with character ch returns true
	///else false
	///
	inline bool next(char ch);

	///
	///if the current pointer points to a node which is end of phrase returns true
	///else false
	///
	inline bool isWord() const;

	///
	///returns the cast of the node pointed by the current pointer
	///
	inline int getCost() const;

private:

	///
	///assigne obj to the tree
	///
	void copy(const Trie& obj);

private:

	Node* root;

	///
	///this pointer is used for searching character by character in the tree
	///
	///it points to the last node in which we have been 
	///
	Node* current;
};


void Trie::startSearch()
{
	this->current = this->root;
}

bool Trie::next(char ch)
{
	if (this->current == nullptr)
		return false;

	this->current =
		this->current->childs[ch == ' ' ? INDEX_OF_SPACE : ch - 'a'];

	return this->current != nullptr;
}

bool Trie::isWord() const
{
	return this->current != nullptr && this->current->isEndOfWord;
}

int Trie::getCost() const
{
	if (this->current != nullptr)
	{
		return this->current->cost;
	}

	return 0;
}