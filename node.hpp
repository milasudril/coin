//@	{"targets":[{"name":"node.hpp","type":"include"}]}

#ifndef SOXN_NODE_HPP
#define SOXN_NODE_HPP

#include <utility>
#include <cassert>

namespace SoXN
	{
	enum class NodeType{Element, String};

	template<NodeType type>	class GetAs;

	template<>
	struct GetAs<NodeType::String>
		{
		template<class Node>
		static auto& get(Node& node) noexcept
			{return *node.m_string;}

		template<class Node>
		static auto& get(const Node& node) noexcept
			{return *node.m_string;}
		};

	template<>
	struct GetAs<NodeType::Element>
		{
		template<class Node>
		static auto& get(Node& node) noexcept
			{return *node.m_element;}

		template<class Node>
		static auto& get(const Node& node) noexcept
			{return *node.m_element;}
		};

	template<class ElementType,class StringType>
	class Node
		{
		public:
			typedef NodeType Type;
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
					case Type::String:
						delete m_string;
					}
				}


			Node(Node&& node) noexcept:m_type(node.m_type),m_element(node.m_element)
				{node.m_element=nullptr;}

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

			template<Type type>
			auto& getAs() const noexcept
				{return GetAs<type>::get(*this);}


			template<Type type>
			auto& getAs() noexcept
				{return GetAs<type>::get(*this);}

		private:
			Type m_type;
			union
				{
				ElementType* m_element;
				StringType* m_string;
				};

			template<Type type>
			friend class GetAs;
		};
	}

#endif
