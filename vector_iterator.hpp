/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miarzuma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 13:31:05 by miarzuma          #+#    #+#             */
/*   Updated: 2022/12/01 21:27:35 by miarzuma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iterator>

namespace ft
{
	// Integral Constant.
	template <typename T, T v>
	struct integral_constant
	{
		typedef T								value_type;
		typedef integral_constant<T, v>			type;
		static const T							value = v;
		operator T() const { return value; }
	};
	typedef ft::integral_constant<bool, true>	true_type;
	typedef ft::integral_constant<bool, false>	false_type;

	// Is Integral.
	template <typename T> struct	is_integral : ft::integral_constant<bool, false> {};

	template <> struct is_integral<bool> : ft::integral_constant<bool, true> {};
	template <> struct is_integral<char> : ft::integral_constant<char, true> {};
	template <> struct is_integral<signed char> : ft::integral_constant<signed char, true> {};
	template <> struct is_integral<unsigned char> : ft::integral_constant<unsigned char, true> {};
	template <> struct is_integral<short> : ft::integral_constant<short, true> {};
	template <> struct is_integral<unsigned short> : ft::integral_constant<unsigned short, true> {};
	template <> struct is_integral<int> : ft::integral_constant<int, true> {};
	template <> struct is_integral<unsigned int> : ft::integral_constant<unsigned int, true> {};
	template <> struct is_integral<long> : ft::integral_constant<long, true> {};
	template <> struct is_integral<unsigned long> : ft::integral_constant<unsigned long, true> {};
	template <> struct is_integral<long long> : ft::integral_constant<long long, true> {};
	template <> struct is_integral<unsigned long long> : ft::integral_constant<unsigned long long, true> {};
	template <> struct is_integral<wchar_t> : ft::integral_constant<wchar_t, true> {};
	template <> struct is_integral<char16_t> : ft::integral_constant<char16_t, true> {};

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

	// Lexicographical Compare.
	template <typename InputIterator1, typename InputIterator2>
	bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 < *first2)
				return true;
			if (*first2 < *first1)
				return false;
			++first1;
			++first2;
		}
		return (first2 != last2);
	}

	// Enable If.
	template <bool U, class T = void> struct enable_if {};
	template <class T> struct enable_if<true, T> { typedef T	type; };

	// Is Same.
	template <typename T, typename U> struct is_same : false_type {};
	template <typename T> struct is_same<T, T> : ft::true_type {};

	// Iterator Traits.
	template <class Iter> struct iterator_traits
	{
		typedef typename Iter::difference_type difference_type;
		typedef typename Iter::value_type value_type;
		typedef typename Iter::pointer pointer;
		typedef typename Iter::reference reference;
		typedef typename Iter::iterator_category iterator_category;
	};
	template <class T> struct iterator_traits<T*>
	{
		typedef std::ptrdiff_t difference_type;
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef std::random_access_iterator_tag iterator_category;
	};
	template <class T> struct iterator_traits<const T*>
	{
		typedef std::ptrdiff_t difference_type;
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef std::random_access_iterator_tag iterator_category;
	};

	// Random Access Iterator.
	template <class T> class random_access_iterator
	{
		public:
			typedef T								*pointer;
			typedef T								&reference;
			typedef T								value_type;
			typedef std::ptrdiff_t					difference_type;
			typedef std::random_access_iterator_tag	iterator_category;
			typedef T*								iterator_type;
		private:
			pointer ptr;
		public:
			random_access_iterator() : ptr(NULL) {}
			random_access_iterator(pointer ptr) : ptr(ptr) {}
			~random_access_iterator() {}
			random_access_iterator(const random_access_iterator &copy) { *this = copy; }
			random_access_iterator &operator=(const random_access_iterator &rhs)
			{
				if (this == &rhs)
					return *this;
				this->ptr = rhs.ptr;
				return *this;
			}

			pointer base() const { return ptr; }
			pointer operator->() const { return ptr; }
			reference operator*() const { return *ptr; }
			random_access_iterator &operator++()
			{
				++ptr;
				return *this;
			}
			random_access_iterator operator++(int)
			{
				random_access_iterator temp = *this;
				++ptr;
				return temp;
			}
			random_access_iterator &operator--()
			{
				--ptr;
				return *this;
			}
			random_access_iterator operator--(int)
			{
				random_access_iterator temp = *this;
				--ptr;
				return temp;
			}
			operator random_access_iterator<const T> () const
			{
				return (random_access_iterator<const T>(this->ptr));
			}
			random_access_iterator operator+(difference_type n) const { return ptr + n; }
			random_access_iterator operator-(difference_type n) const { return ptr - n; }
			difference_type operator+(const random_access_iterator &it) const { return ptr + it.ptr; }
			difference_type operator-(const random_access_iterator &it) const { return ptr - it.ptr; }
			random_access_iterator operator+=(difference_type n) { ptr += n; return *this; }
			random_access_iterator operator-=(difference_type n) { ptr -= n; return *this; }
			reference operator[](difference_type n) const { return ptr[n]; }
	};

	// Non Member Functions random access iterator.
	template <typename It1, typename It2>
	bool operator==(random_access_iterator<It1> const &lhs, 
	random_access_iterator<It2> const &rhs)
	{ return (lhs.base() == rhs.base()); }

	template <typename It1, typename It2>
	bool operator!=(random_access_iterator<It1> const &lhs,
	random_access_iterator<It2> const &rhs)
	{ return (lhs.base() != rhs.base()); }

	template <typename It1, typename It2>
	bool operator>(random_access_iterator<It1> const &lhs,
	random_access_iterator<It2> const &rhs)
	{ return (lhs.base() > rhs.base()); }

	template <typename It1, typename It2>
	bool operator<(random_access_iterator<It1> const &lhs,
	random_access_iterator<It2> const &rhs)
	{ return (lhs.base() < rhs.base()); }

	template <typename It1, typename It2>
	bool operator>=(random_access_iterator<It1> const &lhs,
	random_access_iterator<It2> const &rhs)
	{ return (lhs.base() >= rhs.base()); }

	template <typename It1, typename It2>
	bool operator<=(random_access_iterator<It1> const &lhs,
	random_access_iterator<It2> const &rhs)
	{ return (lhs.base() <= rhs.base()); }

    template <typename T>
    typename ft::random_access_iterator<T>::difference_type
    operator-(const ft::random_access_iterator<T> lhs,
    const ft::random_access_iterator<T> rhs)
    { return (lhs.base() - rhs.base()); }

    template<typename T_L, typename T_R>
    typename ft::random_access_iterator<T_L>::difference_type
    operator-(const ft::random_access_iterator<T_L> lhs,
    const ft::random_access_iterator<T_R> rhs)
    { return (lhs.base() - rhs.base()); }

	template <typename T> random_access_iterator<T>
	operator+(typename random_access_iterator<T>::difference_type n,
	const random_access_iterator<T> &rhs)
	{ return (rhs + n); }

    template <typename T>
	typename random_access_iterator<T>::difference_type
    operator-(const random_access_iterator<T> &lhs, const random_access_iterator<T> &rhs)
    { return (lhs.base() - rhs.base()); }

	// Reverse Iterator.
	template <class Iter> class reverse_iterator
	{
		public:
			typedef Iter												iterator_type;
			typedef typename iterator_traits<Iter>::iterator_category	iterator_category;
			typedef typename iterator_traits<Iter>::value_type			value_type;
			typedef typename iterator_traits<Iter>::difference_type		difference_type;
			typedef typename iterator_traits<Iter>::pointer				pointer;
			typedef typename iterator_traits<Iter>::reference			reference;
		private:
			iterator_type	baseIter;
		public:
			reverse_iterator() : baseIter() {}
			explicit reverse_iterator(iterator_type iter) : baseIter(iter) {}
			template<typename U>
			reverse_iterator(const reverse_iterator<U> &iter) : baseIter(iter.base()) {}

			template <typename U> reverse_iterator& operator=(const reverse_iterator &other)
			{
				if (this != &other)
					this->baseIter = other.base();
				return *this;
			}
			iterator_type		base() const
			{ return baseIter; }
			reference			operator*() const
			{
				iterator_type tmp = baseIter;
				return *--tmp;
			}
			pointer				operator->() const
			{
				iterator_type tmp(baseIter);
				--tmp;
				return tmp.operator->();
			}
			reference			operator[](difference_type n) const
			{ return base()[-n-1]; }
			reverse_iterator	&operator++()
			{
				--baseIter;
				return *this;
			}
			reverse_iterator	operator++(int)
			{
				reverse_iterator temp = *this;
				--baseIter;
				return temp;
			}
			reverse_iterator	&operator--()
			{
				++baseIter;
				return *this;
			}
			reverse_iterator	operator--(int)
			{
				reverse_iterator temp = *this;
				++baseIter;
				return temp;
			}
			reverse_iterator	operator+(difference_type n) const
			{ return reverse_iterator(base() - n); }
			reverse_iterator	operator-(difference_type n) const
			{ return reverse_iterator(base() + n); }
			reverse_iterator	operator+=(difference_type n)
			{
				baseIter -= n;
				return *this;
			}
			reverse_iterator	operator-=(difference_type n)
			{
				baseIter += n;
				return *this;
			}

	};

	// Non Member Functions reverse iterator.
	template <typename It1, typename It2>
	bool operator==(const reverse_iterator<It1> &lhs,
	const reverse_iterator<It2> &rhs)
	{ return (lhs.base() == rhs.base()); }

	template <typename It1, typename It2>
	bool operator!=(const reverse_iterator<It1> &lhs,
	const reverse_iterator<It2> &rhs)
	{ return (lhs.base() != rhs.base()); }

	template <typename It1, typename It2>
	bool operator>(const reverse_iterator<It1> &lhs,
	const reverse_iterator<It2> &rhs)
	{ return (lhs.base() < rhs.base()); }

	template <typename It1, typename It2>
	bool operator<(const reverse_iterator<It1> &lhs,
	const reverse_iterator<It2> &rhs)
	{ return (lhs.base() > rhs.base()); }

	template <typename It1, typename It2>
	bool operator>=(const reverse_iterator<It1> &lhs,
	const reverse_iterator<It2> &rhs)
	{ return (lhs.base() <= rhs.base()); }

	template <typename It1, typename It2>
	bool operator<=(const reverse_iterator<It1> &lhs,
	const reverse_iterator<It2> &rhs)
	{ return (lhs.base() >= rhs.base()); }

	template <typename Iter> reverse_iterator<Iter>
	operator+(typename reverse_iterator<Iter>::difference_type n,
	const reverse_iterator<Iter> &it)
	{ return reverse_iterator<Iter>(it.base() - n); }

	template <typename Iterator>
	typename reverse_iterator<Iterator>::difference_type
	operator-(const reverse_iterator<Iterator> &lhs,
	const reverse_iterator<Iterator> &rhs)
	{ return rhs.base() - lhs.base(); }

	template <typename Iterator, typename Iterator1>
	typename reverse_iterator<Iterator>::difference_type
	operator-(const reverse_iterator<Iterator> &lhs,
	const reverse_iterator<Iterator1> &rhs)
	{ return rhs.base() - lhs.base(); }

	// Distance.
	template <typename InputIt>
	typename ft::iterator_traits<InputIt>::difference_type
	distance(InputIt first, InputIt last)
	{
		typename ft::iterator_traits<InputIt>::difference_type	dist = 0;

		while (first != last)
		{
			++dist;
			++first;
		}
		return (dist);
	}
}
