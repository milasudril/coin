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

namespace SoXN
	{
	class Element
		{
		public:
			typedef Node<Element,std::string> node_type;

			explicit Element(const std::string& name):m_name(name)
				{}

			explicit Element(std::string&& name):m_name(std::move(name))
				{}

			Element& append(const node_type& node)
				{
				m_children.push_back(node);
				return *this;
				}

			Element& append(node_type&& node)
				{
				m_children.push_back(std::move(node));
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
				std::for_each(m_attribs.begin(),m_attribs.end(),std::forward(f));
				return *this;
				}

			template<class Function>
			Element& visitAttributes(Function&& f)
				{
				std::for_each(m_attribs.begin(),m_attribs.end(),std::forward(f));
				return *this;
				}

		private:
			std::string m_name;
			std::map<std::string,std::string> m_attribs;
			std::vector<node_type> m_children;
		};
	}

#endif
