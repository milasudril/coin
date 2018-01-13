//@	{"targets":[{"name":"node.hpp","type":"include"}]}

#ifndef SOXN_NODE_HPP
#define SOXN_NODE_HPP

#include <utility>
#include <cassert>
#include <cstdio>

namespace SoXN
	{
	template<class ElementType,class StringType>
	class Node //[Upgrade lang C++17]: variant
		{
		public:
			enum class Type{Element, String};
			typedef ElementType element_type;
			typedef StringType string_type;

			auto type() const noexcept
				{return m_type;}

			explicit Node(const StringType& leaf):m_type(Type::String)
				{m_string=new StringType(leaf);}

			explicit Node(const ElementType& e):m_type(Type::Element)
				{m_element=new ElementType(e);}

			Node(const Node& node)
				{
				m_type=node.type();
				switch(node.type())
					{
					case Type::Element:
						m_element=new ElementType(*node.m_element);
						break;
					case Type::String:
						m_string=new StringType(*node.m_string);
						break;
					}
				}

			~Node() noexcept
				{
				switch(m_type)
					{
					case Type::Element:
						delete m_element;
						break;
					case Type::String:
						delete m_string;
						break;
					}
				}


			Node(Node&& node) noexcept:m_type(node.m_type),m_element(node.m_element)
				{
				node.m_element=nullptr;
				}

			Node& operator=(Node&& node) noexcept
				{
				m_type=node.m_type;
				std::swap(m_element,node.m_element);
				return *this;
				}

			Node& operator=(const Node& node)
				{
				Node temp(node);
				*this=std::move(temp);
				return *this;
				}

			template<class T>
			auto& getAs() const noexcept
				{return GetAs<T>::get(*this);}


			template<class T>
			auto& getAs() noexcept
				{return GetAs<T>::get(*this);}

		private:
			Type m_type;
			union
				{
				ElementType* m_element;
				StringType* m_string;
				};

			template<class T,bool dummy=0>
			struct GetAs
				{};

			template<bool dummy>
			struct GetAs<ElementType,dummy>
				{
				static auto& get(Node& node) noexcept
					{return *node.m_element;}

				static const auto& get(const Node& node) noexcept
					{return *node.m_element;}
				};

			template<bool dummy>
			struct GetAs<StringType,dummy>
				{
				static auto& get(Node& node) noexcept
					{return *node.m_string;}

				static const auto& get(const Node& node) noexcept
					{return *node.m_string;}
				};
		};
	}

#endif
