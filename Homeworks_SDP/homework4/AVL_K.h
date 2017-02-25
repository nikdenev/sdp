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

#define AVL_K_MAX_DEPTH 64

template <class Key>
class AVL_K
{
private:
	struct Node
	{
		Node(const Key& k) :
			key(k), count(1), height(1),
			left(nullptr), right(nullptr)
		{}

		Node(const Key& k, unsigned cnt, Node* l, Node* r, char h = 1) :
			key(k), count(cnt), left(l), right(r),
			height(h)
		{}

		Key key;

		unsigned count;

		Node* left;
		Node* right;

		char height;
	};

public:
	AVL_K();
	explicit AVL_K(const Key& key);
	~AVL_K();
	AVL_K(const AVL_K<Key>& obj);
	AVL_K<Key>& operator= (const AVL_K<Key>& obj);

	void insert(const Key& key);

	unsigned removeAll(const Key& key);

	bool remove(const Key& key);

	bool find(const Key& key) const;

	unsigned getSize() const;

	unsigned getCount() const;

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
			return new Node(otherRoot->key, otherRoot->count,
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
	unsigned allCount;

public:
	bool isWeightBalanced() const;
	bool isHeightBalanced() const;

private:
	unsigned isWeightBalanced(Node* node, bool& result) const;
	unsigned isHeightBalanced(Node* node, bool& result) const;
};

template<class Key>
inline AVL_K<Key>::AVL_K() :
	root(nullptr), nodesCount(0), allCount(0)
{

}

template<class Key>
inline AVL_K<Key>::AVL_K(const Key & key) :
	root(nullptr), nodesCount(0), allCount(0)
{
	this->insert(key);
}

template<class Key>
inline AVL_K<Key>::~AVL_K()
{
	this->clear();
}

template<class Key>
inline AVL_K<Key>::AVL_K(const AVL_K<Key> & obj) :
	root(copy(obj.root)),
	nodesCount(obj.nodesCount),
	allCount(obj.allCount)
{
	assert(isHeightBalanced());
}

template<class Key>
inline AVL_K<Key> & AVL_K<Key>::operator=(const AVL_K<Key> & obj)
{
	if (this != &obj)
	{
		this->clear();

		this->root = copy(obj.root);
		this->nodesCount = obj.nodesCount;
		this->allCount = obj.allCount;

		assert(isHeightBalanced());
	}

	return *this;
}

template<class Key>
inline void AVL_K<Key>::insert(const Key & key)
{
	++this->allCount;
	Node** stack[AVL_K_MAX_DEPTH];
	stack[0] = &this->root;
	int stackSize = 1;

	while (true)
	{
		Node*& cur = *stack[stackSize - 1];

		if (!cur)
		{
			++this->nodesCount;
			cur = new AVL_K<Key>::Node(key);
			break;
		}

		if (cur->key == key)
		{
			++cur->count;
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

template<class Key>
inline unsigned AVL_K<Key>::removeAll(const Key & key)
{
	--this->nodesCount;
	unsigned result;

	Node** stack[AVL_K_MAX_DEPTH];
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
			result = cur->count;
			this->allCount -= result;

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

template<class Key>
inline bool AVL_K<Key>::remove(const Key & key)
{
	Node** stack[AVL_K_MAX_DEPTH];
	stack[0] = &this->root;
	int stackSize = 1;

	while (true)
	{
		Node*& cur = *stack[stackSize - 1];

		if (!cur)
			return false;

		if (cur->key == key)
		{
			--this->allCount;

			if (--cur->count)
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
			return;
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

template<class Key>
inline bool AVL_K<Key>::find(const Key & key) const
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

template<class Key>
inline unsigned AVL_K<Key>::getSize() const
{
	return this->nodesCount;
}

template<class Key>
inline unsigned AVL_K<Key>::getCount() const
{
	return this->allCount;
}

template<class Key>
inline void AVL_K<Key>::clear()
{
	clear(this->root);
	this->root = nullptr;
	this->nodesCount = 0;
	this->allCount = 0;
}

/////////////////////////////////////////////////////////

template<class Key>
inline void AVL_K<Key>::clear(Node * node)
{
	if (node)
	{
		clear(node->left);
		clear(node->right);
		delete node;
	}
}

template<class Key>
inline char AVL_K<Key>::getHeight(Node * node) const
{
	if (!node)
		return 0;

	return node->height;
}

template<class Key>
inline void AVL_K<Key>::rightRotation(Node *& father)
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

template<class Key>
inline void AVL_K<Key>::leftRotation(Node *& father)
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

template<class Key>
inline bool AVL_K<Key>::isWeightBalanced() const
{
	bool ans = true;
	isWeightBalanced(this->root, ans);

	return ans;
}

template<class Key>
inline bool AVL_K<Key>::isHeightBalanced() const
{
	bool ans = true;
	isHeightBalanced(this->root, ans);

	return ans;
}

template<class Key>
inline unsigned AVL_K<Key>::isWeightBalanced(Node * node, bool& result) const
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

template<class Key>
inline unsigned AVL_K<Key>::isHeightBalanced(Node * node, bool& result) const
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

//////////////////////////////////////////////////////////////

#include <string>

#define MOD_STRING 1000000009u
#define BASE_STRING 128u

static unsigned string_hash(const std::string& str)
{
	unsigned ans = 0;

	for (char i : str)
		ans = ((ans*MOD_STRING) + i) % BASE_STRING;

	return ans;
}

template <>
struct AVL_K<std::string>::Node
{
	Node(const std::pair<unsigned, std::string>& k) :
		key(k), count(1), height(1),
		left(nullptr), right(nullptr)
	{}

	Node(const std::pair<unsigned, std::string>& k, 
		unsigned cnt, Node* l, Node* r, char h = 1) :
		key(k), count(cnt), left(l), right(r),
		height(h)
	{}

	std::pair<unsigned, std::string> key;

	unsigned count;

	Node* left;
	Node* right;

	char height;
};

template <>
inline void AVL_K<std::string>::insert(const std::string& Key)
{
	std::pair<unsigned, std::string> key(string_hash(Key), Key);

	++this->allCount;
	Node** stack[AVL_K_MAX_DEPTH];
	stack[0] = &this->root;
	int stackSize = 1;

	while (true)
	{
		Node*& cur = *stack[stackSize - 1];

		if (!cur)
		{
			++this->nodesCount;
			cur = new AVL_K<std::string>::Node(key);
			break;
		}

		if (cur->key == key)
		{
			++cur->count;
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

template<>
inline unsigned AVL_K<std::string>::removeAll(const std::string & Key)
{
	std::pair<unsigned, std::string> key(string_hash(Key), Key);

	--this->nodesCount;
	unsigned result;

	Node** stack[AVL_K_MAX_DEPTH];
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
			result = cur->count;
			this->allCount -= result;

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


template <>
inline bool AVL_K<std::string>::remove(const std::string & Key)
{
	std::pair<unsigned, std::string> key(string_hash(Key), Key);

	Node** stack[AVL_K_MAX_DEPTH];
	stack[0] = &this->root;
	int stackSize = 1;

	while (true)
	{
		Node*& cur = *stack[stackSize - 1];

		if (!cur)
			return false;

		if (cur->key == key)
		{
			--this->allCount;

			if (--cur->count)
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
			return true;
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

template<>
inline bool AVL_K<std::string>::find(const std::string & Key) const
{
	std::pair<unsigned, std::string> key(string_hash(Key), Key);

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
