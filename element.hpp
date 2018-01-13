//@	{
//@	"targets":[{"name":"element.hpp","type":"include"}]
//@	}

#ifndef SOXN_ELEMENT_HPP
#define SOXN_ELEMENT_HPP

#include "node.hpp"
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cassert>

namespace SoXN
	{
	class Element
		{
		public:
			typedef Node<Element,std::string> NodeModel;
			typedef std::pair<std::string,std::string> Attribute;

			template<class T>
			class NodePtr
				{
				public:
				private:
					explicit NodePtr(const std::vector<NodeModel>& nodes,size_t offset) noexcept:
						r_nodes(nodes),m_offset(offset)
						{assert(offset < nodes.size());}

					const std::vector<NodeModel>& r_nodes;
					size_t m_offset;
				};

			explicit Element(const std::string& name):m_name(name)
				{}

			explicit Element(std::string&& name):m_name(std::move(name))
				{}

			const std::string& name() const noexcept
				{return m_name;}

			std::string& name() noexcept
				{return m_name;}

			Element& append(const NodeModel& node)
				{
				m_children.push_back(node);
				return *this;
				}

			Element& append(NodeModel&& node)
				{
				m_children.push_back(std::move(node));
				return *this;
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
				std::for_each(m_children.begin(),m_children.end(),std::forward<Function>(f));
				return *this;
				}

			template<class Function>
			Element& visitChildren(Function&& f)
				{
				std::for_each(m_children.begin(),m_children.end(),std::forward<Function>(f));
				return *this;
				}


			const std::string& attribute(const std::string& name) const
				{
				auto i=m_attribs.find(name);
				if(i==m_attribs.end())
					{abort();}
				return i->second;
				}

			std::string& attribute(const std::string& name)
				{
				auto i=m_attribs.find(name);
				if(i==m_attribs.end())
					{abort();}
				return i->second;
				}

			std::string& attributeAdd(const std::string& name)
				{
				auto ip=m_attribs.insert({name,""});
				if(!ip.second)
					{abort();}
				return ip.first->second;
				}

			template<class Function>
			const Element& visitAttributes(Function&& f) const
				{
				std::for_each(m_attribs.begin(),m_attribs.end(),std::forward<Function>(f));
				return *this;
				}

			template<class Function>
			Element& visitAttributes(Function&& f)
				{
				std::for_each(m_attribs.begin(),m_attribs.end(),std::forward<Function>(f));
				return *this;
				}

		private:
			std::string m_name;
			std::map<std::string,std::string> m_attribs;
			std::vector<NodeModel> m_children;
		};
	}

#endif
