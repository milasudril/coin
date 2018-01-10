//@	{"targets":[{"name":"node.hpp","type":"include"}]}

#ifndef SOXN_NODE_HPP
#define SOXN_NODE_HPP

#include "element.hpp"
#include <type_traits>
#include <cassert>

namespace SoXN
	{
	class Node
		{
		public:
			enum class Type{Element, String};

			auto type() const noexcept
				{return m_type;}

			explicit Node(const std::string& leaf):m_type(Type::String)
				{m_string=new std::string(leaf);}

			explicit Node(const Element& e):m_type(Type::Element)
				{m_element=new Element(e);}

			Node(Node&& node) noexcept:m_type(node.m_type),m_element(node.m_element)
				{node.m_element=nullptr;}

			Node& operator=(Node&& node) noexcept
				{
				m_type=node.m_type;
				std::swap(m_element,node.m_element);
				return *this;
				}

			Node(const Node& node)
				{
				m_type=node.type();
				switch(node.type())
					{
					case Type::Element:
						m_element=new Element(*node.m_element);
						break;
					case Type::String:
						m_string=new std::string(*node.m_string);
						break;
					}
				}

			Node& operator=(const Node& node)
				{
				Node temp(node);
				*this=std::move(temp);
				return *this;
				}

			~Node()
				{
				switch(m_type)
					{
					case Type::Element:
						delete m_element;
					case Type::String:
						delete m_string;
					}
				}

			template<Type t>
			inline auto& getAs() const noexcept;

			template<Type t>
			auto& getAs() noexcept;

		private:
			Type m_type;
			union
				{
				Element* m_element;
				std::string* m_string;
				};
		};

		template<>
		inline auto& Node::getAs<Node::Type::Element>() const noexcept
			{
			assert(m_type==Type::Element);
			return *m_element;
			}

		template<>
		inline auto& Node::getAs<Node::Type::String>() const noexcept
			{
			assert(m_type==Type::String);
			return *m_string;
			}


		template<>
		inline auto& Node::getAs<Node::Type::Element>() noexcept
			{
			assert(m_type==Type::Element);
			return *m_element;
			}

		template<>
		inline auto& Node::getAs<Node::Type::String>() noexcept
			{
			assert(m_type==Type::String);
			return *m_string;
			}


	}

#endif
