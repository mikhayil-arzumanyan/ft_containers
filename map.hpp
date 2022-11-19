/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miarzuma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 12:17:38 by miarzuma          #+#    #+#             */
/*   Updated: 2022/11/19 15:32:48 by miarzuma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include "map_iterator.hpp"

namespace ft
{
    		//   key     		Type of keys.
    		//   T       	 	Type of elements.
		   	//   Compare  		Comparison object used to sort the binary tree.
			//   Allocator		Object used to manage the vector' storage.
	template<
		typename Key,
		typename T,
		typename Compare = std::less<Key>,
			typename Allocator = std::allocator<std::pair<const Key, T> > >
	class map
	{
		private:
			//Attributes
			struct Node
			{
				std::pair<const Key, T>		content;
				Node*						parent;
				Node*						left;
				Node*						right;
			};
		public:
			//Member Type
			typedef Key									key_type;
			typedef T									mapped_type;
			typedef std::pair<const Key, T>				value_type;
			typedef std::size_t							size_type;
			typedef std::ptrdiff_t						difference_type;
			typedef Compare								key_compare;
			typedef Allocator							allocator_type;
			typedef value_type&							reference;
			typedef const value_type&					const_reference;
			typedef typename Allocator::pointer			pointer;
			typedef typename Allocator::const_pointer	const_pointer;
			typedef typename ft::map_iterator<Key, T, Compare, Node, false>		iterator;
			typedef typename ft::map_iterator<Key, T, Compare, Node, true>		const_iterator;
			typedef typename ft::rev_map_iterator<Key, T, Compare, Node, false>	reverse_iterator;
			typedef typename ft::rev_map_iterator<Key, T, Compare, Node, true>	const_reverse_iterator;
		public:
			//Member classes
			class value_compare : std::binary_function<value_type, value_type, bool>
			{
				protected:
					Compare comp;
					value_compare(Compare c) : comp(c) {}
				public:
					bool operator() (const value_type& lhs, const value_type& rhs) const
					{
						return (this->comp(lhs.first, rhs.first));
					}
					typedef bool		result_type;
					typedef value_type	first_argument_type;
					typedef value_type	second_argument_type;
				private:
					friend class map<key_type, value_type, key_compare, allocator_type>;
			};
		private:
			//Attributes
			Node*					m_root;
			Node*					m_lastElem;
			size_type				m_size;
			allocator_type			m_allocPair;
			key_compare				m_comp;
			std::allocator<Node>	m_allocNode;

// __ Constructors and Destructor

		public:
			//Default
			explicit map(const Compare& comp,
			const Allocator& alloc = Allocator()) : m_size(0), m_allocPair(alloc), m_comp(comp)
			{
				m_lastElem = createNode(std::pair<const Key, T>());
				m_root = m_lastElem;
				m_lastElem->left = m_lastElem;
				m_lastElem->right = m_lastElem;
			}

			//Range
			template<typename InputIt>map(InputIt first, InputIt last,
			const Compare& comp = Compare(), const Allocator& alloc = Allocator(),
			typename ft::enable_if<!ft::is_integral<InputIt>::value>::type* = 0) :
			m_size(0), m_allocPair(alloc), m_comp(comp)
			{
				m_lastElem = createNode(std::pair<const Key, T>());
				m_root = m_lastElem;
				m_lastElem->left = m_lastElem;
				m_lastElem->right = m_lastElem;
				for (; first != last; ++first)
					insert(*first);
			}

			//Copy
			map(const map& other) : 
			m_size(0), m_allocPair(other.m_allocPair), m_comp(other.m_comp), m_allocNode(other.m_allocNode)
			{
				m_lastElem = createNode(std::pair<const Key, T>());
				m_root = m_lastElem;
				m_lastElem->left = m_lastElem;
				m_lastElem->right = m_lastElem;
				for (const_iterator it = other.begin(); it != other.end(); ++it)
					insert(*it);
			}

			//Operator=
			map &operator=(const map &other)
			{
				map tmp(other);
				this->swap(tmp);
				return *this;
			}

			//Destroy
			~map()
			{
//				clear();
				deallocateNode(m_lastElem);
			}

// __ Iterators

			iterator begin()						{ return iterator(m_lastElem->right, m_lastElem, m_comp); }
			const_iterator begin() const			{ return const_iterator(m_lastElem->right, m_lastElem, m_comp); }
			iterator end()							{ return iterator(m_lastElem, m_lastElem, m_comp); }
			const_iterator end() const				{ return const_iterator(m_lastElem, m_lastElem, m_comp); }
			reverse_iterator rbegin()				{ return reverse_iterator(m_lastElem->left, m_lastElem, m_comp); }
			const_reverse_iterator rbegin() const	{ return const_reverse_iterator(m_lastElem->left, m_lastElem, m_comp); } 
			reverse_iterator rend()					{ return reverse_iterator(m_lastElem, m_lastElem, m_comp); }
			const_reverse_iterator rend() const		{ return const_reverse_iterator(m_lastElem, m_lastElem, m_comp); }

// __ Capacity

			//Empty
			bool empty() const 			{ return m_size == 0; }

			//Size
			size_type size() const 		{ return m_size; }

			//Max
			size_type max_size() const
			{
				return m_allocPair.max_size();
			}

// __ Element Access

			T& operator[](const Key& k)
			{
				Node* tmp = searchNode(m_root, k);
				if (tmp)
					return tmp->content.second;
				value_type val = make_pair<Key, T>(k, T());
				return insertNode(m_root, val)->content.second;
			}

// __ Modifiers

			//Insert one element
			ft::pair<iterator, bool> insert (const value_type& val)
			{
				Node* elemIsPresent = searchNode(m_root, val.first);
				if (elemIsPresent)
					return ft::pair<iterator, bool>(iterator(elemIsPresent, m_lastElem, m_comp), false);
				++m_size;
				return ft::pair<iterator, bool>(iterator(insertNode(m_root, val), m_lastElem, m_comp), true);
			}

			//Insert one element starting from a certain position
			iterator insert (iterator position, const value_type& val)
			{
				if (position->first > val.firs)
				{
					iterator prev(position);
					--prev;
					while (prev != end() && prev->first >= val.first)
					{
						--position;
						--prev;
					}
				}
				else if (position->first < val.first)
				{
					iterator next(position);
					++next;
					while (next != end() && next->first <= val.first)
					{
						++position;
						++next;
					}
				}
				if (position != end() && val.first == position->first)
					return position;
				++m_size;
				return iterator(insertNode(position.getNode(), val), m_lastElem, m_comp);
			}

			//Insert all elements
			template <typename InputIt>
			void insert (InputIt first, InputIt last,
			typename ft::enable_if<!ft::is_integral<InputIt>::value>::type* = 0)
			{
				while (first != last)
					insert(*first++);
			}
	};

}
