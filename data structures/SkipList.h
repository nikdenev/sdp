#pragma once

#include <cstring>
#include <cstdlib>

template <class Key>
class SkipList
{
private:
	struct Node
	{
		Node(char towerHight) :
			towerHight(towerHight)
		{
			tower = new Node*[towerHight];
		}

		Node(char towerHight, Key value) :
			towerHight(towerHight), value(value)
		{
			tower = new Node*[towerHight];
		}

		~Node()
		{
			delete[] tower;
		}

		Key value;
		Node** tower;
		char towerHight;
	};

public:
	SkipList(const size_t maxTowerHeight = 16) :
		MAX_TOWER_HIGHT(maxTowerHeight)
	{
		init();
		srand(0);
	}

	~SkipList()
	{
		helpClear();
	}

	SkipList(const SkipList& other)
	{
		srand(0);
		MAX_TOWER_HIGHT = other.MAX_TOWER_HIGHT;
		init();
		copy(other);
	}

	SkipList& operator= (const SkipList& other)
	{
		if (this != &other)
		{
			MAX_TOWER_HIGHT = other.MAX_TOWER_HIGHT;
			clear();
			copy(other);
		}

		return *this;
	}

	void insert(const Key& value)
	{
		++size;

		Node*** helpTower = new Node**[MAX_TOWER_HIGHT];

		for (int i = 0; i < MAX_TOWER_HIGHT; ++i)
		{
			helpTower[i] = &start->tower[i];
		}

		Node* cur = start;
		char curLevel = MAX_TOWER_HIGHT - 1;

		while (curLevel >= 0)
		{
			if (cur->tower[curLevel] == end)
				--curLevel;
			else if (cur->tower[curLevel]->value < value)
			{
				cur = cur->tower[curLevel];

				for (int i = 0; i <= curLevel; ++i)
				{
					helpTower[i] = &cur->tower[i];
				}
			}
			else
				--curLevel;
		}

		Node* temp = new Node(calculateHight(), value);
		for (int i = 0; i < (*cur->tower)->towerHight; ++i)
		{
			temp->tower[i] = *helpTower[i];
			*helpTower[i] = temp;
		}

		delete[] helpTower;
	}

	bool remove(const Key& value)
	{
		Node*** helpTower = new Node**[MAX_TOWER_HIGHT];

		for (int i = 0; i < MAX_TOWER_HIGHT; ++i)
		{
			helpTower[i] = &start->tower[i];
		}

		Node* cur = start;
		char curLevel = MAX_TOWER_HIGHT - 1;

		while (curLevel >= 0)
		{
			if (cur->tower[curLevel] == end)
				--curLevel;
			else if (cur->tower[curLevel]->value < value)
			{
				cur = cur->tower[curLevel];

				for (int i = 0; i <= curLevel; ++i)
				{
					helpTower[i] = &cur->tower[i];
				}
			}
			else
				--curLevel;
		}

		Node* temp = *cur->tower;

		if (temp->value != value)
		{
			delete[] helpTower;
			return false;
		}

		for (int i = 0; i < temp->towerHight; ++i)
		{
			*helpTower[i] = temp->tower[i];
		}

		--size;
		delete temp;

		delete[] helpTower;
		return true;
	}

	bool search(const Key& value) const
	{
		Node* cur = start;
		char curLevel = MAX_TOWER_HIGHT - 1;

		while (curLevel >= 0)
		{
			if (cur->tower[curLevel] == end)
				--curLevel;
			else if (cur->tower[curLevel]->value == value)
				return true;
			else if (cur->tower[curLevel]->value < value)
				cur = cur->tower[curLevel];
			else
				--curLevel;
		}

		return false;
	}

	size_t getSize() const
	{
		return size;
	}

	void clear()
	{
		helpClear();
		init();
	}

private:
	void init()
	{
		size = 0u;
		start = new Node(MAX_TOWER_HIGHT);
		end = new Node(1);

		for (int i = 0; i < MAX_TOWER_HIGHT; ++i)
		{
			start->tower[i] = end;
		}
	}

	void helpClear()
	{
		Node* temp;

		while (start != end)
		{
			temp = start;

			start = *start->tower;

			delete temp;
		}

		delete end;
	}

	void copy(const SkipList& other)
	{
		size = other.size;

		Node*** helpTower = new Node**[MAX_TOWER_HIGHT];

		for (int i = 0; i < MAX_TOWER_HIGHT; ++i)
		{
			helpTower[i] = &start->tower[i];
		}

		Node* otherCur = *other.start->tower;
		Node* cur = start;

		while (otherCur != other.end)
		{
			cur = *cur->tower =
				new Node(otherCur->towerHight, otherCur->value);

			otherCur = *otherCur->tower;

			for (int i = 0; i < cur->towerHight; ++i)
			{
				*helpTower[i] = cur;
				helpTower[i] = &cur->tower[i];
			}
		}

		for (int i = 0; i < MAX_TOWER_HIGHT; ++i)
		{
			*helpTower[i] = end;
		}

		delete[] helpTower;
	}

	char calculateHight()
	{
		char res = 1;
		while (res < MAX_TOWER_HIGHT && rand() & 1)
			++res;

		return res;
	}

private:
	Node* start;
	Node* end;
	size_t size;
	size_t MAX_TOWER_HIGHT;
};