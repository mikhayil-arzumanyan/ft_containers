/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miarzuma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 12:17:38 by miarzuma          #+#    #+#             */
/*   Updated: 2022/11/30 17:59:08 by miarzuma         ###   ########.fr       */
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
	template<typename T>
	struct Node
	{
		T							content;
		Node*						parent;
		Node*						left;
		Node*						right;

		// Searches for the element with the highest key in the tree.
		static Node* searchMaxNode(Node *root, Node* m_lastElem)
		{
			while (root && root != m_lastElem && root->right && root->right != m_lastElem)
				root = root->right;
			return root;
		}

		// Searches for the element with the lowest key in the tree.
		static Node* searchMinNode(Node *root, Node* m_lastElem)
		{
			while (root && root != m_lastElem && root->left && root->left != m_lastElem)
				root = root->left;
			return root;
		}

		// Operator++
		static Node* increment(Node* m_node, Node* m_lastElem)
		{
			Node* node = m_node;
			if (node == m_lastElem)
			{
				node = node->parent;
				node = Node::searchMinNode(node, m_lastElem);
			}
			else if (node->right && node->right != m_lastElem)
				node = Node::searchMinNode(node->right, m_lastElem);
			else
			{
				Node* tmp_node = node->parent;
				while(tmp_node != m_lastElem && tmp_node->right == m_node)
				{
					m_node = tmp_node;
					tmp_node = tmp_node->parent;
				}
				node = tmp_node;
			}
			return node;

		}

		// Operator--
		static Node* decrement(Node* m_node, Node* m_lastElem)
		{
			Node* node = m_node;
			if (node == m_lastElem)
			{
				node = node->parent;
				node = Node::searchMaxNode(node, m_lastElem);
			}
			else if (node->left && node->left != m_lastElem)
				node = Node::searchMaxNode(node->left, m_lastElem);
			else
			{

				Node* tmp_node = node->parent;
				while(tmp_node != m_lastElem && tmp_node->left == m_node)
				{
					m_node = tmp_node;
					tmp_node = tmp_node->parent;
				}
				node = tmp_node;
			}
			return node;
		}
	};
	template<
		typename Key,
		typename T,
		typename Compare = ft::less<Key>,
		typename Allocator = std::allocator<ft::pair<const Key, T> > >
	class map
	{
			typedef ft::Node<ft::pair<const Key, T> >	Node;

		public:
			// Member Type.
			typedef Key									key_type;
			typedef Compare								key_compare;
			typedef Allocator							allocator_type;
			typedef T									mapped_type;

			typedef ft::pair<const Key, T>				value_type;
			typedef long int							difference_type;
			typedef size_t								size_type;

			typedef T&									reference;
			typedef const T&							const_reference;
			typedef T*									pointer;
			typedef const T*							const_pointer;

			typedef typename ft::map_iterator<value_type, false>		iterator;
			typedef typename ft::map_iterator<value_type, true>			const_iterator;
			typedef typename ft::rev_map_iterator<value_type, false>	reverse_iterator;
			typedef typename ft::rev_map_iterator<value_type, true>		const_reverse_iterator;
		public:
			// Member classes.
			class value_compare
			{
				friend class map;
				protected:
					Compare comp;
					value_compare(Compare c) : comp(c) {}
				public:
					typedef bool		result_type;
					typedef value_type	first_argument_type;
					typedef value_type	second_argument_type;
					bool operator() (const value_type a, const value_type b) const
					{
						return comp(a.first, b.first);
					}
			};

		private:
			// Attributes.
			Node*					m_root;
			Node*					m_lastElem;
			size_type				m_size;
			allocator_type			m_allocPair;
			key_compare				m_comp;
			std::allocator<Node>	m_allocNode;

// __ Constructors and Destructor

		public:
			// Default.
			explicit map(const Compare& comp = Compare(),
			const Allocator& alloc = Allocator()) : m_size(0), m_allocPair(alloc), m_comp(comp)
			{
				m_lastElem = createNode(ft::pair<const Key, T>());
				m_lastElem->left = 0;
				m_lastElem->right = 0;
				m_root = m_lastElem;
				m_lastElem->parent = m_root;
			}

			// Range.
			template<typename InputIterator>map(InputIterator first, InputIterator last,
			const Compare& comp = Compare(), const Allocator& alloc = Allocator(),
			typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = 0) :
			m_size(0), m_allocPair(alloc), m_comp(comp)
			{
				m_lastElem = createNode(ft::pair<const Key, T>());
				m_root = m_lastElem;
				m_lastElem->left = m_lastElem;
				m_lastElem->right = m_lastElem;
				for (; first != last; ++first)
					insert(*first);
			}

			// Copy.
			map(const map& other) : 
			m_size(0), m_allocPair(other.m_allocPair), m_comp(other.m_comp), m_allocNode(other.m_allocNode)
			{
				m_lastElem = createNode(ft::pair<const Key, T>());
				m_root = m_lastElem;
				m_lastElem->left = m_lastElem;
				m_lastElem->right = m_lastElem;
				for (const_iterator it = other.begin(); it != other.end(); ++it)
					insert(*it);
			}

			// Operator=.
			map &operator=(const map &other)
			{
				map tmp(other);
				this->swap(tmp);
				return *this;
			}

			// Destroy.
			~map()
			{
				clear();
				deallocateNode(m_lastElem);
			}

// __ Getters

			allocator_type get_allocator() const { return m_allocPair; }

// __ Iterators

			iterator begin()						{ return iterator(Node::searchMinNode(m_root, m_lastElem), m_lastElem); }
			const_iterator begin() const			{ return const_iterator(Node::searchMinNode(m_root, m_lastElem), m_lastElem); }
			iterator end()							{ return iterator(m_lastElem, m_lastElem); }
			const_iterator end() const				{ return const_iterator(m_lastElem, m_lastElem); }
			reverse_iterator rbegin()				{ return reverse_iterator(Node::searchMaxNode(m_root, m_lastElem), m_lastElem); }
			const_reverse_iterator rbegin() const	{ return const_reverse_iterator(Node::searchMaxNode(m_root, m_lastElem), m_lastElem); } 
			reverse_iterator rend()					{ return reverse_iterator(Node::searchMinNode(m_root, m_lastElem), m_lastElem); }
			const_reverse_iterator rend() const		{ return const_reverse_iterator(Node::searchMinNode(m_root, m_lastElem), m_lastElem); }

// __ Capacity

			// Empty.
			bool empty() const 			{ return m_size == 0; }

			// Size.
			size_type size() const 		{ return m_size; }

			// Max.
			size_type max_size() const
			{
				return m_allocPair.max_size();
			}

// __ Element Access

			// At
			T& at(const Key& key)
			{
				iterator it;
				it = this->find(key);
				if (it == this->end())
					throw std::out_of_range("map::at: key not found");
				return (it->second);
			}

			// At (const)
			const T& at(const Key& key) const
			{
				iterator it;
				it = this->find(key);
				if (it == this->end())
					throw std::out_of_range("map::at: key not found");
				return (it->second);
			}

			// Operator []
			T& operator[](const Key& k)
			{
				Node* tmp = searchNode(m_root, k);
				if (tmp)
					return tmp->content.second;
				value_type val = ft::make_pair<Key, T>(k, T());
				return insertNode(m_root, val)->content.second;
			}

// __ Modifiers

			// Insert one element.
			ft::pair<iterator, bool> insert (const value_type& val)
			{
				Node* elemIsPresent = searchNode(m_root, val.first);
				if (elemIsPresent && elemIsPresent != m_lastElem)
					return ft::pair<iterator, bool>(iterator(elemIsPresent, m_lastElem), false);
				++m_size;
				m_lastElem->parent = m_root;
				return (ft::pair<iterator, bool>(iterator(insertNode(m_root, val), m_lastElem), true));
			}

			// Insert one element starting from a certain position.
			iterator insert (iterator position, const value_type& val)
			{
				if (position->first > val.first)
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
				m_lastElem->parent = m_root;
				return iterator(insertNode(position.getNode(), val), m_lastElem);
			}

			// Inserts all elements.
			template <typename InputIterator>
			void insert (InputIterator first, InputIterator last,
			typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = 0)
			{
				while (first != last)
					insert(*first++);
			}

			// Removes one element.
			void erase (iterator position)
			{
				deleteNode(position.getNode(), position->first);
				--m_size;
				m_lastElem->parent = m_root;
			}

			// Removes one element on a specific key.
			size_type erase (const Key& k)
			{
				size_type ret = deleteNode(m_root, k);
				m_size -= ret;
				m_lastElem->parent = m_root;
				return ret;
			}

			// Removes a range of elements.
			void erase (iterator first, iterator last)
			{
				while (first != last)
				{
					iterator tmp(first);
					++first;
					erase(tmp);
				}
			}

			// Swaps the constent of this one.
			void swap (map& a)
			{
				swap(m_root, a.m_root);
				swap(m_lastElem, a.m_lastElem);
				swap(m_size, a.m_size);
				swap(m_comp, a.m_comp);
				swap(m_allocPair, a.m_allocPair);
				swap(m_allocNode, a.m_allocNode);
			}

			// Removes all elements.
			void clear() { erase(begin(), end()); }

// __ Observers

			// Return key comparison object.
			Compare key_comp() const { return m_comp; }

			// Return value comparison object.
			value_compare value_comp() const { return value_compare(m_comp); }

// __ Operations

			// Searches the container for an element.
			iterator find(const Key& k)
			{
				Node* tmp = searchNode(m_root, k);
				if (tmp)
					return iterator(tmp, m_lastElem);
				return end();
			}

			// Searches the container for an element (const).
			const_iterator find(const Key& k) const
			{
				Node* tmp = searchNode(m_root, k);
				if (tmp)
					return iterator(tmp, m_lastElem);
				return end();
			}

			// Count elements with a specific key.
			size_type count (const Key& k) const
			{
				Node* tmp = searchNode(m_root, k);
				return tmp ? true : false;
			}

			// Return the element whose key is not considered to go before k.
			iterator lower_bound(const Key& k)
			{
				iterator it = begin();
				for (; it != end(); ++it)
					if (!m_comp(it->first, k))
						break;
				return it;
			}

			// Return the element whose key is not considered to go before k (const).
			const_iterator lower_bound(const Key& k) const
			{
				const_iterator it = begin();
				for (; it != end(); ++it)
					if (!m__comp(it->first, k))
						break;
				return it;
			}

			// Return for the element whose key is considered to go after k.
			iterator upper_bound(const Key& k)
			{
				iterator it = begin();
				for (; it != end(); ++it)
					if (m_comp(k, it->first))
						break;
				return it;
			}

			// Return for the element whose key is considered to go after k (const).
			const_iterator upper_bound(const Key& k) const
			{
				const_iterator it = begin();
				for (; it != end(); ++it)
					if (m_comp(k, it->first))
						break;
				return it;
			}

			// Returns the bounds of a range.
			pair<iterator, iterator> equal_range(const Key& k)
			{
				iterator it = upper_bound(k);
				if (it != begin())
				{
					--it;
					if (m_comp(it->first, k) || m_comp(k, it->first))
						++it;
				}
				iterator next(it);
				if (it != end())
					++next;
				return ft::make_pair<iterator, iterator>(it, next);
			}

			// Returns the bounds of a range.
			pair<const_iterator, const_iterator> equal_range(const Key& k) const
			{
				const_iterator it = upper_bound(k);
				if (it != begin())
				{
					--it;
					if (m_comp(it->forst, k) || m_comp(k, it->first))
						++it;
				}
				const_iterator next(it);
				if (it != end())
					++next;
				return ft::make_pair<const_iterator, const_iterator>(it, next);
			}

		private:

			// Swap.
			template <typename U>
			void swap(U& a, U& b)
			{
				U tmp = a;
				a = b;
				b = tmp;
			}

// __ AVL Binary Search Tree

			// Creates a new node and assign pair.
			bool isSentinel(Node* node)
			{
				return node == 0 || node == m_lastElem;
			}
			Node* createNode(const value_type& pair)
			{
				Node* newNode = m_allocNode.allocate(1);
				m_allocPair.construct(&newNode->content, pair);
				newNode->parent = m_lastElem;
				newNode->left = m_lastElem;
				newNode->right = m_lastElem;
				return newNode;
			}

			// Calls the destructor.
			void deallocateNode(Node* del)
			{
				m_allocPair.destroy(&del->content);
				m_allocNode.deallocate(del, 1);
			}

			// Calculates the tree's height.
			int heightTree(Node *root, int height)
			{
				if (!root || root == m_lastElem)
					return height - 1;
				int leftHeight = heightTree(root->left, height + 1);
				int rightHeight = heightTree(root->right, height + 1);
				return leftHeight > rightHeight ? leftHeight : rightHeight;
			}

			// Searches key in the tree and returns the element if it finds key.
			Node* searchNode(Node* root, Key k) const
			{
				if (!root || root == m_lastElem)
					return 0;
				if (!m_comp(root->content.first, k) && !m_comp(k, root->content.first))
					return root;
				if (root->content.first > k && root->left && root->left != m_lastElem)
					return searchNode(root->left, k);
				else if (root->content.first < k && root->right && root->right != m_lastElem)
					return searchNode(root->right, k);
				return 0;
			}

			
			// Inserts a pair in the tree or a specific subtree by adding a new element, and 
            // then equilibrates the AVL tree if necessary. If element is already present, 
            // does nothing and return NULL.
			Node* insertNode(Node* insertPos, const value_type& pair)
			{
				if (m_root == m_lastElem)
				{
					m_root = createNode(pair);
					m_root->left = m_lastElem;
					m_root->right = m_lastElem;
					m_root->parent = m_lastElem;
					return m_root;
				}
				if (insertPos->content.first == pair.first)
					return 0;
				if (insertPos->content.first > pair.first && insertPos->left && insertPos->left != m_lastElem)
					return insertNode(insertPos->left, pair);
				else if (insertPos->content.first < pair.first && insertPos->right && insertPos->right != m_lastElem)
					return insertNode(insertPos->right, pair);
				Node *newNode = createNode(pair);
				if (insertPos->content.first > newNode->content.first && (!insertPos->left || insertPos->left == m_lastElem))
					insertPos->left = newNode;
				else if (insertPos->content.first < newNode->content.first && (!insertPos->right || insertPos->right == m_lastElem))
					insertPos->right = newNode;
				else if (insertPos->left && insertPos->left != m_lastElem && insertPos->content.first > newNode->content.first)
				{
					newNode->left = m_lastElem;
					m_lastElem->right = newNode;
					insertPos->left = newNode;
				}
				else if (insertPos->right && insertPos->right != m_lastElem && insertPos->content.first < newNode->content.first)
				{
					newNode->right = m_lastElem;
					m_lastElem->left = newNode;
					insertPos->right = newNode;
				}
				newNode->parent = insertPos;
				balanceTheTree(&m_root, newNode);
				m_root->parent = m_lastElem;
				return newNode;
			}

			//
			void transplant(Node* node_1, Node* node_2)
			{
				if (isSentinel(node_1->parent))
					m_root = node_2;
				else if (node_1 == node_1->parent->left)
					node_1->parent->left = node_2;
				else
					node_1->parent->right = node_2;
				if (!isSentinel(node_2))
					node_2->parent = node_1->parent;
			}

			//
			void treeDelete(Node* del)
			{
				if (isSentinel(del->left))
					transplant(del, del->right);
				else if (isSentinel(del->right))
					transplant(del, del->left);
				else
				{
					Node* minNode = Node::searchMinNode(del->right, m_lastElem);
					if (minNode->parent != del)
					{
						transplant(minNode, minNode->right);
						minNode->right = del->right;
						minNode->right->parent = minNode;
					}
					transplant(del, minNode);
					minNode->left = del->left;
					minNode->left->parent = minNode;
				}
			}


			// Deletes the node that matches key from the tree or a specific subtree, and then equilibrates the 
            // AVL tree if necessary. If element is missing, this function does nothing.
			bool deleteNode(Node* deletePos, Key k)
			{
				Node* balanceNode = 0;
				Node* del = searchNode(deletePos, k);
				if (!del || del == m_lastElem)
					return false;
				treeDelete(del);
				balanceNode = del->parent;
			//	if (!del->parent || del->parent == m_lastElem)
			//	{
			//		if (del->left == m_lastElem && del->right == m_lastElem)
			//		{
			//			m_root = m_lastElem;
			//			m_lastElem->left = m_lastElem;
			//			m_lastElem->right = m_lastElem;
			//		}
			//		else if (del->left && del->left != m_lastElem && del->right == m_lastElem)
			//		{
			//			balanceNode = del->parent;
			//			m_root = del->left;
			//			del->left->parent = m_lastElem;
			//			m_lastElem->left = del->left;
			//			del->left->right = m_lastElem;
			//		}
			//		else if (del->left == m_lastElem && del->right && del->right != m_lastElem)
			//		{
			//			balanceNode = del->parent;
			//			m_root = del->right;
			//			del->right->parent = m_lastElem;
			//			m_lastElem->right = del->right;
			//			del->right->left = m_lastElem;
			//		}
			//		else
			//		{
			//			Node* maxNode = Node::searchMaxNode(del->left, m_lastElem);
			//			m_allocPair.destroy(&del->content);
			//			m_allocPair.construct(&del->content, maxNode->content);
			//			return deleteNode(del->left, maxNode->content.first);
			//		}
			//	}
			//	else if ((!del->left || del->left == m_lastElem) && (!del->right || del->right == m_lastElem))
			//	{
			//		balanceNode = del->parent;
			//		Node* linkToParent = 0;
			//		if (del->left == m_lastElem || del->right == m_lastElem)
			//		{
			//			linkToParent = m_lastElem;
			//			del->content.first <= del->parent->content.first ? m_lastElem->right = del->parent :
			//				m_lastElem->left = del->parent;
			//		}
			//		del->content.first <= del->parent->content.first ? del->parent->left = del->parent :
			//			del->parent->right = linkToParent;
			//	}
			//	else if ((del->left && del->left != m_lastElem) && (!del->right || del->right == m_lastElem))
			//	{
			//		balanceNode = del->parent;
			//		del->content.first <= del->parent->content.first ? del->parent->left = del->left :
			//			del->parent->right = del->left;
			//		del->left->parent = del->parent;
			//		if (del->right == m_lastElem)
			//		{
			//			m_lastElem->left = del->left;
			//			del->left->right = m_lastElem;
			//		}
			//	}
			//	else if ((!del->left || del->left == m_lastElem) && del->right && del->right != m_lastElem)
			//	{
			//		balanceNode = del->parent;
			//		del->content.first <= del->parent->content.first ? del->parent->left = del->right :
			//			del->parent->right = del->right;
			//		del->right->parent = del->parent;
			//		if (del->left == m_lastElem)
			//		{
			//			m_lastElem->right = del->right;
			//			del->right->left = m_lastElem;
			//		}
			//	}
			//	else
			//	{
			//		Node* maxNode = Node::searchMaxNode(del->left, m_lastElem);
			//		m_allocPair.destroy(&del->content);
			//		m_allocPair.construct(&del->content, maxNode->content);
			//		return deleteNode(del->left, maxNode->content.first);
			//	}
				balanceTheTree(&m_root, balanceNode);
				//deallocateNode(del);
				return true;
			}

			// Compares the heights of left and right subtrees.
			int balanceOfSubtrees(Node* node)
			{
				if (!node || node == m_lastElem)
					return 0;
				return heightTree(node->left, 1) - heightTree(node->right, 1);
			}

			// RIGHT ROTATION
			// Does a right rotation between a node and his left child. The left child will go up and take 
			// the position of this node, and this node will become the right child of the node going up.
			void rotateRight(Node** root, Node* nodeGoingDown)
			{
				Node* nodeGoingUp = nodeGoingDown->left;
				nodeGoingDown->left = nodeGoingUp->right;
				if (nodeGoingUp->right)
					nodeGoingUp->right->parent = nodeGoingDown;
				nodeGoingUp->right = nodeGoingDown;
				nodeGoingUp->parent = nodeGoingDown->parent;
				if (nodeGoingDown->parent && nodeGoingDown->parent->left == nodeGoingDown)
					nodeGoingDown->parent->left = nodeGoingUp;
				else if (nodeGoingDown->parent)
					nodeGoingDown->parent->right = nodeGoingUp;
				nodeGoingDown->parent = nodeGoingUp;
				if (!nodeGoingUp->parent)
					*root = nodeGoingUp;
			}

			// LEFT ROTATION
			// Does a left rotation between a node and his right child. The right child will go up and take
			// the position of this node; and this node will become the left child of the node going up.
			void rotateLeft(Node** root, Node* nodeGoingDown)
			{
				Node* nodeGoingUp = nodeGoingDown->right;
				nodeGoingDown->right = nodeGoingUp->left;
				if (nodeGoingUp->left)
					nodeGoingUp->left->parent = nodeGoingDown;
				nodeGoingUp->left = nodeGoingDown;
				nodeGoingUp->parent = nodeGoingDown->parent;
				if (nodeGoingDown->parent && nodeGoingDown->parent->left == nodeGoingDown)
					nodeGoingDown->parent->left = nodeGoingUp;
				else if (nodeGoingDown->parent)
					nodeGoingDown->parent->right = nodeGoingUp;
				nodeGoingDown->parent = nodeGoingUp;
				if (!nodeGoingUp->parent)
					*root = nodeGoingUp;
			}

			// Starts from a node in the AVL tree, and will check for this node and all the parent's node
            //  until root if their balance (height of left and right subtree) is correct. If not, a rotation
            //  (left or right) around the unbalanced node will occured in order to restore tree's balance.
			void balanceTheTree(Node** root, Node* node)
			{
				while (!isSentinel(node))
				{
					int balance = balanceOfSubtrees(node);
					if (balance < -1 && balanceOfSubtrees(node->right) < 0)
						rotateLeft(root, node);
					else if (balance < -1 && balanceOfSubtrees(node->right) >= 0)
					{
						rotateRight(root, node->right);
						rotateLeft(root, node);
					}
					else if (balance > 1 && balanceOfSubtrees(node->left) > 0)
						rotateRight(root, node);
					else if (balance > 1 && balanceOfSubtrees(node->left) <= 0)
					{
						rotateLeft(root, node->left);
						rotateRight(root, node);
					}
					node = node->parent;
				}
			}
	};

	// Non member functions
	template <typename Key, typename T, typename Compare, typename Alloc>
	void swap(ft::map<Key, T, Compare, Alloc> &lhs,
	ft::map<Key, T, Compare, Alloc> &rhs)
	{ lhs.swap(rhs); }

	template <typename Key, typename T, typename Compare, typename Alloc>
	bool operator==(const ft::map<Key, T, Compare, Alloc> &lhs,
	const ft::map<Key, T, Compare, Alloc> &rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template <typename Key, typename T, typename Compare, typename Alloc>
	bool operator!=(const ft::map<Key, T, Compare, Alloc> &lhs,
	const ft::map<Key, T, Compare, Alloc> &rhs)
	{ return (!(lhs == rhs)); }

	template <typename Key, typename T, typename Compare, typename Alloc>
	bool operator<(const ft::map<Key, T, Compare, Alloc> &lhs,
	const ft::map<Key, T, Compare, Alloc> &rhs)
	{ return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())); }

	template <typename Key, typename T, typename Compare, typename Alloc>
	bool operator<=(const ft::map<Key, T, Compare, Alloc> &lhs,
	const ft::map<Key, T, Compare, Alloc> &rhs)
	{ return (!(rhs < lhs)); }

	template <typename Key, typename T, typename Compare, typename Alloc>
	bool operator>(const ft::map<Key, T, Compare, Alloc> &lhs,
	const ft::map<Key, T, Compare, Alloc> &rhs)
	{ return (rhs < lhs); }

	template <typename Key, typename T, typename Compare, typename Alloc>
	bool operator>=(const ft::map<Key, T, Compare, Alloc> &lhs,
	const ft::map<Key, T, Compare, Alloc> &rhs)
	{ return (!(lhs < rhs)); }
}
