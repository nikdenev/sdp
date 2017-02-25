#pragma once

#include <algorithm>
#include <cassert>

#define AVL_MAX_DEPTH 64

template <class Key>
class AVL
{
private:
	struct Node
	{
		Node() :		//for the nil
			height(0)
		{}
		Node(const Key& k, Node* l, Node* r, char h) :
			key(k), left(l), right(r), height(h)
		{}

		Key key;
		Node* left, *right;
		char height;
	};

public:
	AVL();
	~AVL();
	AVL(const AVL<Key>& other);
	AVL<Key>& operator= (const AVL<Key>& other);

	void insert(const Key& key);

	bool remove(const Key& key);

	bool search(const Key& key) const;

	unsigned getSize() const;

	void clear();

private:
	void clear(Node* node);

	void remove(Node*& node, Node*** stack, int& stackSize);

	void rightRotation(Node*& father);
	void leftRotation(Node*& father);

	void leftRightRotation(Node*& father);
	void rightLeftRotation(Node*& father);

	void insertFixUp(Node*** stack, int stackSize);
	void removeFixUp(Node*** stack, int stackSize);

	bool isHeightBalanced() const;
	unsigned isHeightBalanced(Node* node, bool& result) const;

private:
	Node* root;
	Node* nil;
	size_t size;

private:
	Node* copy(Node* otherRoot, const AVL<Key>& other)
	{
		if (otherRoot == other.nil)
			return nil;

		return new Node(otherRoot->key,
			copy(otherRoot->left, other),
			copy(otherRoot->right, other),
			otherRoot->height);
	}

	Node* extractMin(Node** node, Node** stack[], int& stackSize)
	{
		while ((*node)->left != nil)
		{
			stack[stackSize++] = node;
			node = &(*node)->left;
		}

		Node* temp = *node;
		*node = (*node)->right;

		return temp;
	}
};

template<class Key>
inline AVL<Key>::AVL() :
	size(0)
{
	root = nil = new Node;
}

template<class Key>
inline AVL<Key>::~AVL()
{
	clear();
	delete nil;
}

template<class Key>
inline AVL<Key>::AVL(const AVL<Key> & other) :
	size(other.size)
{
	nil = new Node;
	root = copy(other.root, other);

	assert(isHeightBalanced());
}

template<class Key>
inline AVL<Key> & AVL<Key>::operator=(const AVL<Key> & other)
{
	if (this != &other)
	{
		clear();

		root = copy(other.root, other);
		size = other.size;

		assert(isHeightBalanced());
	}

	return *this;
}

template<class Key>
inline void AVL<Key>::insert(const Key & key)
{
	++size;
	Node** stack[AVL_MAX_DEPTH];
	stack[0] = &root;
	int stackSize = 1;

	while (true)
	{
		Node*& cur = *stack[stackSize - 1];

		if (cur == nil)
		{
			cur = new Node(key, nil, nil, 1);

			insertFixUp(stack, stackSize - 1);
			assert(isHeightBalanced());

			return;
		}

		if (cur->key < key)
			stack[stackSize++] = &cur->right;
		else
			stack[stackSize++] = &cur->left;
	}
}

template<class Key>
inline bool AVL<Key>::remove(const Key & key)
{
	Node** stack[AVL_MAX_DEPTH];
	stack[0] = &root;
	int stackSize = 1;

	while (true)
	{
		Node*& cur = *stack[stackSize - 1];

		if (cur == nil)
			return false;

		if (cur->key == key)
		{
			remove(cur, stack, stackSize);
			--size;

			removeFixUp(stack, stackSize);
			assert(isHeightBalanced());

			return true;
		}

		if (cur->key < key)
			stack[stackSize++] = &cur->right;
		else
			stack[stackSize++] = &cur->left;
	}
}

template<class Key>
inline bool AVL<Key>::search(const Key & key) const
{
	Node* cur = root;

	while (cur != nil)
	{
		if (cur->key == key)
			return true;

		cur = cur->key < key ? cur->right : cur->left;
	}

	return false;
}

template<class Key>
inline unsigned AVL<Key>::getSize() const
{
	return size;
}

template<class Key>
inline void AVL<Key>::clear()
{
	clear(root);
	root = nil;
	size = 0;
}

/////////////////////////////////////////////////////////

template<class Key>
inline void AVL<Key>::clear(Node * node)
{
	if (node != nil)
	{
		clear(node->left);
		clear(node->right);
		delete node;
	}
}

template<class Key>
inline void AVL<Key>::remove(Node *& node, Node *** stack, int & stackSize)
{
	Node* toDel = node;
	if (node->left != nil && node->right != nil)
	{
		int oldStackSize = stackSize;

		Node* min = extractMin(&node->right, stack, stackSize);
		min->right = node->right;
		min->left = node->left;
		min->height = node->height;

		node = min;

		stack[oldStackSize] = &node->right;
	}
	else if (node->left != nil)
	{
		--stackSize;
		node = node->left;
	}
	else /* has only right child or doesn't have child */
	{
		--stackSize;
		node = node->right;
	}

	delete toDel;
}

template<class Key>
inline void AVL<Key>::rightRotation(Node *& father)
{
	Node* temp = father;
	father = father->left;
	temp->left = father->right;
	father->right = temp;

	temp->height = 1 +
		std::max(temp->left->height, temp->right->height);

	father->height = 1 +
		std::max(father->left->height, temp->height);
}

template<class Key>
inline void AVL<Key>::leftRotation(Node *& father)
{
	Node* temp = father;
	father = father->right;
	temp->right = father->left;
	father->left = temp;

	temp->height = 1 +
		std::max(temp->left->height, temp->right->height);

	father->height = 1 +
		std::max(temp->height, father->right->height);
}

template<class Key>
inline void AVL<Key>::leftRightRotation(Node *& father)
{
	Node* temp1 = father;
	Node* temp2 = father->left;
	father = temp2->right;

	temp1->left = father->right;
	temp2->right = father->left;

	father->left = temp2;
	father->right = temp1;

	temp1->height = 1 +
		std::max(temp1->left->height, temp1->right->height);

	temp2->height = 1 +
		std::max(temp2->left->height, temp2->right->height);

	father->height = 1 +
		std::max(temp1->height, temp2->height);
}

template<class Key>
inline void AVL<Key>::rightLeftRotation(Node *& father)
{
	Node* temp1 = father;
	Node* temp2 = father->right;
	father = temp2->left;

	temp1->right = father->left;
	temp2->left = father->right;

	father->right = temp2;
	father->left = temp1;

	temp1->height = 1 +
		std::max(temp1->left->height, temp1->right->height);

	temp2->height = 1 +
		std::max(temp2->left->height, temp2->right->height);

	father->height = 1 +
		std::max(temp1->height, temp2->height);
}

template<class Key>
inline void AVL<Key>::insertFixUp(Node *** stack, int stackSize)
{
	while (stackSize)
	{
		Node*& cur = *stack[stackSize - 1];

		cur->height = 1 + std::max(cur->left->height, cur->right->height);

		char difference = cur->right->height - cur->left->height;
		if (difference == 0)
			break;

		if (difference > 1)
		{
			if (cur->right->right->height - cur->right->left->height > 0)
				leftRotation(cur);
			else
				rightLeftRotation(cur);

			break;
		}
		else if (difference < -1)
		{
			if (cur->left->right->height - cur->left->left->height > 0)
				leftRightRotation(cur);
			else
				rightRotation(cur);

			break;
		}

		--stackSize;
	}
}

template<class Key>
inline void AVL<Key>::removeFixUp(Node *** stack, int stackSize)
{
	while (stackSize)
	{
		Node*& cur = *stack[stackSize - 1];

		cur->height = 1 + std::max(cur->left->height, cur->right->height);

		char difference = cur->right->height - cur->left->height;
		if (difference == 1 || difference == -1)
			break;

		if (difference > 1)
		{
			if (cur->right->right->height - cur->right->left->height < 0)
				rightLeftRotation(cur);
			else
				leftRotation(cur);
		}
		else if (difference < -1)
		{
			if (cur->left->right->height - cur->left->left->height > 0)
				leftRightRotation(cur);
			else
				rightRotation(cur);
		}

		--stackSize;
	}
}

template<class Key>
inline bool AVL<Key>::isHeightBalanced() const
{
	bool ans = true;
	isHeightBalanced(root, ans);

	return ans;
}

template<class Key>
inline unsigned AVL<Key>::isHeightBalanced(Node * node, bool& result) const
{
	if (node == nil)
		return 0;

	char hl = isHeightBalanced(node->left, result);

	if (!result)
		return 0;

	assert(hl == node->left->height);

	char hr = isHeightBalanced(node->right, result);

	if (!result)
		return 0;

	assert(hr == node->right->height);

	if (abs(hl - hr) > 1)
		result = false;

	return 1 + std::max(hl, hr);
}