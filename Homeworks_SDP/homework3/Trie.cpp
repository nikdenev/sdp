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

#include "Trie.h"
#include "Stack.h"

Trie::Trie()
{
	this->current = this->root = new Node;
}

Trie::~Trie()
{
	this->clear();
	delete this->root;
}

Trie::Trie(const Trie & obj)
{
	this->current = this->root = new Node;
	
	this->copy(obj);
}

Trie & Trie::operator=(const Trie & obj)
{
	if (this != &obj)
	{
		this->clear();

		this->copy(obj);
	}

	return *this;
}

void Trie::insertPhrase(const char * word, int cost)
{
	Node* temp = this->root;

	int index = *word == ' ' ? INDEX_OF_SPACE : *word - 'a';

	while (*word && temp->childs[index] != nullptr)
	{
		temp = temp->childs[index];
		++word;
		index = *word == ' ' ? INDEX_OF_SPACE : *word - 'a';
	}

	while (*word)
	{
		temp = temp->childs[index] = new Node;
		++word;
		index = *word == ' ' ? INDEX_OF_SPACE : *word - 'a';
	}

	temp->isEndOfWord = true;
	temp->cost = cost;
}

bool Trie::findPhrase(const char* phrase, int& cost) const
{
	if (!phrase)
		return false;

	Node* temp = this->root;

	while (*phrase && temp != nullptr)
	{
		temp = temp->childs[*phrase == ' ' ? INDEX_OF_SPACE : *phrase - 'a' ];
		++phrase;
	}

	if (temp == nullptr || *phrase)
		return false;

	cost = temp->cost;

	return temp->isEndOfWord;
}

void Trie::clear()
{
	Stack<Node*> stack;
	stack.push(this->root);

	while (!stack.isEmpty())
	{
		Node* temp = stack.top();
		stack.pop();

		for (int i = 0; i < DIFFERENT_CHARS; ++i)
		{
			if (temp->childs[i] != nullptr)
				stack.push(temp->childs[i]);
		}

		delete[] temp;
	}

	this->current = this->root = new Node;
}

void Trie::copy(const Trie& obj)
{
	Stack<Node*> otherStack;
	otherStack.push(obj.root);
	
	Stack<Node*> myStack;
	myStack.push(this->root);

	while (!myStack.isEmpty())
	{
		Node* otherTemp = otherStack.top();
		otherStack.pop();

		Node* myTemp = myStack.top();
		myStack.pop();

		for (int i = 0; i < DIFFERENT_CHARS; ++i)
		{
			if (otherTemp->childs[i] != nullptr)
			{
				otherStack.push(otherTemp->childs[i]);

				myStack.push(myTemp->childs[i] = 
					new Node(otherTemp->childs[i]->isEndOfWord,
						otherTemp->childs[i]->cost));
			}
		}
	}
}