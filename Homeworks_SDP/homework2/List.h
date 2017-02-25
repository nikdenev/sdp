#pragma once

#include <iostream>

template <class T>
class List
{
private:
	struct Node
	{
		Node() :
			next(nullptr),
			previous(nullptr)
		{}
		Node(Node* next, Node* previous) :
			next(next),
			previous(previous)
		{
			next->previous = this;
			previous->next = this;
		}
		Node(Node* next, Node* previous, const T& data) :
			next(next),
			previous(previous),
			data(data)
		{
			next->previous = this;
			previous->next = this;
		}

		T data;
		Node* next;
		Node* previous;
	};

public:
	class Iterator
	{
	public:
		friend class List;

	public:
		Iterator() : pointer(nullptr)
		{}

		const T& operator*() const
		{
			return this->pointer->data;
		}

		T& operator*()
		{
			return this->pointer->data;
		}

		Iterator& operator++()
		{
			this->pointer = this->pointer->next;
			return *this;
		}

		Iterator operator++(int UNUSED)
		{
			Iterator temp = *this;
			++(*this);
			return temp;
		}

		Iterator& operator--()
		{
			this->pointer = this->pointer->previous;
			return *this;
		}

		Iterator operator--(int UNUSED)
		{
			Iterator temp = *this;
			--(*this);
			return temp;
		}

		bool operator==(const Iterator& obj) const
		{
			return this->pointer == obj.pointer;
		}

		bool operator!=(const Iterator& obj) const
		{
			return !(obj == *this);
		}
	private:
		Iterator(Node* pointer) :
			pointer(pointer)
		{}

		Node* pointer;
	};

public:
	List()
	{
		init();
	}

	~List()
	{
		this->clear();
		delete start;
		delete ends;
	}

	List(const List<T>& obj)
	{
		this->init();
		this->copy(obj);
	}

	List<T>& operator=(const List<T>& obj)
	{
		if (this != &obj)
		{
			this->clear();
			this->copy(obj);
		}

		return *this;
	}

	void push_front(const T& element)
	{
		new Node(this->start->next, this->start, element);
	}

	void push_back(const T& element)
	{
		new Node(this->ends, this->ends->previous, element);
	}

	const T& front() const
	{
		return this->start->next->data;
	}

	T& front()
	{
		return this->start->next->data;
	}

	const T& back() const
	{
		return this->ends->previous->data;
	}

	T& back()
	{
		return this->ends->previous->data;
	}

	void pop_front()
	{
		if (!this->empty())
		{
			Node* temp = this->start->next;
			temp->next->previous = this->start;
			this->start->next = temp->next;
			delete temp;
		}
	}

	void pop_back()
	{
		if (!this->empty())
		{
			Node* temp = this->ends->previous;
			temp->previous->next = this->ends;
			this->ends->previous = temp->previous;
			delete temp;
		}
	}

	void clear()
	{
		Node* temp = this->start;

		while (temp->next != this->ends)
		{
			this->start = this->start->next;
			delete temp;
			temp = this->start;
		}
	}

	bool empty() const
	{
		return this->start->next == this->ends;
	}

	void erase(Iterator i)
	{
		Node* temp = i.pointer;

		temp->previous->next = temp->next;
		temp->next->previous = temp->previous;
		delete temp;
	}

	void insertAfter(Iterator i, const T& element)
	{
		//Attention
		new Node((++i).pointer, i.pointer, element);
	}

	void insertBefore(Iterator i, const T& element)
	{
		//Attention
		new Node((++i).pointer, (--i).pointer, element);
	}

	Iterator begin()
	{
		return Iterator(this->start->next);
	}

	Iterator end()
	{
		return Iterator(this->ends);
	}

private:
	void init()
	{
		this->start = new Node;
		try
		{
			this->ends = new Node;
		}
		catch (std::bad_alloc& ba)
		{
			delete this->start;
			throw;
		}
		this->start->next = this->ends;
		this->ends->previous = this->start;
	}

	void copy(const List<T>& obj)
	{
		Node* temp = obj.start->next;

		while (temp != obj.ends)
		{
			this->push_back(temp->data);
			temp = temp->next;
		}
	}

private:
	Node* start;
	Node* ends;
};
