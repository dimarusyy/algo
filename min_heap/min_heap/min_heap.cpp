// min_heap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <iterator>

template <typename T>
class min_heap
{
	std::vector<T> _data;
	static const size_t DEFAULT_HEAP_SIZE = 10;

	void update()
	{
		auto i = _data.size() - 1;
		while (i != 0)
		{
			const auto p_i = parent(i);
			if (!(_data[p_i] > _data[i]))
			{
				break;
			}
			// else
			std::iter_swap(_data.begin() + i, _data.begin() + p_i);
			i = p_i;
		}
	}

	T remove_min()
	{
		if (_data.empty())
		{
			return std::numeric_limits<T>::min();
		}

		if (_data.size() == 1)
		{
			// pop last
			auto last_val = _data.back();
			_data.pop_back();
			return last_val;
		}

		// traverse
		auto root = _data[0];
		_data[0] = _data.back();
		_data.pop_back();
		min_heapify(0);
		
		return root;
	}

	void min_heapify(size_t i)
	{
		auto l = left(i);
		auto r = right(i);
		auto smallest = i;
		
		// find smallest of leafs
		if (l < _data.size() && _data[l] < _data[i])
		{
			smallest = l;
		}

		if (r < _data.size() && _data[r] < _data[smallest])
			smallest = r;
		
		// invoke recursively
		if (smallest != i)
		{
			std::iter_swap(_data.begin() + i, _data.begin() + smallest);
			min_heapify(smallest);
		}
	}

public:
	min_heap(size_t sz = DEFAULT_HEAP_SIZE) 
		: _data() 
	{
		_data.reserve(sz);
	}

	void insert(const T& val)
	{
		_data.push_back(val);
		update();
	}

	void remove(size_t i)
	{
		_data[i] = std::numeric_limits<T>::min();
		update();
		remove_min();
	}

	const std::vector<T>& data() const
	{
		return _data;
	}

	static const auto parent(size_t i)
	{
		return (i - 1) / 2;
	}

	static const auto left(size_t i)
	{
		return 2 * i + 1;
	}

	static const auto right(size_t i)
	{
		return 2 * i + 2;
	}

};

int main()
{
	min_heap<int> h{};
	h.insert(10);
	h.insert(12);
	h.insert(1);
	h.insert(3);
	std::copy(h.data().begin(), h.data().end(), std::ostream_iterator < int >(std::cout, " "));
	std::cout << std::endl;

	// remove at index 0
	h.remove(0);
	std::copy(h.data().begin(), h.data().end(), std::ostream_iterator < int >(std::cout, " "));
	std::cout << std::endl;

    return 0;
}

