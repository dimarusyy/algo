// sort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <iterator>
#include <vector>

template <typename T>
struct merge_t
{
	void operator()(T data[], size_t lo, size_t mid, size_t hi)
	{
		auto n_left = mid - lo + 1;
		auto n_right = hi - mid;
		
		std::vector<T> left(n_left);
		std::vector<T> right(n_right);

		// copy
		for (auto i = 0U; i < n_left; i++)
		{
			left[i] = data[i + lo];
		}
		for (auto i = 0U; i < n_right; i++)
		{
			right[i] = data[i + mid + 1];
		}

		// merge
		size_t i = 0, j = 0, k = lo;
		while (i < n_left && j < n_right)
		{
			if (left[i] <= right[j])
			{
				data[k++] = left[i++];
			}
			else
			{
				data[k++] = right[j++];
			}
		}

		while (i < n_left)
		{
			data[k++] = left[i++];
		}
		while (j < n_right)
		{
			data[k++] = right[j++];
		}
	}
};

template <typename T, typename M = merge_t<T>>
void merge_sort(T data[], size_t lo, size_t hi)
{
	if (lo < hi)
	{
		// mid element
		auto mid = lo + (hi - lo) / 2;

		merge_sort(data, lo, mid);
		merge_sort(data, mid + 1, hi);

		static M merge_impl;
		merge_impl(data, lo, mid, hi);
	}
}

int main()
{
	int v[] = { 10, 5, 1, 2, 3, 3 };

// 	merge_sort(v, 0, _countof(v) - 1);
// 	for (auto vi : v)
// 	{
// 		std::cout << vi << " ";
// 	}

    return 0;
}

