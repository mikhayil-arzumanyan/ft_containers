/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miarzuma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 13:29:52 by miarzuma          #+#    #+#             */
/*   Updated: 2022/11/05 20:50:26 by miarzuma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "vector.hpp"
#include "vector_iterator.hpp"

namespace ft
{
	template <typename T, typename Container = ft::vector<T> >
	class stack
	{
		public:
			//Member types
			typedef Container							container_type;
			typedef typename Container::value_type		value_type;
			typedef typename Container::size_type		size_type;
			typedef typename Container::reference		reference;
			typedef typename Container::const_reference	const_reference;

		protected:
			//Member objects
			container_type						c;

		public:
			//constructers and Destructer
			explicit stack(const Container& cont = Container()) { this->c = cont; }
			stack(const stack& other) { this->c = other.c; }
			~stack() { this->c.clear(); }
			stack& operator=(const stack& other)
			{
				if (this != &other)
					this->c = other.c;
				return (*this);
			}

			//Member functions
			reference top() { return this->c.back(); }
			const_reference top() const { return this->c.back(); }
			bool empty() const { return this->c.empty(); }
			size_type size() const { return this->c.size(); }
			void push(const value_type& value) { return this->c.push_back( value); }
			void pop() { return this->c.pop_back(); }


			//Non-member function
			template <typename U, typename Cont>
			friend bool operator==(const stack<U, Cont>& lhs, const stack<U, Cont>& rhs)
			{ return (lhs.c == rhs.c); }

			template <typename U, typename Cont>
			friend bool operator!=(const stack<U, Cont>& lhs, const stack<U, Cont>& rhs)
			{ return (lhs.c != rhs.c); }

			template <typename U, typename Cont>
			friend bool operator>(const stack<U, Cont>& lhs, const stack<U, Cont>& rhs)
			{ return (lhs.c > rhs.c); }

			template <typename U, typename Cont>
			friend bool operator>=(const stack<U, Cont>& lhs, const stack<U, Cont>& rhs)
			{ return (lhs.c >= rhs.c); }

			template <typename U, typename Cont>
			friend bool operator<(const stack<U, Cont>& lhs, const stack<U, Cont>& rhs)
			{ return (lhs.c < rhs.c); }

			template <typename U, typename Cont>
			friend bool operator<=(const stack<U, Cont>& lhs, const stack<U, Cont>& rhs)
			{ return (lhs.c <= rhs.c); }
	};
}
