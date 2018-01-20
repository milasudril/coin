//@	{
//@	"targets":[{"name":"element.hpp","type":"include"}]
//@	}

#ifndef COIN_ELEMENT_HPP
#define COIN_ELEMENT_HPP

#include "node.hpp"
#include "tag.hpp"
#include <vector>
#include <cassert>

namespace CoIN
	{
	class Element
		{
		public:
			typedef Node<Element,std::string> NodeModel;

		private:
			template<class NodeContainer,class T>
			class NodePtrImpl;
			typedef std::vector<NodeModel> ChildrenStorage;

		public:
			typedef Tag::Attribute Attribute;

			template<class T>
			using NodePtr=NodePtrImpl<ChildrenStorage*,T>;

			template<class T>
			using NodePtrConst=NodePtrImpl<const ChildrenStorage*,T>;

			Element()=default;

			explicit Element(const std::string& name):m_tag(name)
				{}

			explicit Element(const Tag& tag):m_tag(tag)
				{}

			explicit Element(std::string&& name):m_tag(std::move(name))
				{}

			const std::string& name() const noexcept
				{return m_tag.name();}

			const Tag& tag() const noexcept
				{return m_tag;}

			operator bool() const noexcept
				{return m_tag.name().size()!=0;}

			template<class T>
			Element& append(T&& content)
				{
				m_children.push_back(NodeModel(std::forward<T>(content)));
				return *this;
				}

			template<class T>
			auto create(T&& content)
				{
				append(std::forward<T>(content));
				return NodePtr<T>(&m_children,m_children.size() - 1);
				}



			NodeModel& node(size_t index) noexcept
				{
				assert(index<m_children.size());
				return m_children[index];
				}

			const NodeModel& node(size_t index) const noexcept
				{
				assert(index<m_children.size());
				return m_children[index];
				}


			template<class Function>
			const Element& visitChildren(Function&& f) const
				{
				std::for_each(m_children.begin(), m_children.end(), [&f](const auto& node){visit(node,f);});
				return *this;
				}

			template<class Function>
			Element& visitChildren(Function&& f)
				{
				std::for_each(m_children.begin(), m_children.end(), [&f](auto& node){visit(node,f);});
				return *this;
				}

			auto childCount() const
				{return m_children.size();}


			const std::string& attribute(const std::string& name) const
				{return m_tag.attribute(name);}

			std::string& attribute(const std::string& name)
				{return m_tag.attribute(name);}

			std::string& attributeCreate(const std::string& name)
				{return m_tag.attributeCreate(name);}

			Element& attributeAdd(const Attribute& attrib)
				{
				m_tag.attributeAdd(attrib);
				return *this;
				}


			template<class Function>
			const Element& visitAttributes(Function&& f) const
				{
				m_tag.visitAttributes(std::forward<Function>(f));
				return *this;
				}

			template<class Function>
			Element& visitAttributes(Function&& f)
				{
				m_tag.visitAttributes(std::forward<Function>(f));
				return *this;
				}

		private:
			Tag m_tag;
			ChildrenStorage m_children;

			template<class NodeContainer,class T>
			class NodePtrImpl
				{
				public:
					NodePtrImpl() noexcept:r_nodes(nullptr),m_offset(0){}

					auto& operator*() const noexcept
						{return (*r_nodes)[m_offset].template getAs<T>();}
					auto operator->() const noexcept
						{return &(*r_nodes)[m_offset].template getAs<T>();}

					operator bool() const noexcept
						{return r_nodes!=nullptr;}

				private:
					friend class Element;

					explicit NodePtrImpl(NodeContainer nodes,size_t offset) noexcept:
						r_nodes(nodes),m_offset(offset)
						{
						assert(nodes!=nullptr);
						assert(offset < nodes->size());
						}

					NodeContainer r_nodes;
					size_t m_offset;
				};
		};

	template<class Writer>
	void write(const std::string& str, Writer&& writer)
		{writer.output(str);}

	template<class Writer>
	void write(const CoIN::Element& element, Writer&& writer)
		{
		writer.elementBegin(element.tag());
		element.visitChildren([&writer](const auto& node)
			{write(node, writer);});
		writer.elementEnd(element.tag());
		}
	}

#endif
