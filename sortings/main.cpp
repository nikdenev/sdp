#include <algorithm>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <cassert>
#include <ctime>
#include <random>

#include "MergeSort.h"
#include "ParallelMergeSort.h"
#include "QuickSort.h"
#include "HeapSort.h"

#define ARRAY_SIZE 200
class SomethingBig
{
public:
	SomethingBig()
	{
		arr = new int[ARRAY_SIZE];
	}
	~SomethingBig()
	{
		delete[] arr;
	}
	SomethingBig(const SomethingBig& obj)
	{
		x = obj.x;

		arr = new int[ARRAY_SIZE];

		for (int i = 0; i < ARRAY_SIZE; ++i)
			arr[i] = obj.arr[i];
	}
	SomethingBig& operator=(const SomethingBig& obj)
	{
		if (this != &obj)
		{
			x = obj.x;

			for (int i = 0; i < ARRAY_SIZE; ++i)
				arr[i] = obj.arr[i];
		}

		return *this;
	}

	void setX(int y)
	{
		x = y;
	}

	int getX() const
	{
		return x;
	}

	bool operator<(const SomethingBig& obj)
	{
		return x < obj.x;
	}

private:
	int x;
	int* arr;
};

SomethingBig* generatorBig(int size)
{
	SomethingBig* arr = new SomethingBig[size];

	srand(0);

	for (int i = 0; i < size; ++i)
		arr[i].setX(rand());

	return arr;
}

std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(-(1 << 30), (1 << 30));

int* generatorInt(int size)
{
	int* arr = new int[size];

	for (int i = 0; i < size; ++i)
		arr[i] = distribution(generator);

	return arr;
}

void TEST_BIG(int size)
{
	int i = 0;
	int times = 20;

	double qtime = 0.0, pmtime = 0.0, stdtime = 0.0;

	while (i++ != times) {

	SomethingBig* arr = generatorBig(size);
	SomethingBig* arr2 = new SomethingBig[size];
	for (int i = 0; i < size; ++i)
	{
		arr2[i] = arr[i];
	}
	SomethingBig* arr3 = new SomethingBig[size];
	for (int i = 0; i < size; ++i)
	{
		arr3[i] = arr[i];
	}
	

	clock_t start = clock();

	QuickSort(arr, arr + size);

	clock_t end = clock();

	std::cout << "Quick sort: " << (double(end - start)) / CLOCKS_PER_SEC << std::endl;

	start = clock();

	my_parallel_merge_sort(arr2, arr2 + size);

	end = clock();

	std::cout << "Parallel merge: " << (double(end - start)) / CLOCKS_PER_SEC << std::endl;

	start = clock();

	std::sort(arr3, arr3 + size);

	end = clock();

	std::cout << "std::sort: " << (double(end - start)) / CLOCKS_PER_SEC << std::endl;
	std::cout << std::endl;


	assert(std::is_sorted(arr, arr + size));
	assert(std::is_sorted(arr2, arr2 + size));
	assert(std::is_sorted(arr3, arr3 + size));

	delete[] arr;
	delete[] arr2;
	delete[] arr3;

	}

	std::cout << "TEST WITH BIG OBJECTS:\n" << "Size: " << size << '\n';

	std::cout << "Quick sort: " << qtime / times << std::endl;
	std::cout << "Parallel merge: " << pmtime / times << std::endl;
	std::cout << "std::sort: " << stdtime / times << std::endl;
	std::cout << std::endl;
}

void TEST_INT(int size)
{
	int i = 0;
	int times = 2;

	double danitime = 0.0 ,qtime = 0.0, pmtime = 0.0, stdtime = 0.0;

	while (i++ != times){

	int* arr = generatorInt(size);
	int* arr2 = new int[size];
	for (int i = 0; i < size; ++i)
	{
		arr2[i] = arr[i];
	}
	
	int* arr3 = new int[size];
	for (int i = 0; i < size; ++i)
	{
		arr3[i] = arr[i];
	}
	int* arr4 = new int[size];
	for (int i = 0; i < size; ++i)
	{
		arr4[i] = arr[i];
	}
	
	clock_t start = clock();

	QuickSort(arr, arr + size);

	clock_t end = clock();

	qtime += (double(end - start)) / CLOCKS_PER_SEC;
	
	start = clock();

	my_parallel_merge_sort(arr2, arr2 + size);

	end = clock();

	pmtime += (double(end - start)) / CLOCKS_PER_SEC;

	start = clock();

	std::sort(arr3, arr3 + size);

	end = clock();

	stdtime += (double(end - start)) / CLOCKS_PER_SEC;

	assert(std::is_sorted(arr, arr + size));
	assert(std::is_sorted(arr2, arr2 + size));
	assert(std::is_sorted(arr3, arr3 + size));

	delete[] arr4;
	delete[] arr;
	delete[] arr2;
	delete[] arr3;
	}

	std::cout << "TEST WITH INT: \n" << "Size: " << size << '\n';

	std::cout << "Quick sort: " << qtime/times << std::endl;
	std::cout << "Parallel merge: " << pmtime/ times << std::endl;
	std::cout << "std::sort: " << stdtime/ times << std::endl;
	std::cout << std::endl;
}


void TEST_INT_Quicks(int size)
{
	int i = 0;
	int times = 3;

	double hptime = 0.0, q1time = 0.0, q2time = 0.0, pmtime = 0.0, stdtime = 0.0;

	while (i++ != times) {

		int* arr = generatorInt(size);
		int* arr2 = new int[size];
		for (int i = 0; i < size; ++i)
		{
			arr2[i] = arr[i];
		}
		int* arr3 = new int[size];
		for (int i = 0; i < size; ++i)
		{
			arr3[i] = arr[i];
		}
		int* arr4 = new int[size];
		for (int i = 0; i < size; ++i)
		{
			arr4[i] = arr[i];
		}
		int* arr5 = new int[size];
		for (int i = 0; i < size; ++i)
		{
			arr5[i] = arr[i];
		}

		clock_t start = clock();

		QuickSort(arr, arr + size);

		clock_t end = clock();

		q1time += (double(end - start)) / CLOCKS_PER_SEC;

		start = clock();

		heapSort(arr5, arr5 + size);

		end = clock();

		hptime += (double(end - start)) / CLOCKS_PER_SEC;


		start = clock();

		std::sort(arr4, arr4 + size);

		end = clock();

		stdtime += (double(end - start)) / CLOCKS_PER_SEC;

		start = clock();

		BetterQuickSort(arr2, arr2 + size);

		end = clock();

		q2time += (double(end - start)) / CLOCKS_PER_SEC;

		start = clock();

		my_parallel_merge_sort(arr3, arr3 + size);

		end = clock();

		pmtime += (double(end - start)) / CLOCKS_PER_SEC;

		
		assert(std::is_sorted(arr, arr + size));
		assert(std::is_sorted(arr2, arr2 + size));
		assert(std::is_sorted(arr3, arr3 + size));
		assert(std::is_sorted(arr4, arr4 + size));
		assert(std::is_sorted(arr5, arr5 + size));

		delete[] arr;
		delete[] arr2;
		delete[] arr3;
		delete[] arr4;
		delete[] arr5;
	}

	std::cout << "TEST WITH INT: \n" << "Size: " << size << '\n';

	std::cout << "Quick sort: " << q1time / times << std::endl;
	std::cout << "Heap sort: " << hptime / times << std::endl;
	std::cout << "Better Quick sort: " << q2time / times << std::endl;
	std::cout << "Parallel merge: " << pmtime / times << std::endl;
	std::cout << "std::sort: " << stdtime / times << std::endl;
	std::cout << std::endl;
}

void THE_TEST(int size)
{
	int i = 0;
	int times = 40;

	double hptime = 0.0, q1time = 0.0, q2time = 0.0, pmtime = 0.0, stdtime = 0.0;

	while (i++ != times) {

		int* arr = generatorInt(size);
		int* arr2 = new int[size];
		for (int i = 0; i < size; ++i)
		{
			arr2[i] = arr[i];
		}
		int* arr5 = new int[size];
		for (int i = 0; i < size; ++i)
		{
			arr5[i] = arr[i];
		}

		clock_t start = clock();
		QuickSort(arr, arr + size);
		clock_t end = clock();
		q1time += (double(end - start)) / CLOCKS_PER_SEC;

		start = clock();
		QuickHeapSort(arr5, arr5 + size);
		end = clock();
		hptime += (double(end - start)) / CLOCKS_PER_SEC;

		start = clock();
		BetterQuickSort(arr2, arr2 + size);
		end = clock();
		q2time += (double(end - start)) / CLOCKS_PER_SEC;

		assert(std::is_sorted(arr, arr + size));
		assert(std::is_sorted(arr2, arr2 + size));
		assert(std::is_sorted(arr5, arr5 + size));

		delete[] arr;
		delete[] arr2;
		delete[] arr5;
	}

	std::cout << "TEST WITH INT: \n" << "Size: " << size << '\n';

	std::cout << "Quick sort: " << q1time / times << std::endl;
	std::cout << "Quick with Heap sort: " << hptime / times << std::endl;
	std::cout << "Better Quick sort: " << q2time / times << std::endl;
	std::cout << std::endl;
}

int main()
{
	THE_TEST(30000);

	return 0;
}
