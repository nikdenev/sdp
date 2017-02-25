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

#include <algorithm>
#include <cmath>
#include <cassert>

#include "AVL_K.h"

#define AVL_KV_MAX_DEPTH 64

template <class Key, class Value>
class AVL_KV
{
private:
	struct Node
	{
		Node(const Key& k, const AVL_K<Value>& otherAVL) :
			key(k), avl(otherAVL), height(1),
			left(nullptr), right(nullptr)
		{}

		Node(const Key& k, AVL_K<Value>& otherAVL,
			Node* l, Node* r, char h = 1) :
			key(k), avl(otherAVL), left(l), right(r),
			height(h)
		{}

		Key key;

		AVL_K<Value> avl;

		Node* left;
		Node* right;

		char height;
	};

public:
	AVL_KV();
	~AVL_KV();
	AVL_KV(const AVL_KV<Key, Value>& obj);
	AVL_KV& operator= (const AVL_KV<Key, Value>& obj);

	void insert(const Key& key, const Value& value);

	unsigned removeAll(const Key& key);

	bool remove(const Key& key, const Value& value);

	bool find(const Key& key, const Value& value) const;

	unsigned getSize() const;

	void clear();

private:
	void clear(Node* node);

	char getHeight(Node* node) const;

	void rightRotation(Node*& father);

	void leftRotation(Node*& father);

private:
	Node* copy(Node* otherRoot)
	{
		if (otherRoot)
		{
			return new Node(otherRoot->key, otherRoot->avl,
				copy(otherRoot->left),
				copy(otherRoot->right),
				otherRoot->height);
		}
		
		return nullptr;
	}

	Node* extractMin(Node** node, Node** stack[], int& stackSize)
	{
		while ((*node)->left)
		{
			stack[stackSize++] = node;
			node = &(*node)->left;
		}
	
		Node* temp = *node;
		*node = (*node)->right;

		return temp;
	}

private:
	Node* root;
	unsigned nodesCount;

public:
	bool isWeightBalanced() const;
	bool isHeightBalanced() const;

private:
	unsigned isWeightBalanced(Node* node, bool& result) const;
	unsigned isHeightBalanced(Node* node, bool& result) const;
};

template<class Key, class Value>
inline AVL_KV<Key, Value>::AVL_KV() :
	root(nullptr), nodesCount(0)
{

}

template<class Key, class Value>
inline AVL_KV<Key, Value>::~AVL_KV()
{
	this->clear();
}

template<class Key, class Value>
inline AVL_KV<Key, Value>::AVL_KV(const AVL_KV<Key, Value> & obj) :
	root(copy(obj.root)), 
	nodesCount(obj.nodesCount)
{
	assert(isHeightBalanced());
}

template<class Key, class Value>
inline AVL_KV<Key, Value> & AVL_KV<Key, Value>::operator=(const AVL_KV<Key, Value> & obj)
{
	if (this != &obj)
	{
		this->clear();

		this->root = copy(obj.root);
		this->nodesCount = obj.nodesCount;

		assert(isHeightBalanced());
	}

	return *this;
}

template<class Key, class Value>
inline void AVL_KV<Key, Value>::insert(const Key & key, const Value & value)
{
	Node** stack[AVL_KV_MAX_DEPTH];
	stack[0] = &this->root;
	int stackSize = 1;

	while (true)
	{
		Node*& cur = *stack[stackSize - 1];

		if (!cur)
		{
			++this->nodesCount;
			cur = new Node(key, AVL_K<Value>(value));
			break;
		}

		if (cur->key == key)
		{
			cur->avl.insert(value);
			return;
		}

		if (cur->key < key)
			stack[stackSize++] = &cur->right;
		else
			stack[stackSize++] = &cur->left;
	}

	--stackSize;
	while (stackSize)
	{
		Node*& cur = *stack[stackSize - 1];

		char lH, rH, d;
		cur->height = 1 +
			std::max(
				lH = getHeight(cur->left),
				rH = getHeight(cur->right));

		d = rH - lH;
		if (d == 0)
			break;

		if (d > 1)
		{
			if (getHeight(cur->right->right) - getHeight(cur->right->left) > 0)
			{
				//rr
				leftRotation(cur);
			}
			else
			{
				//rl
				rightRotation(cur->right);
				leftRotation(cur);
			}

			break;
		}
		else if (d < -1)
		{
			if (getHeight(cur->left->right) - getHeight(cur->left->left) > 0)
			{
				//lr
				leftRotation(cur->left);
				rightRotation(cur);
			}
			else
			{
				//ll
				rightRotation(cur);
			}

			break;
		}

		--stackSize;
	}

	assert(isHeightBalanced());
}

template<class Key, class Value>
inline unsigned AVL_KV<Key, Value>::removeAll(const Key & key)
{
	--this->nodesCount;
	unsigned result;

	Node** stack[AVL_KV_MAX_DEPTH];
	stack[0] = &this->root;
	int stackSize = 1;

	while (true)
	{
		Node*& cur = *stack[stackSize - 1];

		if (!cur)
		{
			++this->nodesCount;
			return 0u;
		}

		if (cur->key == key)
		{
			result = cur->avl.getCount();

			if (cur->left && cur->right)
			{
				int oldStackSize = stackSize;
				Node* min = extractMin(&cur->right, stack, stackSize);
				min->right = cur->right;
				min->left = cur->left;
				min->height = cur->height;

				Node* toDel = cur;
				cur = min;
				delete toDel;

				stack[oldStackSize] = &cur->right;
			}
			else if (cur->left)
			{
				--stackSize;
				Node* temp = cur;
				cur = cur->left;
				delete temp;
			}
			else /* has only right child or doesn't have child */
			{
				--stackSize;
				Node* temp = cur;
				cur = cur->right;
				delete temp;
			}

			break;
		}

		if (cur->key < key)
			stack[stackSize++] = &cur->right;
		else
			stack[stackSize++] = &cur->left;
	}


	while (stackSize)
	{
		Node*& cur = *stack[stackSize - 1];

		__int8 lH, rH, d;
		cur->height = 1 +
			std::max(
				lH = getHeight(cur->left),
				rH = getHeight(cur->right));

		d = rH - lH;
		if (d == 1 || d == -1)
			break;

		if (d > 1)
		{
			if (getHeight(cur->right->right) - getHeight(cur->right->left) < 0)
			{
				//rl
				rightRotation(cur->right);
				leftRotation(cur);
			}
			else
			{
				//rr
				leftRotation(cur);
			}
		}
		else if (d < -1)
		{
			if (getHeight(cur->left->right) - getHeight(cur->left->left) > 0)
			{
				//lr
				leftRotation(cur->left);
				rightRotation(cur);
			}
			else
			{
				//ll
				rightRotation(cur);
			}
		}

		--stackSize;
	}

	assert(isHeightBalanced());
	return result;
}

template<class Key, class Value>
inline bool AVL_KV<Key, Value>::remove(const Key & key, const Value & value)
{
	Node** stack[AVL_KV_MAX_DEPTH];
	stack[0] = &this->root;
	int stackSize = 1;

	while (true)
	{
		Node*& cur = *stack[stackSize - 1];

		if (!cur)
			return false;

		if (cur->key == key)
		{
			if (!cur->avl.remove(value))
				return false;

			if (cur->avl.getSize())
			{
				return true;
			}
			else
			{
				--this->nodesCount;

				if (cur->left && cur->right)
				{
					int oldStackSize = stackSize;
					Node* min = extractMin(&cur->right, stack, stackSize);
					min->right = cur->right;
					min->left = cur->left;
					min->height = cur->height;

					Node* toDel = cur;
					cur = min;
					delete toDel;

					stack[oldStackSize] = &cur->right;
				}
				else if (cur->left)
				{
					--stackSize;
					Node* temp = cur;
					cur = cur->left;
					delete temp;
				}
				else /* has only right child or doesn't have child */
				{
					--stackSize;
					Node* temp = cur;
					cur = cur->right;
					delete temp;
				}

				break;
			}
		}

		if (cur->key < key)
			stack[stackSize++] = &cur->right;
		else
			stack[stackSize++] = &cur->left;
	}


	while (stackSize)
	{
		Node*& cur = *stack[stackSize - 1];

		__int8 lH, rH, d;
		cur->height = 1 +
			std::max(
				lH = getHeight(cur->left),
				rH = getHeight(cur->right));

		d = rH - lH;
		if (d == 1 || d == -1)
		{
			assert(isHeightBalanced());
			break;
		}

		if (d > 1)
		{
			if (getHeight(cur->right->right) - getHeight(cur->right->left) < 0)
			{
				//rl
				rightRotation(cur->right);
				leftRotation(cur);
			}
			else
			{
				//rr
				leftRotation(cur);
			}
		}
		else if (d < -1)
		{
			if (getHeight(cur->left->right) - getHeight(cur->left->left) > 0)
			{
				//lr
				leftRotation(cur->left);
				rightRotation(cur);
			}
			else
			{
				//ll
				rightRotation(cur);
			}
		}

		--stackSize;
	}

	assert(isHeightBalanced());
	return true;
}

template<class Key, class Value>
inline bool AVL_KV<Key, Value>::find(const Key & key, const Value& value) const
{
	Node* cur = this->root;

	while (true)
	{
		if (!cur)
			return false;

		if (cur->key == key)
			return true;

		if (cur->key < key)
			cur = cur->right;
		else
			cur = cur->left;
	}
}

template<class Key, class Value>
inline unsigned AVL_KV<Key, Value>::getSize() const
{
	return this->nodesCount;
}

template<class Key, class Value>
inline void AVL_KV<Key, Value>::clear()
{
	clear(this->root);
	this->root = nullptr;
	this->nodesCount = 0;
}

/////////////////////////////////////////////////////////

template<class Key, class Value>
inline void AVL_KV<Key, Value>::clear(Node * node)
{
	if (node)
	{
		clear(node->left);
		clear(node->right);
		delete node;
	}
}

template<class Key, class Value>
inline char AVL_KV<Key, Value>::getHeight(Node * node) const
{
	if (!node)
		return 0;

	return node->height;
}

template<class Key, class Value>
inline void AVL_KV<Key, Value>::rightRotation(Node *& father)
{
	Node* temp = father;
	father = father->left;
	temp->left = father->right;
	father->right = temp;

	father->right->height = 1 +
		std::max(getHeight(father->right->left), getHeight(father->right->right));

	father->height = 1 +
		std::max(getHeight(father->left), getHeight(father->right));
}

template<class Key, class Value>
inline void AVL_KV<Key, Value>::leftRotation(Node *& father)
{
	Node* temp = father;
	father = father->right;
	temp->right = father->left;
	father->left = temp;

	father->left->height = 1 + 
		std::max(getHeight(father->left->left), getHeight(father->left->right));

	father->height = 1 + 
		std::max(getHeight(father->left), getHeight(father->right));
}

//////////////////////////////////////////////////////////////

template<class Key, class Value>
inline bool AVL_KV<Key, Value>::isWeightBalanced() const
{
	bool ans = true;
	isWeightBalanced(this->root, ans);

	return ans;
}

template<class Key, class Value>
inline bool AVL_KV<Key, Value>::isHeightBalanced() const
{
	bool ans = true;
	isHeightBalanced(this->root, ans);

	return ans;
}

template<class Key, class Value>
inline unsigned AVL_KV<Key, Value>::isWeightBalanced(Node * node, bool& result) const
{
	if (!node)
		return 0;

	int wl = isWeightBalanced(node->left, result);

	if (!result)
		return 0;

	int wr = isWeightBalanced(node->right, result);

	if (!result)
		return 0;

	if (abs(wl - wr) > 1)
		result = false;

	return 1 + wl + wr;
}

template<class Key, class Value>
inline unsigned AVL_KV<Key, Value>::isHeightBalanced(Node * node, bool& result) const
{
	if (!node)
		return 0;

	char hl = isHeightBalanced(node->left, result);

	if (!result)
		return 0;

	assert(hl == getHeight(node->left));

	char hr = isHeightBalanced(node->right, result);

	if (!result)
		return 0;

	assert(hr == getHeight(node->right));

	if (abs(hl - hr) > 1)
		result = false;

	return 1 + std::max(hl, hr);
}

