/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miarzuma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 13:29:09 by miarzuma          #+#    #+#             */
/*   Updated: 2022/12/01 17:50:45 by miarzuma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
# define VECTOR_HPP

#include <memory>
#include <vector>
#include <cstddef>
#include <iterator>
#include "vector_iterator.hpp"

namespace ft
{
	template < typename T, typename Alloc = std::allocator<T> >
	class vector
	{
		public:
			typedef T												value_type;
			typedef std::size_t										size_type;
			typedef Alloc											allocator_type;

			typedef T&												reference;
			typedef const T&										const_reference;
			typedef T*												pointer;
			typedef const T*										const_pointer;

			typedef std::ptrdiff_t									difference_type;

			typedef ft::random_access_iterator<value_type>			iterator;
			typedef ft::random_access_iterator<const value_type>	const_iterator;
			typedef ft::reverse_iterator<iterator>					reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>			const_reverse_iterator;

		protected:
			allocator_type		m_alloc;
			size_type			m_size;
			size_type			m_capacity;
			pointer				m_data;

// __ Constructors & Destructor	

		public:
			// Default.
			vector(const allocator_type& alloc = allocator_type())
			{
				m_alloc = alloc;
				m_size = 0;
				m_capacity = 0;
				m_data = m_alloc.allocate(0);
			}
			// Fill.
			vector(size_type n, const value_type& val = value_type(), const allocator_type& alloc = Alloc())
			{
				m_alloc = alloc;
				m_size = n;
				m_capacity = n;
				m_data = this->m_alloc.allocate(m_size);
				size_type i = 0;
				while ( i < n )
					this->m_alloc.construct(m_data + i++, val);
			}
			// Copy.
			vector (const vector &copy)
			{
				m_capacity = copy.m_capacity;
				m_size = copy.m_size;
				m_alloc = copy.m_alloc;
				this->m_data = m_alloc.allocate(m_capacity);
				size_type i = 0;
				while (i < m_size)
				{
					m_alloc.construct(m_data + i, copy.m_data[i]);
					i++;
				}
			}
			// Range.
			template <typename InputIterator>
			vector(InputIterator first, InputIterator last,
			const allocator_type alloc = allocator_type(),
			typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = 0)
			{
				m_alloc = alloc;
				value_type len = ft::distance(first, last);
				m_size = len;
				m_capacity = len;
				m_data = this->m_alloc.allocate(m_capacity);
				value_type i = 0;
				while (i < len)
				{
					this->m_alloc.construct((m_data + i), *first);
					first++;
					i++;
				}
			}
			// Destroy.
			~vector()
			{
				while (m_size > 0)
					pop_back();
				m_alloc.deallocate(m_data, m_capacity);
			}

// __ Membor Functions

			// Operator=.
			vector& operator=(const vector& other)
			{
				if (this == &other)
					return *this;
				while (m_size > 0) pop_back();
				m_alloc.deallocate(m_data, m_capacity);
				this->m_capacity = other.m_capacity;
				this->m_alloc = other.m_alloc;
				this->m_size = other.m_size;
				this->m_data = m_alloc.allocate(m_capacity);
				size_type i = 0;
				while (i < m_size)
				{
					m_alloc.construct(m_data + i, other.m_data[i]);
					i++;
				}
				return *this;
			}

			// Get Allocator.
			allocator_type get_allocator() const { return m_alloc; }

			// Assign fill.
			void assign(size_type count, const value_type& val)
			{
				while (m_size > 0)
					pop_back();
				if (count > m_capacity)
				{
					m_alloc.deallocate(m_data, m_capacity);
					m_data = m_alloc.allocate(count);
					m_capacity = count;
				}
				size_type i = 0;
				while (i < count)
				{
					m_alloc.construct(m_data + i, val);
					i++;
				}
				m_size = count;
			}

			// Assign range.
			template <typename InputIt>
			void assign(InputIt first, InputIt last,
			typename enable_if<!is_integral<InputIt>::value>::type* = 0)
			{
				while (m_size > 0)
					pop_back();
				size_type count = ft::distance(first, last);
				if (count > m_capacity)
				{
					m_alloc.deallocate(m_data, m_capacity);
					m_data = m_alloc.allocate(count);
					m_capacity = count;
				}
				size_type i = 0;
				while (i < count)
				{
					m_alloc.construct(m_data + i, *first);
					first++;
					i++;
				}
				m_size = count;
			}

// __ Capasity

			// Empty.
			bool empty(void) const { return (this->m_size == 0); }

			// Size.
			size_type size() const { return this->m_size; }

			// Capacity.
			size_type capacity() const { return this->m_capacity; }

			// Max size.
			size_type max_size() const { return this->m_alloc.max_size(); }

			// Reserver.
			void reserve(size_type new_cap)
			{
				if (new_cap > max_size())
					throw std::length_error("Length error");
				if (new_cap < m_capacity)
					return;
				T* newAlloc = m_alloc.allocate(new_cap);
				size_type i = 0;
				while (i < m_size)
				{
					m_alloc.construct(newAlloc + i, m_data[i]);
					i++;
				}
				i = 0;
				while (i < m_size)
				{
					m_alloc.destroy(m_data + i);
					i++;
				}
				if (m_data)
					m_alloc.deallocate(m_data, m_capacity);
				m_capacity = new_cap;
				m_data = newAlloc;
			}

// __ Modifiers

			// Insert singl element.
			iterator insert(iterator pos, const T &value)
			{
				size_type	start;

				start = pos - this->begin();
				this->insert(pos, 1, value);
				return (this->begin() + start);
			}
	
			// Insert fill.
			void insert(iterator pos, size_type count, const T& value)
			{
				pointer		tmp;
				size_type	start;
				size_type	i;
				size_type	j;

				start =static_cast<size_type>(pos - this->begin());
				i = 0;
				j = 0;
				if (count == 0)
					return ;
				else if (pos < this->begin() && pos > this->end())
					throw std::logic_error("vector:insert error");
				else if (this->max_size() < this->m_size + count)
					throw std::length_error("vector:insert error");
				if (this->m_size + count > this->m_capacity)
				{
					size_type	tmp_cap = (m_capacity * 2 >= this->m_size + count) ? \
										 (this->m_capacity * 2) : (this->m_size + count);
					tmp = this->m_alloc.allocate(tmp_cap);
					try
					{
						for (; i < start; ++i)
							this->m_alloc.construct(tmp + i, this->m_data[i]);
						for (; j < count; ++j)
							this->m_alloc.construct(tmp + i + j, value);
						for (; i + j < (this->m_size + count); ++i)
							this->m_alloc.construct(tmp + i + j, this->m_data[i]);
						for (int t = 0; i < this->m_size; ++t)
							this->m_alloc.destroy(this->m_alloc.address(this->m_data[t]));
					}
					catch (...)
					{
						for (size_type t = 0; t < i + j; t++)
                			this->m_alloc.destroy(tmp + t);
						this->m_alloc.deallocate(tmp, this->m_size + count);
						throw ;
					}
					this->m_alloc.deallocate(this->m_data, this->m_capacity);
					this->m_capacity = tmp_cap;
					this->m_data = tmp;
				}
				else
				{
					for (size_type t = this->m_size + count - 1; t >= start; --t)
					{
						if (t >= start && t < start + count)
							this->m_alloc.construct(this->m_data + t, value);
						else if (t >= this->m_size)
							this->m_alloc.construct(this->m_data + t, this->m_data[t - count]);
						else if (t <= this->m_size && t >= start + count)
						{
							this->m_alloc.destroy(this->m_data + t);
							this->m_alloc.construct(this->m_data + t, this->m_data[t - count]);
						}
						if (t == 0)
							break ;
					}
				}
				this->m_size = this->m_size + count;
			}

			// Insert range.
			template <typename InputIt>
			void insert(iterator pos, InputIt first, InputIt last,
			typename enable_if<!is_integral<InputIt>::value, InputIt>::type* = NULL)
			{
				pointer		tmp;
				size_type	start;
				size_type	i;
				size_type	j;
				size_type	count;

				count = static_cast<size_type>(ft::distance(first, last));
				start = static_cast<size_type>(pos - this->begin());
				i = 0;
				j = 0;
				if (count == 0)
					return ;
				else if (pos < this->begin() && pos > this->end())
					throw std::logic_error("vector:insert error");
				else if (this->max_size() < this->m_size + count)
					throw std::length_error("vector:insert error");
				if (this->m_size + count > this->m_capacity)
				{
					size_type	tmp_cap = (m_capacity * 2 >= this->m_size + count) ? \
										 (this->m_capacity * 2) : (this->m_size + count);
					tmp = this->m_alloc.allocate(tmp_cap);
					try
					{
						for (; i < start; ++i)
							this->m_alloc.construct(tmp + i, this->m_data[i]);
						for (; j < count; ++j)
							this->m_alloc.construct(tmp + i + j, *(first++));
						for (; i + j < (this->m_size + count); ++i)
							this->m_alloc.construct(tmp + i + j, this->m_data[i]);
						for (int t = 0; i < this->m_size; ++t)
							this->m_alloc.destroy(this->m_alloc.address(this->m_data[t]));
					}
					catch (...)
					{
						for (size_type t = 0; t < i + j; t++)
            		    	this->m_alloc.destroy(tmp + t);
						this->m_alloc.deallocate(tmp, this->m_size + count);
						throw ;
					}
					this->m_alloc.deallocate(this->m_data, this->m_capacity);
					this->m_capacity = tmp_cap;
					this->m_data = tmp;
				}
				else
				{
					for (size_type t = this->m_size + count - 1; t >= start; --t)
					{
						if (t >= start && t < start + count)
							this->m_alloc.construct(this->m_data + t, *(--last));
						else if (t >= this->m_size)
							this->m_alloc.construct(this->m_data + t, this->m_data[t - count]);
						else if (t <= this->m_size && t >= start + count)
						{
							this->m_alloc.destroy(this->m_data + t);
							this->m_alloc.construct(this->m_data + t, this->m_data[t - count]);
						}
						if (t == 0)
							break ;
					}
				}
				this->m_size = this->m_size + count;
			}

			// Erase single element.
			iterator erase(iterator pos)
			{
				size_type index = pos - begin();
				m_alloc.destroy(m_data + index);
				size_type i = index;
				while (i < m_size - 1)
				{
					m_alloc.construct(m_data + i, m_data[i + 1]);
					i++;
				}
				m_size--;
				return iterator(m_data + index);
			}

			// Erase range.
			iterator erase(iterator first, iterator last)
			{
				size_type index = first - begin();
				size_type count = last - first;
				while (first != last)
				{
					m_alloc.destroy(first.base());
					first++;
				}
				size_type temp = index;
				while (last < this->end())
				{
					m_alloc.construct(m_data + temp, *last);
					temp++;
					last++;
				}
				m_size -= count;
				return iterator(m_data + index);
			}

			// Push Back.
			void push_back(const value_type& val)
			{
				if (m_size == m_capacity)
				{
					if (m_capacity == 0)
						m_capacity = m_capacity + 1;
					else
						m_capacity = (m_capacity * 2);
					this->reserve(m_capacity);
				}
				this->m_alloc.construct(m_data + m_size, val);
				m_size++;
			}

			// Pop Back.
			void pop_back()
			{
				m_size--;
				m_alloc.destroy(m_data + m_size);
			}

			// Clear.
			void clear()
			{
				while (m_size > 0)
					pop_back();
			}

			// Resize.
			void resize (size_type count, const value_type& val = value_type())
			{
				if (m_size > count)
				{
					size_type i = count;
					while (i > m_size)
					{
						m_alloc.destroy(m_data + i);
						i++;
					}
					m_size = count;
				}
				else if (m_size < count)
				{
					if (m_capacity < count)
					{
						value_type new_cap;
						if (m_capacity * 2 > count)
							new_cap = m_capacity * 2;
						else
							new_cap = count;
						reserve(new_cap);
					}
					size_type i = m_size;
					while (i < count)
					{
						m_alloc.construct(m_data + i, val);
						m_size++;
						i++;
					}
				}
			}

			// Swap.
			void swap(vector &x)
			{
				std::swap(this->m_data, x.m_data);
				std::swap(this->m_size, x.m_size);
				std::swap(this->m_capacity, x.m_capacity);
				std::swap(this->m_alloc, x.m_alloc);
			}

// __ Element Access

			// At.
			reference at(size_type pos)
			{
				if (pos >= m_size)
					throw std::out_of_range("Out of range");
				return m_data[pos];
			}

			// At (const).
			const_reference at (size_type pos) const
			{
				if (pos >= m_size)
					throw std::out_of_range("Out of range");
				return m_data[pos];
			}

			// Operator[].
			reference operator[](size_type pos) { return m_data[pos]; }
			const_reference operator[] (size_type pos) const { return m_data[pos]; }

			// Front.
			reference front() { return m_data[0]; }
			const_reference front() const { return m_data[0]; }

			// Back.
			reference back() { return m_data[m_size - 1]; }
			const_reference back() const { return m_data[m_size - 1]; }

			// Data.
			T* data() { return m_data; }
			const T* data() const { return m_data; }

// __ Iterators

			iterator begin() { return iterator(m_data); }
			iterator end() { return iterator(m_data + m_size); }
			const_iterator begin() const { return const_iterator(m_data); }
			const_iterator end() const { return const_iterator(m_data + m_size); }
			reverse_iterator rbegin() { return reverse_iterator(end()); }
			reverse_iterator rend() { return reverse_iterator(begin()); }
			const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
			const_reverse_iterator rend() const { return const_reverse_iterator(end()); }
	};

// __ Operators

			template <typename T, typename Alloc>
			bool operator==(const ft::vector<T,Alloc> &v1, const ft::vector<T,Alloc> &v2)
			{ return v1.size() == v2.size() && ft::equal(v1.begin(), v1.end(), v2.begin()); }

			template <typename T, typename Alloc>
			bool operator!=(const ft::vector<T,Alloc> &v1, const ft::vector<T,Alloc> &v2)
			{ return !(v1 == v2); }

			template <typename T, typename Alloc>
			bool operator>(const ft::vector<T,Alloc> &v1, const ft::vector<T,Alloc> &v2)
			{ return v2 < v1; }

			template <typename T, typename Alloc>
			bool operator<(const ft::vector<T,Alloc> &v1, const ft::vector<T,Alloc>& v2)
			{ return ft::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end()); }

			template <typename T, typename Alloc>
			bool operator>=(const ft::vector<T,Alloc> &v1, const ft::vector<T,Alloc> &v2)
			{ return !(v1 < v2); }

			template <typename T, typename Alloc>
			bool operator<=(const ft::vector<T, Alloc> &v1, const ft::vector<T,Alloc> &v2)
			{ return !(v2 < v1); }

			template <typename T, typename Alloc>
			void swap(vector<T,Alloc> &x, vector<T,Alloc> &y)
			{ x.swap(y); }
}

#endif
