/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_iterator.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miarzuma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 20:54:37 by miarzuma          #+#    #+#             */
/*   Updated: 2022/12/01 17:21:41 by miarzuma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iterator>
#include <cmath>
#include <functional>
#include <exception>
#include <iostream>

namespace ft
{
	template<typename T>
	struct Node;

	// Pair.
	template <typename T1, typename T2>
	class pair
	{
		public:
			pair() : first(), second() {};
			pair(const T1& a, const T2& b) : first(a), second(b) {};
			pair(const pair<T1, T2>& copy) : first(copy.first), second(copy.second) {};
			template <typename U, typename V>
			pair(const pair<U, V>& copy) : first(copy.first), second(copy.second) {};
			~pair() {};
			pair& operator=(const pair& assign)
			{
				if (this != &assign)
				{
					first = assign.first;
					second = assign.second;
				}
				return (*this);
			}

		T1 first;
		T2 second;
	};

	// Pair operators.
	template <typename T1, typename T2>
	bool operator==(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
	{ return (lhs.first == rhs.first && lhs.second == rhs.second); }

	template <typename T1, typename T2>
	bool operator!=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
	{ return (!(lhs == rhs)); }

	template <typename T1, typename T2>
	bool operator<(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
	{
		if (lhs.first < rhs.first)
			return (true);
		else if (lhs.first == rhs.first)
			return (lhs.second < rhs.second);
		return (false);
	}

	template <typename T1, typename T2>
	bool operator<=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
	{ return (!(rhs < lhs)); }
	
	template <typename T1, typename T2>
	bool operator>(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
	{ return (rhs < lhs); }

	template <typename T1, typename T2>
	bool operator>=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
	{ return (!(lhs < rhs)); }

	// Is integral.
	template <typename T>
	struct is_integral { static const bool value = false; };

	template <>
	struct is_integral<bool> { static const bool value = true; };

	template <>
	struct is_integral<char> { static const bool value = true; };

	template <>
	struct is_integral<short> { static const bool value = true; };

	template <>
	struct is_integral<int> { static const bool value = true; };

	template <>
	struct is_integral<long> { static const bool value = true; };

	template <>
	struct is_integral<long long> { static const bool value = true; };

	template <>
	struct is_integral<unsigned char> { static const bool value = true; };

	template <>
	struct is_integral<unsigned int> { static const bool value = true; };

	template <>
	struct is_integral<unsigned long> { static const bool value = true; };

	template <>
	struct is_integral<unsigned long long> { static const bool value = true; };

	// Use a boolean to typedef eiter type 1 or type 2.
	template <bool isConst, typename isFalse, typename isTrue>
	struct chooseConst {};

	template <typename isFalse, typename isTrue>
	struct chooseConst<false, isFalse, isTrue>
	{ typedef isFalse type; };

	template <typename isFalse, typename isTrue>
	struct chooseConst<true, isFalse, isTrue>
	{ typedef isTrue type; };

	// Enable if.
	template <bool B>
	struct enable_if {};

	template <>
	struct enable_if<true> { typedef int type; };

	// Map iterator.
	
	// Key			Type of key elements.
	// T			Type of mapped elements.
	// Compare		The predicate used to sort the bynary tree.
	// Node			The structure used as nodes in the binary tree.
	// B 			Boolean to indicate if it's an iterator / a const iterator.
	template < typename T, bool B>
	class map_iterator
	{
		public:
			typedef ft::Node<T>															Node;
			typedef T																	value_type;
			typedef long int															difference_type;
			typedef size_t																size_type;
			typedef std::bidirectional_iterator_tag										iterator_category;
			typedef typename chooseConst<B, value_type&, const value_type&>::type		reference;
			typedef typename chooseConst<B, value_type*, const value_type*>::type		pointer;
			typedef Node*																nodePtr;
		private:
			nodePtr			m_node;
			nodePtr			m_lastElem;
		public:

// __ Constructors & Destructor

			// Default.
			map_iterator(nodePtr node = 0, nodePtr lastElem = 0) :
				m_node(node), m_lastElem(lastElem) {}

			// Copy.
			map_iterator(const map_iterator< T, false>& copy)
			{
				m_node = copy.getNode();
				m_lastElem = copy.getLastElem();
			}

			// Destroy.
			~map_iterator() {}

			// Operator=.
			map_iterator& operator=(const map_iterator& assign)
			{
				if (this != &assign)
				{
					m_node = assign.m_node;
					m_lastElem = assign.m_lastElem;
				}
				return (*this);
			}

// __ Getters

			nodePtr getNode() const { return m_node; }
			nodePtr getLastElem() const { return m_lastElem; }

// __ Operators

			reference operator*() const { return (m_node->content); }
			pointer operator->() const { return (&m_node->content); }
			map_iterator& operator++()
			{
				m_node = Node::increment(m_node, m_lastElem);
				return (*this);
			}

			map_iterator operator++(int)
			{
				map_iterator tmp(*this);
				++(*this);
				return (tmp);
			}

			map_iterator& operator--()
			{
				m_node = Node::decrement(m_node, m_lastElem);
				return (*this);
			}

			map_iterator operator--(int)
			{
				map_iterator tmp(*this);
				--(*this);
				return (tmp);
			}

			bool operator==(const map_iterator& it) const { return (it.m_node == m_node); }
			bool operator!=(const map_iterator& it) const { return (it.m_node != m_node); }

		private:
			Node* searchMaxNode(Node *root)
			{
				if (root && root != m_lastElem && root->right && root->right != m_lastElem)
					return searchMaxNode(root->right);
				return root;
			}

			Node* searchMinNode(Node *root)
			{
				if (root && root != m_lastElem && root->left && root->left != m_lastElem)
					return searchMinNode(root->left);
				return root;
			}
	};

	// Revers map iterator.
	template<typename T,  bool B>
	class rev_map_iterator
	{
		public:
			typedef ft::Node<T>															Node;
			typedef T																	value_type;
			typedef long int															difference_type;
			typedef size_t																size_type;
			typedef std::bidirectional_iterator_tag										iterator_category;
			typedef typename chooseConst<B, value_type&, const value_type&>::type		reference;
			typedef typename chooseConst<B, value_type*, const value_type*>::type		pointer;
			typedef Node*																nodePtr;
		private:
			nodePtr			m_node;
			nodePtr			m_lastElem;
	
		public:

// __ Constructors & Destructor

			// Default.
			rev_map_iterator(nodePtr node = 0, nodePtr lastElem = 0) :
				m_node(node), m_lastElem(lastElem) {}

			// Copy.
			rev_map_iterator(const rev_map_iterator<T, false>& copy)
			{
				m_node = copy.getNonConstNode();
				m_lastElem = copy.getNonConstLastElem();
			}

			// Convert.
			explicit rev_map_iterator(map_iterator<T, true> copy)
			{
				//--copy;
				m_node = copy.getNode();
				m_lastElem = copy.getLastElem();
			}

			// Destroy.
			~rev_map_iterator() {}

			// Operator=.
			rev_map_iterator& operator=(const rev_map_iterator& assign)
			{
				if (this != &assign)
				{
					m_node = assign.m_node;
					m_lastElem = assign.m_lastElem;
				}
				return (*this);
			}

// __ Getters

			nodePtr getNonConstNode() const { return m_node; }
			nodePtr getNonConstLastElem() const { return m_lastElem; }

// __ Operators

			reference operator*() const 
			{ 
				//std::cout << "\nisNil: " << (m_node == m_lastElem ? "true" : "false") << "\n";
			//	std::cout << "\nil->parent: " << (m_node->parent->content).first << "\n";
				Node* tmp = Node::decrement(m_node, m_lastElem);
			//	std::cout << "\nnode: " << tmp->content.first << "\n";
				return (tmp->content); 
			}
			pointer operator->() const 
			{
				Node* tmp = Node::decrement(m_node, m_lastElem);
				return (&(tmp->content));
			}

			rev_map_iterator& operator++()
			{
				m_node = Node::decrement(m_node, m_lastElem);
				return (*this);
			}

			rev_map_iterator operator++(int)
			{
				rev_map_iterator tmp(*this);
				++(*this);
				return (tmp);
			}

			rev_map_iterator& operator--()
			{
				m_node = Node::increment(m_node, m_lastElem);
				return (*this);
			}

			rev_map_iterator operator--(int)
			{
				rev_map_iterator tmp(*this);
				--(*this);
				return (tmp);
			}

			bool operator==(const rev_map_iterator& it) const { return (it.m_node == m_node); }
			bool operator!=(const rev_map_iterator& it) const { return (it.m_node != m_node); }

		private:

			Node* searchMaxNode(Node *root)
			{
				if (root && root != m_lastElem && root->right && root->right != m_lastElem)
					return searchMaxNode(root->right);
				return root;
			}

			Node* searchMinNode(Node *root)
			{
				if (root && root != m_lastElem && root->left && root->left != m_lastElem)
					return searchMinNode(root->left);
				return root;
			}

	};

	// Less.
	template <typename T>
	struct less
	{
		typedef T			first_argument_type;
		typedef T			second_argument_type;
		typedef bool		result_type;
		bool operator() (const T& a, const T& b) const
		{ return (a < b); }
	};

	// Make pair.
	template <typename T1, typename T2>
	ft::pair<T1, T2> make_pair(T1 a, T2 b)
	{ return ft::pair<T1, T2>(a, b); }

	// Lexicographical Compare.
	template <typename InputIterator1, typename InputIterator2>
	bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2)
	{
		while (first1 != last1)
		{
			if (first2 == last2 || *first2 < *first1)
				return false;
			else if (*first1 < *first2)
				return true;
			++first1;
			++first2;
		}
		return (first2 != last2);
	}

	// Equal.
	template <typename InputIterator1, typename InputIterator2>
	bool equal (InputIterator1 begin1, InputIterator1 end1, InputIterator2 begin2)
	{
		while (begin1 != end1)
		{
			if (!(*begin1 == *begin2))
					return (false);
			++begin1;
			++begin2;
		}
		return (true);
	}
}

