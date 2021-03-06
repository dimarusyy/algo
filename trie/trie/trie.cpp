// trie.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <utility>
#include <iostream>
#include <string>
#include <map>
#include <array>
#include <memory>
#include <climits>

class trie
{
	struct node
	{
		node(int s, int e)
			: node(s, std::make_shared<int>(e))
		{
		}

		node(int s, std::shared_ptr<int>&& e)
			: starting_idx(s), ending_idx(e), suffix_node(nullptr), suffix_index(-1)
		{
		}

		int start() const
		{
			return starting_idx;
		}

		int end() const
		{
			return*(ending_idx.get());
		}

		int starting_idx;
		std::shared_ptr<int> ending_idx;

		std::shared_ptr<node> suffix_node;
		int suffix_index;

		std::array<std::shared_ptr<node>, CHAR_MAX> siblings;
	};

	struct active_t
	{
		explicit active_t(const std::shared_ptr<node>& nd)
			: node(nd), active_length(0), active_edge(-1)
		{
		}

		std::shared_ptr<node> node;
		size_t active_length;
		int active_edge;
	};

	// root node
	std::shared_ptr<node> _root;
	std::shared_ptr<int> _leaf_end;

	void append(const std::string& text)
	{
		if (text.empty())
		{
			return;
		}

		// create root
		if (_root == nullptr)
		{
			_root.reset(new node(-1, -1));
		}

		active_t curr_active{ _root };
		size_t remaining = 0;
		size_t end = 0;

		auto walk_down = [&](const std::shared_ptr<node>& nd)
		{
			auto edge_length = [](const std::shared_ptr<node>& nd)
			{
				return nd->end() - nd->start() + 1;
			};

			if (curr_active.active_length >= edge_length(nd))
			{
				curr_active.active_edge += edge_length(nd);
				curr_active.active_length -= edge_length(nd);
				curr_active.node = nd;

				return true;
			}

			return false;
		};

		auto add_impl = [&](size_t sym_idx)
		{
			*(_leaf_end.get()) = sym_idx;

			remaining++;
			std::shared_ptr<node> last_node = nullptr;
			while (remaining > 0)
			{
				//if active length is 0 then look for current character from root
				if (curr_active.active_length == 0)
				{
					curr_active.active_edge = sym_idx;
				}

				auto curr_sym = text[curr_active.active_edge];
				if (curr_active.node->siblings[curr_sym] == nullptr)
				{
					curr_active.node->siblings[curr_sym] = std::make_shared<node>(sym_idx, std::move(_leaf_end));

					/*A new leaf edge is created in above line starting	from  an existing node (the current activeNode), and
					if there is any internal node waiting for it's suffix link get reset, point the suffix link from that last
					internal node to current activeNode. Then set lastNewNode to NULL indicating no more node waiting for suffix link reset.*/
					if (last_node != nullptr)
					{
						last_node->suffix_node = curr_active.node;
						last_node = nullptr;
					}
				}
				//create a new leaf node with current character from leaf. This is rule 2 extension.
				else
				{
					auto next = curr_active.node->siblings[text[curr_active.active_edge]];

					// should we start from the next node ?
					if (walk_down(next))
					{
						continue;
					}

					// rule 3
					if (text[next->start() + curr_active.active_length] == text[sym_idx])
					{
						if (last_node != nullptr && curr_active.node != _root)
						{
							last_node->suffix_node = curr_active.node;
							last_node = nullptr;
						}

						curr_active.active_length++;
						break;
					}

					//split edge
					auto split_end = next->start() + curr_active.active_length - 1;

					// new internal node
					auto split_node = std::make_shared<node>(next->start(), split_end);
					curr_active.node->siblings[text[curr_active.active_edge]] = split_node;

					// new leaf
					split_node->siblings[text[sym_idx]] = std::make_shared<node>(sym_idx, std::move(_leaf_end));
					next->starting_idx += curr_active.active_length;
					split_node->siblings[text[next->start()]] = next;

					if (last_node != nullptr)
					{
						last_node->suffix_node = split_node;
					}

					last_node = split_node;
				}

				//
				remaining--;
				if (curr_active.node == _root && curr_active.active_length > 0)
				{
					curr_active.active_length--;
					curr_active.active_edge = sym_idx - remaining + 1;
				}
				else if (curr_active.node != _root)
				{
					curr_active.node = curr_active.node->suffix_node;
				}
			}

		};

		// build
		for (auto i = 0; i < text.length(); i++)
		{
			// add i-th symbol
			add_impl(i);
		}

		//dump
		dump(text, _root, 0);
	}

public:
	explicit trie(const std::string& text) : _root(nullptr), _leaf_end(std::make_shared<int>(-1))
	{
		append(text);
	}

	void dump(const std::string& text, const std::shared_ptr<node>& nd, int labelHeight)
	{
		if (nd == nullptr)  return;

		if (nd->start() != -1) //A non-root node
		{
			//Print the label on edge from parent to current node
			std::cout << text.substr(nd->start(), nd->end() - nd->start() + 1);
		}

		int leaf = 1;
		for (auto i = 0; i < CHAR_MAX; i++)
		{
			if (nd->siblings[i] != nullptr)
			{
				if (leaf == 1 && nd->start() != -1)
				{
					std::cout << "[" << nd->suffix_index << "]" << std::endl;
				}

				//Current node is not a leaf as it has outgoing
				//edges from it.
				leaf = 0;
				dump(text, nd->siblings[i], labelHeight + nd->siblings[i]->end() - nd->siblings[i]->start() + 1);
			}
		}

		if (leaf == 1)
		{
			nd->suffix_index = text.length() - labelHeight;
			std::cout << " [" << nd->suffix_index << "]" << std::endl;
		}
	}
};

//////////////////////////////////////////////////////////////////////////

int main()
{
	trie t {"abcabxabcd$"};
    return 0;
}

