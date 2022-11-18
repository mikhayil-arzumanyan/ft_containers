/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_iterator.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miarzuma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 20:54:37 by miarzuma          #+#    #+#             */
/*   Updated: 2022/11/17 20:31:18 by miarzuma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iterator>
#include <cmath>
#include <functional>

namespace ft
{
	//Is_integral
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

	//Use a boolean to typedef eiter type 1 or type 2.
	template <bool isConst, typename isFalse, typename isTrue>
	struct chooseConst {};

	template <typename isFalse, typename isTrue>
	struct chooseConst<false, isFalse, isTrue>
	{ typedef isFalse type; };

	template <typename isFalse, typename isTrue>
	struct chooseConst<true, isFalse, isTrue>
	{ typedef isTrue type; };

	//Enable_if
	template <bool B>
	struct enable_if {};

	template <>
	struct enable_if<true> { typedef int type; };

	// Key			Type of key elements.
	// T			Type of mapped elements.
	// Compare		The predicate used to sort the bynary tree.
	// Node			The structure used as nodes in the binary tree.
	// B 			Boolean to indicate if it's an iterator / a const iterator.
	template <typename Key, typename T, typename Compare, typename Node, bool B>
	class map_iterator
	{
		public:
			typedef Key																	key_type;	
			typedef Compare																key_compare;
			typedef T																	mapped_type;
			typedef std::pair<const key_type, mapped_type>								value_type;
			typedef long int															difference_type;
			typedef size_t																size_type;
			typedef std::bidirectional_iterator_tag										iterator_category;
			typedef typename chooseConst<B, value_type&, const value_type&>::type		reference;
			typedef typename chooseConst<B, value_type*, const value_type*>::type		pointer;
			typedef Node*																nodePtr;
		private:
			nodePtr			m_node;
			nodePtr			m_lastElem;
			key_compare		m_comp;
		public:

// __ Constructors & Destructor

			//Default
			map_iterator(nodePtr node = 0, nodePtr lastElem = 0, const key_compare& comp = key_compare()) :
				m_node(node), m_lastElem(lastElem), m_comp(comp) {}

			//Copy
			map_iterator(const map_iterator<Key, T, Compare, Node, false>& copy)
			{
				m_node = copy.getNode();
				m_lastElem = copy.getLastElem();
				m_comp = copy.getCompare();
			}

			//Destroy
			~map_iterator() {}

			//Operator=
			map_iterator& operator=(const map_iterator& assign)
			{
				if (this != assign.m_node)
				{
					m_node = assign.m_node;
					m_lastElem = assign.m_lastElem;
					m_comp = assign.m_comp;
				}
				return (*this);
			}

// __ Getters

			nodePtr getNode() const { return m_node; }
			nodePtr getLastElem() const { return m_lastElem; }
			key_compare getCompare() const { return m_comp; }

// __ Operators

			reference operator*() const { return (m_node->content); }
			pointer operator->() const { return (&m_node->content); }
			map_iterator& operator++()
			{
				nodePtr previousNode = m_node;
				if (m_node == m_lastElem)
				{
					m_node = m_lastElem->right;
					return (*this);
				}
				while (m_node != m_lastElem && !m_comp(previousNode->content.first, m_node->content.first))
				{
					if (m_node->right && (m_node->right == m_lastElem ||
								m_comp(previousNode->content.first, m_node->content.first)))
					{
						m_node = m_node->right;
						Node* tmp = 0;
						if (m_node != m_lastElem && (tmp = searchMinNode(m_node)))
							m_node = tmp;
					}
					else
						m_node = m_node->parent;
				}
				return (*this);
			}

			map_iterator operator++(int)
			{
				map_iterator res(*this);
				if (m_node == m_lastElem)
				{
					m_node = m_lastElem->right;
					return (res);
				}
				while (m_node != m_lastElem && !m_comp(res->first, m_node->content.first))
				{
					if (m_node->right && (m_node->right == m_lastElem ||
								m_comp(res->first, m_node->right->content.first)))
					{
						m_node = m_node->right;
						Node* tmp = 0;
						if (m_node != m_lastElem && (tmp = searchMinNode(m_node)))
							m_node = tmp;
					}
					else
						m_node = m_node->parent;
				}
				return (res);
			}

			map_iterator& operator--()
			{
				nodePtr previousNode = m_node;
				if (m_node == m_lastElem)
				{
					m_node = m_lastElem->left;
					return (*this);
				}
				while (m_node != m_lastElem && !m_comp(m_node->content.first, previousNode->content.first))
				{
					if (m_node->left && (m_node->left == m_lastElem ||
								m_comp(m_node->left->content.first, previousNode->content.first)))
					{
						m_node = m_node->left;
						Node* tmp = 0;
						if (m_node != m_lastElem && (tmp = searchMaxNode(m_node)))
							m_node = tmp;
					}
					else
						m_node = m_node->parent;
				}
				return (*this);
			}

			map_iterator operator--(int)
			{
				map_iterator res(*this);
				if (m_node == m_lastElem)
				{
					m_node = m_lastElem->left;
					return (res);
				}
				while (m_node != m_lastElem && !m_comp(m_node->content.first, res->first))
				{
					if (m_node->left && (m_node->left == m_lastElem ||
								m_comp(m_node->left->content.first, res->first)))
					{
						m_node = m_node->left;
						Node* tmp = 0;
						if (m_node != m_lastElem && (tmp = searchMaxNode(m_node)))
							m_node = m_node->parent;
					}
					return (res);
				}
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

	template<typename Key, typename T, typename Compare, typename Node, bool B>
	class rev_map_iterator
	{
		public:
			typedef Key													key_type;
			typedef Compare												key_compare;
			typedef T													mapped_type;
			typedef std::pair<const key_type, mapped_type>				value_type;
			typedef long int											difference_type;
			typedef size_t												size_type;
			typedef std::bidirectional_iterator_tag										iterator_category;
			typedef typename chooseConst<B, value_type&, const value_type&>::type		reference;
			typedef typename chooseConst<B, value_type*, const value_type*>::type		pointer;
			typedef Node*																nodePtr;
		private:
			nodePtr			m_node;
			nodePtr			m_lastElem;
			key_compare		m_comp;

// __ Constructors & Destructor

			//Default
			rev_map_iterator(nodePtr node = 0, nodePtr lastElem = 0, const key_compare& comp = key_compare()) :
				m_node(node), m_lastElem(lastElem), m_comp(comp) {}

			//Copy
			rev_map_iterator(const rev_map_iterator<Key, T, Compare, Node, false>& copy)
			{
				m_node = copy.getNonConstNode();
				m_lastElem = copy.getNonConstLastElem();
				m_comp = copy.getCompare();
			}

			//Convert
			explicit rev_map_iterator(map_iterator<Key, T, Compare, Node, false> copy)
			{
				--copy;
				m_node = copy.getNonConstNode();
				m_lastElem = copy.getNonConstLastElem();
				m_comp = copy.getCompare();
			}

			//Destroy
			~rev_map_iterator() {}

			//Operator=
			rev_map_iterator& operator=(const rev_map_iterator& assign)
			{
				if (this != &assign)
				{
					m_node = assign.m_node;
					m_lastElem = assign.m_lastElem;
					m_comp = assign.m_comp;
				}
				return (*this);
			}

// __ Getters

			nodePtr getNonConstNode() const { return m_node; }
			nodePtr getNonConstLastElem() const { return m_lastElem; }
			key_compare getCompare() const { return m_comp; }

// __ Operators

			reference operator*() const { return (m_node->content); }
			pointer operator->() const { return (&m_node->content); }

			rev_map_iterator& operator++()
			{
				nodePtr previousNode = m_node;
				if (m_node == m_lastElem)
				{
					m_node = m_lastElem->left;
					return (*this);
				}
				while (m_node != m_lastElem && !m_comp(m_node->content.first, previousNode->content.first))
				{
					if (m_node->left && (m_node->left == m_lastElem ||
								m_comp(m_node->left->content.first, previousNode->content.first)))
					{
						m_node = m_node->left;
						Node* tmp = 0;
						if (m_node != m_lastElem && (tmp = searchMaxNode(m_node)))
							m_node = tmp;
					}
					else
						m_node = m_node->parent;
				}
				return (*this);
			}

			rev_map_iterator operator++(int)
			{
				rev_map_iterator res(*this);
				if (m_node == m_lastElem)
				{
					m_node = m_lastElem->left;
					return (res);
				}
				while (m_node != m_lastElem && !m_comp(m_node->content.first, res->first))
				{
					if (m_node->left && (m_node->left == m_lastElem ||
								m_comp(m_node->left->content.first, res->first)))
					{
						m_node = m_node->left;
						Node* tmp = 0;
						if (m_node != m_lastElem && (tmp = searchMaxNode(m_node)))
							m_node = tmp;
					}
					else
						m_node = m_node->parent;
				}
				return (res);
			}

			rev_map_iterator& operator--()
			{
				nodePtr previousNode = m_node;
				if (m_node == m_lastElem)
				{
					m_node = m_lastElem->right;
					return (*this);
				}
				while (m_node != m_lastElem && !m_comp(previousNode->content.first, m_node->content.first))
				{
					if (m_node->right && (m_node->right == m_lastElem ||
								m_comp(previousNode->content.first, m_node->right->content.first)))
					{
						m_node = m_node->right;
						Node* tmp = 0;
						if (m_node != m_lastElem && (tmp = searchMinNode(m_node)))
							m_node = tmp;
					}
					else
						m_node = m_node->parent;
				}
				return (*this);
			}

			rev_map_iterator operator--(int)
			{
				rev_map_iterator res(*this);
				if (m_node == m_lastElem)
				{
					m_node = m_lastElem->right;
					return (res);
				}
				while (m_node != m_lastElem && !m_comp(res->first, m_node->content.first))
				{
					if (m_node->right && (m_node->right == m_lastElem ||
								m_comp(res->first, m_node->right->content.first)))
					{
						m_node = m_node->right;
						Node* tmp = 0;
						if (m_node != m_lastElem && (tmp = searchMinNode(m_node)))
							m_node = tmp;
					}
					else
						m_node = m_node->parent;
				}
				return (res);
			}

			bool operator==(const rev_map_iterator& it) const { return (it.m_node = m_node); }
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

}
