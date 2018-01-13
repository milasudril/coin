//@	{"targets":[{"name":"node.hpp","type":"include"}]}

#ifndef SOXN_NODE_HPP
#define SOXN_NODE_HPP

#include <utility>
#include <cassert>

namespace SoXN
	{
	template<class ElementModel,class StringModel>
	class Node //[Upgrade lang C++17]: variant
		{
		public:
			enum class Type{Element, String};

			auto type() const noexcept
				{return m_type;}

			explicit Node(const StringModel& leaf):m_type(Type::String)
				{m_string=new StringModel(leaf);}

			explicit Node(const ElementModel& e):m_type(Type::Element)
				{m_element=new ElementModel(e);}

			Node(const Node& node)
				{
				m_type=node.type();
				switch(node.type())
					{
					case Type::Element:
						m_element=new ElementModel(*node.m_element);
						break;
					case Type::String:
						m_string=new StringModel(*node.m_string);
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
				ElementModel* m_element;
				StringModel* m_string;
				};

			template<class T,bool dummy=0>
			struct GetAs
				{};

			template<bool dummy>
			struct GetAs<ElementModel,dummy>
				{
				static auto& get(Node& node) noexcept
					{return *node.m_element;}

				static const auto& get(const Node& node) noexcept
					{return *node.m_element;}
				};

			template<bool dummy>
			struct GetAs<StringModel,dummy>
				{
				static auto& get(Node& node) noexcept
					{return *node.m_string;}

				static const auto& get(const Node& node) noexcept
					{return *node.m_string;}
				};
		};

	template<class ElementModel,class StringModel,class Function>
	void visit(const Node<ElementModel,StringModel>& node,Function&& f)
		{
		typedef typename Node<ElementModel,StringModel>::Type NodeType;
		switch(node.type())
			{
			case NodeType::Element:
				f(node.template getAs<ElementModel>());
				break;
			case NodeType::String:
				f(node.template getAs<StringModel>());
				break;
			}
		}


	template<class ElementModel,class StringModel,class Function>
	void visit(Node<ElementModel,StringModel>& node,Function&& f)
		{
		typedef typename Node<ElementModel,StringModel>::Type NodeType;
		switch(node.type())
			{
			case NodeType::Element:
				f(node.template getAs<ElementModel>());
				break;
			case NodeType::String:
				f(node.template getAs<StringModel>());
				break;
			}
		}
	}

#endif
