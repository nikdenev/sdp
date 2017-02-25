#pragma once

#include <cassert>

#define RBT2_MAX_DEPTH 80

template <class Key>
class RBT2
{
private:
	struct Node
	{
		Node() :		//for the nil
			isRed(false)
		{
			left = right = this;
		}
		Node(const Key& k, Node* l, Node* r, bool isRed) :
			key(k), left(l), right(r), isRed(isRed)
		{}

		Key key;
		Node* left, *right;
		bool isRed;
	};

public:
	RBT2();
	~RBT2();
	RBT2(const RBT2<Key>& other);
	RBT2<Key>& operator= (const RBT2<Key>& other);

	void insert(const Key& key);

	bool remove(const Key& key);

	bool search(const Key& key) const;

	unsigned getSize() const;

	void clear();

private:
	void clear(Node* node);

	bool remove(Node*& node, Node*** stack, int& stackSize);

	void rightRotation(Node*& father);
	void leftRotation(Node*& father);

	void leftRightRotation(Node*& father);
	void rightLeftRotation(Node*& father);

	void insertFixUp(Node*** stack, int stackSize);
	void removeFixUp(Node*** stack, int stackSize);

	bool isValid();
	int checkBlackHeight(const Node* node, bool& result);
	bool everyRedHasBlackChild(const Node* node);

private:
	Node* root;
	Node* nil;
	size_t size;

private:
	Node* copy(Node* otherRoot, const RBT2<Key>& other)
	{
		if (otherRoot == other.nil)
			return nil;

		return new Node(otherRoot->key,
			copy(otherRoot->left, other),
			copy(otherRoot->right, other),
			otherRoot->isRed);
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
		stack[stackSize++] = node;		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		return temp;
	}
};

template<class Key>
inline RBT2<Key>::RBT2() :
	size(0)
{
	root = nil = new Node;
}

template<class Key>
inline RBT2<Key>::~RBT2()
{
	clear();
	delete nil;
}

template<class Key>
inline RBT2<Key>::RBT2(const RBT2<Key> & other) :
	size(other.size)
{
	nil = new Node;
	root = copy(other.root, other);

	assert(isValid());
}

template<class Key>
inline RBT2<Key> & RBT2<Key>::operator=(const RBT2<Key> & other)
{
	if (this != &other)
	{
		clear();

		root = copy(other.root, other);
		size = other.size;

		assert(isValid());
	}

	return *this;
}

template<class Key>
inline void RBT2<Key>::insert(const Key & key)
{
	++size;
	Node** stack[RBT2_MAX_DEPTH];
	stack[0] = &root;
	int stackSize = 1;

	while (true)
	{
		Node*& cur = *stack[stackSize - 1];

		if (cur == nil)
		{
			cur = new Node(key, nil, nil, true);

			insertFixUp(stack, stackSize);
			assert(isValid());

			return;
		}

		if (cur->key < key)
			stack[stackSize++] = &cur->right;
		else
			stack[stackSize++] = &cur->left;
	}
}

template<class Key>
inline bool RBT2<Key>::remove(const Key & key)
{
	Node** stack[RBT2_MAX_DEPTH];
	stack[0] = &root;
	int stackSize = 1;

	while (true)
	{
		Node*& cur = *stack[stackSize - 1];

		if (cur == nil)
			return false;

		if (cur->key == key)
		{
			if (!remove(cur, stack, stackSize))		//the removed node was black
				removeFixUp(stack, stackSize);

			--size;
			assert(isValid());

			return true;
		}

		if (cur->key < key)
			stack[stackSize++] = &cur->right;
		else
			stack[stackSize++] = &cur->left;
	}
}

template<class Key>
inline bool RBT2<Key>::search(const Key & key) const
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
inline unsigned RBT2<Key>::getSize() const
{
	return size;
}

template<class Key>
inline void RBT2<Key>::clear()
{
	clear(root);
	root = nil;
	size = 0;
}

/////////////////////////////////////////////////////////

template<class Key>
inline void RBT2<Key>::clear(Node * node)
{
	if (node != nil)
	{
		clear(node->left);
		clear(node->right);
		delete node;
	}
}

template<class Key>
inline bool RBT2<Key>::remove(Node *& node, Node *** stack, int & stackSize)
{
	bool removedColor = node->isRed;
	Node* toDel = node;
	if (node->left != nil && node->right != nil)
	{
		int oldStackSize = stackSize;

		Node* min = extractMin(&node->right, stack, stackSize);
		removedColor = min->isRed;

		min->right = node->right;
		min->left = node->left;
		min->isRed = node->isRed;

		node = min;

		//because the old pointer was from the node which is deleted
		stack[oldStackSize] = &node->right;
	}
	else if (node->left != nil)
	{
		node = node->left;
		stack[stackSize - 1] = &node;
	}
	else /* has only right child or doesn't have child */
	{
		 node = node->right;
		 stack[stackSize - 1] = &node;
	}

	delete toDel;

	return removedColor;
}

template<class Key>
inline void RBT2<Key>::rightRotation(Node *& father)
{
	Node* temp = father;
	father = father->left;
	temp->left = father->right;
	father->right = temp;
}

template<class Key>
inline void RBT2<Key>::leftRotation(Node *& father)
{
	Node* temp = father;
	father = father->right;
	temp->right = father->left;
	father->left = temp;
}

template<class Key>
inline void RBT2<Key>::leftRightRotation(Node *& father)
{
	Node* temp1 = father;
	Node* temp2 = father->left;
	father = temp2->right;

	temp1->left = father->right;
	temp2->right = father->left;

	father->left = temp2;
	father->right = temp1;
}

template<class Key>
inline void RBT2<Key>::rightLeftRotation(Node *& father)
{
	Node* temp1 = father;
	Node* temp2 = father->right;
	father = temp2->left;

	temp1->right = father->left;
	temp2->left = father->right;

	father->right = temp2;
	father->left = temp1;
}

template<class Key>
inline void RBT2<Key>::insertFixUp(Node *** stack, int stackSize)
{
	while (stackSize > 2 && (*stack[stackSize - 2])->isRed)
	{
		Node* node = *stack[stackSize - 1];
		Node*& nodeParent = *stack[stackSize - 2];
		Node*& nodeParentParent = *stack[stackSize - 3];

		if (nodeParent == nodeParentParent->left)
		{
			Node* uncle = nodeParentParent->right;

			if (uncle->isRed)	//just recolor and going higher
			{
				uncle->isRed = nodeParent->isRed = false;
				nodeParentParent->isRed = true;
				stackSize -= 2;
			}
			else
			{
				if (nodeParent->right == node)
					leftRotation(nodeParent);

				nodeParentParent->isRed = true;
				nodeParent->isRed = false;
				rightRotation(nodeParentParent);

				return;
			}
		}
		else	//symmetric
		{
			Node* uncle = nodeParentParent->left;

			if (uncle->isRed)
			{
				uncle->isRed = nodeParent->isRed = false;
				nodeParentParent->isRed = true;
				stackSize -= 2;
			}
			else
			{
				if (nodeParent->left == node)
					rightRotation(nodeParent);

				nodeParentParent->isRed = true;
				nodeParent->isRed = false;
				leftRotation(nodeParentParent);

				return;
			}
		}
	}

	root->isRed = false;
}

template<class Key>
inline void RBT2<Key>::removeFixUp(Node *** stack, int stackSize)
{
	Node* node;
	while (!(node = *stack[stackSize - 1])->isRed && stackSize > 1)
	{
		Node** nodeParent = stack[stackSize - 2];

		if (node == (*nodeParent)->left)
		{
			Node** brother = &(*nodeParent)->right;

			if ((*brother)->isRed)
			{
				(*brother)->isRed = false;
				(*nodeParent)->isRed = true;
				leftRotation(*nodeParent);

				nodeParent = &(*nodeParent)->left;
				brother = &(*nodeParent)->right;

				// :)	because after the rotation the pointer 
				//has changed we need to restore it
				//after this statment the loop terminates and because of
				//that the path back to the root doesn't matter
				stack[stackSize - 2] = nodeParent;
			}

			if (!(*brother)->left->isRed && !(*brother)->right->isRed)
			{
				(*brother)->isRed = true;
				--stackSize;
			}
			else
			{
				if (!(*brother)->right->isRed)
				{
					(*brother)->isRed = true;
					(*brother)->left->isRed = false;
					rightRotation(*brother);
				}

				(*brother)->isRed = (*nodeParent)->isRed;
				(*nodeParent)->isRed = false;
				(*brother)->right->isRed = false;
				leftRotation(*nodeParent);

				return;
			}
		}
		else	//symmetric
		{
			Node** brother = &(*nodeParent)->left;

			if ((*brother)->isRed)
			{
				(*brother)->isRed = false;
				(*nodeParent)->isRed = true;
				rightRotation(*nodeParent);

				nodeParent = &(*nodeParent)->right;
				brother = &(*nodeParent)->left;

				stack[stackSize - 2] = nodeParent;
			}

			if (!(*brother)->right->isRed && !(*brother)->left->isRed)
			{
				(*brother)->isRed = true;
				--stackSize;
			}
			else
			{
				if (!(*brother)->left->isRed)
				{
					(*brother)->isRed = true;
					(*brother)->right->isRed = false;
					leftRotation(*brother);
				}

				(*brother)->isRed = (*nodeParent)->isRed;
				(*nodeParent)->isRed = false;
				(*brother)->left->isRed = false;
				rightRotation(*nodeParent);

				return;
			}
		}
	}

	node->isRed = false;
}

template<class Key>
inline bool RBT2<Key>::isValid()
{
	if (root->isRed)
		return false;

	bool result = true;
	checkBlackHeight(root, result);

	return result && everyRedHasBlackChild(root);
}

template<class Key>
inline int RBT2<Key>::checkBlackHeight(const Node * node, bool& result)
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
inline bool RBT2<Key>::everyRedHasBlackChild(const Node * node)
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