#pragma once

#include <cassert>

template <class Key>
class RBT
{
private:
	struct Node
	{
		Node(bool isRed) :		//for the nil node
			isRed(isRed)
		{
			left = right = parent = this;
		}
		Node(Node* l, Node* r, Node* p, const Key& key, bool isRed = true) :
			left(l), right(r), parent(p), key(key), isRed(isRed)
		{}

		Node *left, *right, *parent;
		Key key;
		bool isRed;
	};

public:
	RBT();
	~RBT();
	RBT(const RBT<Key>& other);
	RBT<Key>& operator=(const RBT<Key>& other);

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

private:
	void copy(const RBT& other);

	void clear(Node* node);

private:
	bool isValid();

	int checkBlackHeight(const Node* node, bool& result);

	bool everyRedHasBlackChild(const Node* node);

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

	Node* copy(Node* otherNode, const RBT& other)
	{
		if (otherNode == other.nil)
			return nil;

		Node *l, *r, *node;

		node = new Node(l = copy(otherNode->left, other),
			r = copy(otherNode->right, other),
			nil, otherNode->key, otherNode->isRed);

		l->parent = r->parent = node;

		return node;
	}
};

//nil's left child is the root for easier root rotations
template<class Key>
inline RBT<Key>::RBT() :
	nil(new Node(false)),
	root(nil->left),
	size(0)
{
	
}

template<class Key>
inline RBT<Key>::~RBT()
{
	clear();
	delete nil;
}

template<class Key>
inline RBT<Key>::RBT(const RBT<Key> & other) :
	nil(new Node(false)),
	root(nil->left),
	size(0)
{
	copy(other);
}

template<class Key>
inline RBT<Key> & RBT<Key>::operator=(const RBT<Key> & other)
{
	if (this != &other)
	{
		clear();
		copy(other);
	}

	return *this;
}

template<class Key>
inline bool RBT<Key>::search(const Key & key) const
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
inline void RBT<Key>::insert(const Key & key)
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
	{
		prev->right = new Node(nil, nil, prev, key, true);
		insertFixUp(prev->right);
	}
	else
	{
		prev->left = new Node(nil, nil, prev, key, true);
		insertFixUp(prev->left);
	}

	assert(isValid());
}

template<class Key>
inline bool RBT<Key>::remove(const Key & key)
{
	Node* cur = root;

	while (cur != nil)
	{
		if (cur->key == key)
		{
			remove(cur);
			--size;

			assert(isValid());
			return true;
		}

		cur = cur->key < key ? cur->right : cur->left;
	}

	return false;
}

template<class Key>
inline size_t RBT<Key>::getSize() const
{
	return size;
}

template<class Key>
inline void RBT<Key>::clear()
{
	clear(root);
	size = 0;
}

template<class Key>
inline void RBT<Key>::remove(Node * node)
{
	bool removedColor = node->isRed;
	Node* cur;
	if (node->left != nil && node->right != nil)
	{
		Node* min = extractMin(node->right, cur);
		removedColor = min->isRed;

		min->left = node->left;
		node->left->parent = min;

		min->right = node->right;
		node->right->parent = min;

		min->parent = node->parent;
		if (node->parent->left == node)
			node->parent->left = min;
		else
			node->parent->right = min;

		min->isRed = node->isRed;
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

	if (!removedColor)
		removeFixUp(cur);
}

template<class Key>
inline void RBT<Key>::insertFixUp(Node * node)
{
	Node *uncle, *nodeParent;
	while ((nodeParent = node->parent)->isRed)
	{
		if (nodeParent == nodeParent->parent->left)
		{
			uncle = nodeParent->parent->right;

			if (uncle->isRed)	//just recolor and going higher
			{
				uncle->isRed = nodeParent->isRed = false;
				uncle->parent->isRed = true;
				node = uncle->parent;
			}
			else
			{
				if (nodeParent->right == node)		
				{
					leftRotation(nodeParent);
					nodeParent = node;
				}

				nodeParent->parent->isRed = true;
				nodeParent->isRed = false;
				rightRotation(nodeParent->parent);

				return;
			}
		}
		else	//symmetric
		{
			uncle = nodeParent->parent->left;

			if (uncle->isRed)
			{
				uncle->isRed = nodeParent->isRed = false;
				uncle->parent->isRed = true;
				node = uncle->parent;
			}
			else
			{
				if (nodeParent->left == node)
				{
					rightRotation(nodeParent);
					nodeParent = node;
				}

				nodeParent->parent->isRed = true;
				nodeParent->isRed = false;
				leftRotation(nodeParent->parent);

				return;
			}
		}
	}

	root->isRed = false;
}

template<class Key>
inline void RBT<Key>::removeFixUp(Node * node)
{
	Node* brother;
	while (!node->isRed && node != root)
	{
		if (node == node->parent->left)
		{
			brother = node->parent->right;

			if (brother->isRed)
			{
				brother->isRed = false;
				brother->parent->isRed = true;
				leftRotation(brother->parent);

				brother = node->parent->right;
			}

			if (!brother->left->isRed && !brother->right->isRed)
			{
				brother->isRed = true;
				node = node->parent;
			}
			else
			{
				if (!brother->right->isRed)
				{
					brother->isRed = true;
					brother->left->isRed = false;
					rightRotation(brother);

					brother = brother->parent;
				}

				brother->isRed = brother->parent->isRed;
				brother->parent->isRed = false;
				brother->right->isRed = false;
				leftRotation(brother->parent);

				return;
			}
		}
		else	//symmetric
		{
			brother = node->parent->left;

			if (brother->isRed)
			{
				brother->isRed = false;
				brother->parent->isRed = true;
				rightRotation(brother->parent);

				brother = node->parent->left;
			}

			if (!brother->left->isRed && !brother->right->isRed)
			{
				brother->isRed = true;
				node = node->parent;
			}
			else
			{
				if (!brother->left->isRed)
				{
					brother->isRed = true;
					brother->right->isRed = false;
					leftRotation(brother);

					brother = brother->parent;
				}

				brother->isRed = brother->parent->isRed;
				brother->parent->isRed = false;
				brother->left->isRed = false;
				rightRotation(brother->parent);

				return;
			}
		}
	}

	node->isRed = false;
}

template<class Key>
inline void RBT<Key>::rightRotation(Node * node)
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
	
}

template<class Key>
inline void RBT<Key>::leftRotation(Node * node)
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
}

template<class Key>
inline void RBT<Key>::copy(const RBT<Key> & other)
{
	root = copy(other.root, other);
	size = other.size;
}

template<class Key>
inline void RBT<Key>::clear(Node * node)
{
	if (node != nil)
	{
		clear(node->left);
		clear(node->right);
		delete node;
	}
}

template<class Key>
inline bool RBT<Key>::isValid()
{
	if (root->isRed)
		return false;

	bool result = true;
	checkBlackHeight(root, result);

	return result && everyRedHasBlackChild(root);
}

template<class Key>
inline int RBT<Key>::checkBlackHeight(const Node * node, bool& result)
{
	if (node == nil)
		return 1;

	int leftHeight = checkBlackHeight(node->left, result);
	int rightHeight = checkBlackHeight(node->right, result);

	if (leftHeight != rightHeight)
		result = false;

	return leftHeight + (node->isRed ? 0 : 1);
}

template<class Key>
inline bool RBT<Key>::everyRedHasBlackChild(const Node * node)
{
	if (node == nil)
		return true;

	if (node->isRed)
		return !node->right->isRed && !node->left->isRed &&
			everyRedHasBlackChild(node->left) &&
			everyRedHasBlackChild(node->right);

	return everyRedHasBlackChild(node->left) &&
		everyRedHasBlackChild(node->right);
}
