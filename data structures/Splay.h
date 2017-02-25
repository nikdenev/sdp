#pragma once

#include <vector>
#include <cassert>

template <class T>
class Splay
{
private:
	struct Node
	{
		Node() :
			left(nullptr), right(nullptr)
		{}
		Node(const T& key) :
			key(key), left(nullptr), right(nullptr)
		{}
		Node(const T& key, Node *left, Node* right) :
			key(key), left(left), right(right)
		{}

		T key;
		Node *left, *right;
	};

public:
	Splay() :
		root(nullptr),
		size(0)
	{}

	~Splay()
	{
		clear();
	}

	Splay(const Splay& other)
	{
		copy(other);
	}

	Splay& operator=(const Splay& other)
	{
		if (this != &other)
		{
			clear();
			copy(other);
		}

		return *this;
	}

	void insert(const T& key)
	{
		++size;

		Node** cur = &root;
		while (true)
		{
			path.push_back(cur);

			if (!*cur)
			{
				*cur = new Node(key);
				splaying(path.size());
				path.clear();
				assert(isTop(key));
				return;
			}

			else if ((*cur)->key < key)
				cur = &(*cur)->right;
			else
				cur = &(*cur)->left;
		}
	}

	bool remove(const T& key)
	{
		--size;

		Node** cur = &root;
		while (true)
		{
			if (!*cur)
			{
				++size;
				splaying(path.size());
				path.clear();
				return false;
			}

			path.push_back(cur);

			if ((*cur)->key == key)
			{
				Node* toDel;
				if ((*cur)->left && (*cur)->right)
				{
					int oldSize = path.size();
					Node* min = extractMin(&(*cur)->right);
					min->left = (*cur)->left;
					min->right = (*cur)->right;

					toDel = *cur;
					*cur = min;

					if (oldSize < path.size())
						path[oldSize] = &(*cur)->right;
				}
				else if ((*cur)->left)
				{
					toDel = *cur;
					*cur = toDel->left;
					path.pop_back();
				}
				else
				{
					toDel = *cur;
					*cur = toDel->right;
					path.pop_back();
				}

				delete toDel;
				splaying(path.size());
				path.clear();
				return true;
			}
			else if ((*cur)->key < key)
				cur = &(*cur)->right;
			else
				cur = &(*cur)->left;
		}
	}

	bool search(const T& key)
	{
		Node** cur = &root;
		while (true)
		{
			if (!*cur)
			{
				splaying(path.size());
				path.clear();
				return false;
			}

			path.push_back(cur);

			if ((*cur)->key == key)
			{
				splaying(path.size());
				path.clear();
				return true;
			}
			else if ((*cur)->key < key)
				cur = &(*cur)->right;
			else
				cur = &(*cur)->left;
		}
	}

	size_t getSize() const
	{
		return size;
	}

	void clear()
	{
		clear(root);
		root = nullptr;
		size = 0;
	}

private:
	void copy(const Splay& other)
	{
		size = other.size;
		if (!size)
			return;

		root = new Node(other.root->key);

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
						new Node(cur.first->left->key));

					otherStack.push_back(std::make_pair(cur.first->left, 0));
				}
				else if (cur.first->right)
				{
					cur.second = 2;
					myStack.push_back(myStack.back()->right =
						new Node(cur.first->right->key));

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
						new Node(cur.first->right->key));

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

	Node* extractMin(Node** node)
	{
		while ((*node)->left)
		{
			path.push_back(node);
			node = &(*node)->left;
		}

		Node* temp = *node;
		*node = (*node)->right;
		return temp;
	}

	bool isTop(const T& key)
	{
		if (size != 0)
			return key == root->key;
		return false;
	}

	void splaying(int currentSize)
	{
		while (currentSize > 2)
		{
			if ((*path[currentSize - 3])->left == *path[currentSize - 2])
				if ((*path[currentSize - 2])->left == *path[currentSize - 1])
					rightRightRotation(*path[currentSize - 3]);
				else
					leftRightRotation(*path[currentSize - 3]);
			else
				if ((*path[currentSize - 2])->right == *path[currentSize - 1])
					leftLeftRotation(*path[currentSize - 3]);
				else
					rightLeftRotation(*path[currentSize - 3]);

			currentSize -= 2;
		}

		if (currentSize == 2)
			if ((*path[0])->left == *path[1])
				rightRotation(*path[0]);
			else
				leftRotation(*path[0]);
	}

	void leftRotation(Node*& node)
	{
		Node* temp = node->right;
		node->right = temp->left;
		temp->left = node;
		node = temp;
	}

	void rightRotation(Node*& node)
	{
		Node* temp = node->left;
		node->left = temp->right;
		temp->right = node;
		node = temp;
	}

	void leftLeftRotation(Node*& node)
	{
		Node* temp = node->right;
		node->right = temp->left;
		temp->left = node;

		node = temp->right;
		temp->right = node->left;
		node->left = temp;
	}

	void leftRightRotation(Node*& node)
	{
		Node* temp = node->left->right;
		node->left->right = temp->left;
		temp->left = node->left;

		node->left = temp->right;
		temp->right = node;
		node = temp;
	}

	void rightRightRotation(Node*& node)
	{
		Node* temp = node->left;
		node->left = temp->right;
		temp->right = node;

		node = temp->left;
		temp->left = node->right;
		node->right = temp;
	}

	void rightLeftRotation(Node*& node)
	{
		Node* temp = node->right->left;
		node->right->left = temp->right;
		temp->right = node->right;

		node->right = temp->left;
		temp->left = node;
		node = temp;
	}

private:
	Node* root;
	size_t size;
private:
	std::vector<Node**> path;
};