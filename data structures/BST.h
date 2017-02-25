#pragma once

#include <vector>
#include <utility>

template <class Key>
class BST
{
private:
	struct Node
	{
		Node(const Key key) :
			key(key),
			cnt(1u),
			left(nullptr),
			right(nullptr)
		{}
		Node(const Key key, const size_t cnt) :
			key(key),
			cnt(cnt),
			left(nullptr),
			right(nullptr)
		{}
		Node(const Key key, const size_t cnt, Node* left, Node* right) :
			key(key),
			cnt(cnt),
			left(left),
			right(right)
		{}

		Key key;
		size_t cnt;
		Node *left, *right;
	};

public:
	BST() :
		root(nullptr),
		size(0u)
	{}

	~BST()
	{
		clear(root);
	}

	BST(const BST& other) :
		root(nullptr)
	{
		copy(other);
	}

	BST& operator= (const BST& other)
	{
		if (this != &other)
		{
			clear();
			copy(other);
		}

		return *this;
	}

	void insert(const Key& key)
	{
		++size;
		Node** cur = &root;

		while (*cur)
		{
			if ((*cur)->key == key)
			{
				++(*cur)->cnt;
				return;
			}
			if ((*cur)->key < key)
				cur = &(*cur)->right;
			else
				cur = &(*cur)->left;
		}

		*cur = new Node(key);
	}

	bool remove(const Key& key)
	{
		Node** cur = &root;
		while (*cur)
		{
			if ((*cur)->key == key)
			{
				--size;
				if ((*cur)->cnt > 1u)
					--(*cur)->cnt;
				else
				{
					Node* toDel;
					if ((*cur)->left && (*cur)->right)
					{
						Node* min = extractMin(&(*cur)->right);
						min->left = (*cur)->left;
						min->right = (*cur)->right;
						toDel = *cur;
						*cur = min;
					}
					else if ((*cur)->left)
					{
						toDel = *cur;
						*cur = (*cur)->left;
					}
					else
					{
						toDel = *cur;
						*cur = (*cur)->right;
					}
					delete toDel;
				}
				return true;
			}
			if ((*cur)->key < key)
				cur = &(*cur)->right;
			else
				cur = &(*cur)->left;
		}
		return false;
	}

	bool search(const Key& key) const
	{
		Node* cur = root;

		while (cur)
		{
			if (cur->key == key)
				return true;
			else if (cur->key < key)
				cur = cur->right;
			else
				cur = cur->left;
		}

		return false;
	}

	size_t getSize() const
	{
		return size;
	}

	void clear()
	{
		clear(root);
		root = nullptr;
		size = 0u;
	}

private:
	void clear(Node* node)
	{
		if (!node)
			return;

		std::vector<Node**> stack;
		stack.push_back(&node);

		while (!stack.empty())
		{
			if ((*stack.back())->left)
				stack.push_back(&(*stack.back())->left);
			else if ((*stack.back())->right)
				stack.push_back(&(*stack.back())->right);
			else
			{
				delete *stack.back();
				*stack.back() = nullptr;
				stack.pop_back();
			}
		}
	}

	void copy(const BST& other)
	{
		size = other.size;
		if (!size)
			return;

		root = new Node(other.root->key, other.root->cnt);

		std::vector<Node*> myStack;
		myStack.push_back(root);

		std::vector<std::pair<Node*, int> > otherStack;
		otherStack.push_back(std::make_pair(other.root, 0));

		while (!otherStack.empty())
		{
			std::pair<Node*, int>& cur = otherStack.back();
			if (cur.second == 0)	/* it doesn't have visited children */
			{
				if (cur.first->left)
				{
					++cur.second;
					myStack.push_back(myStack.back()->left =
						new Node(cur.first->left->key, cur.first->left->cnt));

					otherStack.push_back(std::make_pair(cur.first->left, 0));
				}
				else if (cur.first->right)
				{
					cur.second = 2;
					myStack.push_back(myStack.back()->right =
						new Node(cur.first->right->key, cur.first->right->cnt));

					otherStack.push_back(std::make_pair(cur.first->right, 0));
				}
				else
				{
					myStack.pop_back();
					otherStack.pop_back();
				}
			}
			else if (cur.second == 1) /* only the left child is visited */
			{
				if (cur.first->right)
				{
					++cur.second;
					myStack.push_back(myStack.back()->right =
						new Node(cur.first->right->key, cur.first->right->cnt));

					otherStack.push_back(std::make_pair(cur.first->right, 0));
				}
				else
				{
					myStack.pop_back();
					otherStack.pop_back();
				}
			}
			else
			{
				myStack.pop_back();
				otherStack.pop_back();
			}
		}
	}

	Node* extractMin(Node** node)
	{
		while ((*node)->left)
			node = &(*node)->left;

		Node* temp = *node;
		*node = (*node)->right;
		return temp;
	}

private:
	Node* root;
	size_t size;
};