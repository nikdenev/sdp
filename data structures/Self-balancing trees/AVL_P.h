#pragma once

#include <cassert>

template <class Key>
class AVL2
{
private:
	struct Node
	{
		Node(char height) :		//for the nil node
			height(height)
		{
			left = right = parent = this;
		}
		Node(Node* l, Node* r, Node* p, const Key& key, char height) :
			left(l), right(r), parent(p), key(key), height(height)
		{}

		Key key;
		Node *left, *right, *parent;
		char height;
	};

public:
	AVL2();
	~AVL2();
	AVL2(const AVL2<Key>& other);
	AVL2<Key>& operator=(const AVL2<Key>& other);

	bool search(const Key& key) const;

	void insert(const Key& key);

	bool remove(const Key& key);

	size_t getSize() const;

	void clear();

private:
	void remove(Node* node);

	void insertFixUp(Node* node);
	void removeFixUp(Node* node);

	void leftRotation(Node* node);
	void rightRotation(Node* node);

	void leftRightRotation(Node* node);
	void rightLeftRotation(Node* node);

private:
	void copy(const AVL2& other);

	void clear(Node* node);

private:
	bool isHeightBalanced() const;
	unsigned isHeightBalanced(Node* node, bool& result) const;

private:
	Node* nil;
	Node*& root;
	size_t size;

private:

	Node* extractMin(Node* node, Node*& cur)
	{
		cur = node;
		while (cur->left != nil)
			cur = cur->left;

		if (cur == node)
		{
			node->parent->right = cur->right;
			cur = cur->right;
		}
		else
		{
			cur->right->parent = cur->parent;
			cur->parent->left = cur->right;

			node = cur;
			cur = cur->right;
		}

		return node;
	}

	Node* copy(Node* otherNode, const AVL2& other)
	{
		if (otherNode == other.nil)
			return nil;

		Node *l, *r, *node;

		node = new Node(l = copy(otherNode->left, other),
			r = copy(otherNode->right, other),
			nil, otherNode->key, otherNode->height);

		l->parent = r->parent = node;

		return node;
	}

};

//nil's left child is the root for easier root rotations
template<class Key>
inline AVL2<Key>::AVL2() :
	nil(new Node(0)),
	root(nil->left),
	size(0)
{

}

template<class Key>
inline AVL2<Key>::~AVL2()
{
	clear();
	delete nil;
}

template<class Key>
inline AVL2<Key>::AVL2(const AVL2<Key> & other) :
	nil(new Node(0)),
	root(nil->left),
	size(0)
{
	copy(other);
}

template<class Key>
inline AVL2<Key> & AVL2<Key>::operator=(const AVL2<Key> & other)
{
	if (this != &other)
	{
		clear();
		copy(other);
	}

	return *this;
}

template<class Key>
inline bool AVL2<Key>::search(const Key & key) const
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
inline void AVL2<Key>::insert(const Key & key)
{
	if (size++ == 0)
	{
		root = new Node(nil, nil, nil, key, false);
		return;
	}

	Node *prev, *cur = root;
	while (cur != nil)
	{
		prev = cur;
		cur = cur->key < key ? cur->right : cur->left;
	}

	if (prev->key < key)
		prev->right = new Node(nil, nil, prev, key, true);
	else
		prev->left = new Node(nil, nil, prev, key, true);

	insertFixUp(prev);

	assert(isHeightBalanced());
}

template<class Key>
inline bool AVL2<Key>::remove(const Key & key)
{
	Node* cur = root;

	while (cur != nil)
	{
		if (cur->key == key)
		{
			remove(cur);
			--size;

			assert(isHeightBalanced());
			return true;
		}

		cur = cur->key < key ? cur->right : cur->left;
	}

	return false;
}

template<class Key>
inline size_t AVL2<Key>::getSize() const
{
	return size;
}

template<class Key>
inline void AVL2<Key>::clear()
{
	clear(root);
	size = 0;
}

template<class Key>
inline void AVL2<Key>::remove(Node * node)
{
	Node* cur;
	if (node->left != nil && node->right != nil)
	{
		Node* min = extractMin(node->right, cur);

		min->left = node->left;
		node->left->parent = min;

		min->right = node->right;
		node->right->parent = min;

		min->parent = node->parent;
		if (node->parent->left == node)
			node->parent->left = min;
		else
			node->parent->right = min;

		min->height = node->height;
	}
	else if (node->left != nil)
	{
		if (node->parent->left == node)
			cur = node->parent->left = node->left;
		else
			cur = node->parent->right = node->left;

		node->left->parent = node->parent;
	}
	else
	{
		if (node->parent->left == node)
			cur = node->parent->left = node->right;
		else
			cur = node->parent->right = node->right;

		node->right->parent = node->parent;
	}

	delete node;

	removeFixUp(cur->parent);
}

template<class Key>
inline void AVL2<Key>::insertFixUp(Node * node)
{
	while (node != nil)
	{
		node->height = 1 + std::max(node->left->height, node->right->height);

		char difference = node->right->height - node->left->height;
		if (difference == 0)
			break;

		if (difference > 1)
		{
			if (node->right->right->height - node->right->left->height > 0)
				leftRotation(node);
			else
				rightLeftRotation(node);

			break;
		}
		else if (difference < -1)
		{
			if (node->left->right->height - node->left->left->height > 0)
				leftRightRotation(node);
			else
				rightRotation(node);

			break;
		}

		node = node->parent;
	}
}

template<class Key>
inline void AVL2<Key>::removeFixUp(Node * node)
{
	while (node != nil)
	{
		node->height = 1 + 
			std::max(node->left->height, node->right->height);

		char difference = node->right->height - node->left->height;
		if (difference == 1 || difference == -1)
			break;

		if (difference > 1)
		{
			if (node->right->right->height - node->right->left->height < 0)
				rightLeftRotation(node);
			else
				leftRotation(node);
		}
		else if (difference < -1)
		{
			if (node->left->right->height - node->left->left->height > 0)
				leftRightRotation(node);
			else
				rightRotation(node);
		}

		node = node->parent;
	}
}

template<class Key>
inline void AVL2<Key>::leftRotation(Node * node)
{
	//this also works for the root because it is nil's left child
	if (node == node->parent->left)
		node->parent->left = node->right;
	else
		node->parent->right = node->right;

	node->right->parent = node->parent;
	node->parent = node->right;
	node->right->left->parent = node;
	node->right = node->right->left;
	node->parent->left = node;

	node->height = 1 +
		std::max(node->left->height, node->right->height);

	node->parent->height = 1 +
		std::max(node->parent->right->height, node->height);
}

template<class Key>
inline void AVL2<Key>::rightRotation(Node * node)
{
	//this also works for the root because it is nil's left child
	if (node == node->parent->left)
		node->parent->left = node->left;
	else
		node->parent->right = node->left;

	node->left->parent = node->parent;
	node->parent = node->left;
	node->left->right->parent = node;
	node->left = node->left->right;
	node->parent->right = node;

	node->height = 1 +
		std::max(node->left->height, node->right->height);

	node->parent->height = 1 + 
		std::max(node->parent->left->height, node->height);
}

template<class Key>
inline void AVL2<Key>::leftRightRotation(Node * node)
{
	Node* temp1 = node;
	Node* temp2 = node->left;
	node = temp2->right;

	if (temp1 == temp1->parent->left)
		temp1->parent->left = node;
	else
		temp1->parent->right = node;
	node->parent = temp1->parent;

	temp1->left = node->right;
	node->right->parent = temp1;

	temp2->right = node->left;
	node->left->parent = temp2;

	node->left = temp2;
	temp2->parent = node;

	node->right = temp1;
	temp1->parent = node;

	temp1->height = 1 +
		std::max(temp1->left->height, temp1->right->height);

	temp2->height = 1 +
		std::max(temp2->left->height, temp2->right->height);

	node->height = 1 +
		std::max(temp1->height, temp2->height);
}

template<class Key>
inline void AVL2<Key>::rightLeftRotation(Node * node)
{
	Node* temp1 = node;
	Node* temp2 = node->right;
	node = temp2->left;

	if (temp1 == temp1->parent->left)
		temp1->parent->left = node;
	else
		temp1->parent->right = node;
	node->parent = temp1->parent;

	temp1->right = node->left;
	node->left->parent = temp1;

	temp2->left = node->right;
	node->right->parent = temp2;

	node->right = temp2;
	temp2->parent = node;

	node->left = temp1;
	temp1->parent = node;

	temp1->height = 1 +
		std::max(temp1->left->height, temp1->right->height);

	temp2->height = 1 +
		std::max(temp2->left->height, temp2->right->height);

	node->height = 1 +
		std::max(temp1->height, temp2->height);
}

template<class Key>
inline void AVL2<Key>::copy(const AVL2<Key> & other)
{
	root = copy(other.root, other);
	size = other.size;
}

template<class Key>
inline void AVL2<Key>::clear(Node * node)
{
	if (node != nil)
	{
		clear(node->left);
		clear(node->right);
		delete node;
	}
}


template<class Key>
inline bool AVL2<Key>::isHeightBalanced() const
{
	bool ans = true;
	isHeightBalanced(this->root, ans);

	return ans;
}

template<class Key>
inline unsigned AVL2<Key>::isHeightBalanced(Node * node, bool& result) const
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