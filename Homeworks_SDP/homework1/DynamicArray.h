#pragma once

template <class TYPE>
class DynamicArray
{
public:
	DynamicArray(size_t capacity = 0);
	~DynamicArray();
	DynamicArray(const DynamicArray<TYPE>& obj);
	DynamicArray<TYPE>& operator=(const DynamicArray<TYPE>& obj);

	void append(const TYPE& element);

	void remove(int index);

	void removeLast();

	const TYPE& getLast() const;
	TYPE& getLast();

	size_t getSize() const;
	size_t getCapacity() const;

	bool isEmpty() const;

	const TYPE& operator[](int index) const;
	TYPE& operator[](int index);

	void clear();

private:

	void copy(const DynamicArray<TYPE>& obj);

	void resize(int newSize);
private:
	TYPE* array;
	size_t size;
	size_t capacity;
};

template<class TYPE>
inline DynamicArray<TYPE>::DynamicArray(size_t capacity) :
	array(nullptr),
	size(0),
	capacity(capacity)
{
	if (capacity > 0)
	{
		this->array = new TYPE[capacity];
	}
	else if (capacity < 0)
	{
		throw "Unvalid capacity!";
	}
}

template<class TYPE>
inline DynamicArray<TYPE>::~DynamicArray()
{
	delete[] this->array;
}

template<class TYPE>
inline DynamicArray<TYPE>::DynamicArray(const DynamicArray<TYPE>& obj) :
	array(nullptr),
	size(0),
	capacity(0)
{
	this->copy(obj);
}

template<class TYPE>
inline DynamicArray<TYPE>& DynamicArray<TYPE>::operator=(const DynamicArray<TYPE>& obj)
{
	if (this != &obj)
		this->copy(obj);

	return *this;
}

template<class TYPE>
inline void DynamicArray<TYPE>::append(const TYPE & element)
{
	if (this->size == this->capacity)
	{
		this->resize(this->size ? 2*this->size : 2);
	}

	this->array[this->size++] = element;
}

template<class TYPE>
inline void DynamicArray<TYPE>::remove(int index)
{
	if (index < 0 || index >= this->getSize())
		throw "Unvalid index!";

	if (4 * this->size <= this->capacity)
		this->resize(2 * this->size);

	for (int i = index + 1; i < this->size; ++i)
		this->array[i - 1] = this->array[i];

	--this->size;
}

template<class TYPE>
inline void DynamicArray<TYPE>::removeLast()
{
	if (this->isEmpty())
	{
		throw "Can't remove elements from empty array!";
	}

	if (4 * this->size <= this->capacity)
		this->resize(2 * this->size);

	--this->size;
}

template<class TYPE>
inline const TYPE & DynamicArray<TYPE>::getLast() const
{
	if (this->isEmpty())
	{
		throw "Can't get the last element from empty array!";
	}

	return this->array[this->size - 1];
}

template<class TYPE>
inline TYPE & DynamicArray<TYPE>::getLast()
{
	if (this->isEmpty())
	{
		throw "Can't get the last element from empty array!";
	}

	return this->array[this->size - 1];
}

template<class TYPE>
inline size_t DynamicArray<TYPE>::getSize() const
{
	if (this->isEmpty())
	{
		throw "Can't get the last element from empty array!";
	}

	return this->size;
}

template<class TYPE>
inline size_t DynamicArray<TYPE>::getCapacity() const
{
	return this->capacity;
}

template<class TYPE>
inline bool DynamicArray<TYPE>::isEmpty() const
{
	return !this->size;
}

template<class TYPE>
inline const TYPE & DynamicArray<TYPE>::operator[](int index) const
{
	if (index < 0 || index >= this->getSize())
		throw "Unvalid index!";

	return this->array[index];
}

template<class TYPE>
inline TYPE & DynamicArray<TYPE>::operator[](int index)
{
	if (index < 0 || index >= this->getSize())
		throw "Unvalid index!";

	return this->array[index];
}

template<class TYPE>
inline void DynamicArray<TYPE>::clear()
{
	delete[] this->array;

	this->array = nullptr;

	this->size = this->capacity = 0;
}

template<class TYPE>
inline void DynamicArray<TYPE>::copy(const DynamicArray<TYPE>& obj)
{
	if (this->capacity < obj.getSize())
	{
		this->size = 0;

		resize(obj.getCapacity());
	}

	this->size = obj.getSize();

	for (int i = 0; i < this->size; ++i)
		this->array[i] = obj[i];
}

template<class TYPE>
inline void DynamicArray<TYPE>::resize(int newSize)
{
	if (newSize == 0)
	{
		this->clear();
	}
	else
	{
		TYPE* newArray = new TYPE[newSize];

		for (int i = 0; i < this->size && i < newSize; ++i)
			newArray[i] = this->array[i];

		delete[] this->array;

		this->array = newArray;

		this->capacity = newSize;
	}
}
