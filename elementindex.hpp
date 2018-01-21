//@	{
//@	"targets":[{"name":"elementindex.hpp","type":"include"}]
//@	}

#ifndef COIN_ELEMENTINDEX_HPP
#define COIN_ELEMENTINDEX_HPP

#include "element.hpp"
#include <vector>
#include <cassert>
#include <map>

namespace CoIN
	{
	class ElementIndex
		{
		public:
			explicit ElementIndex(Element& owner);
			ElementIndex(ElementIndex&&)=delete;
			ElementIndex operator=(ElementIndex&&)=delete;

			int count(const std::string& element_name) const noexcept
				{
				auto i=m_elements.find(element_name);
				if(i==m_elements.end())
					{return 0;}
				return i->second.size();
				}

			template<class ErrorPolicy=ErrorPolicyDefault_>
			bool checkCountEquals(const std::string element_name,int n,ErrorPolicy&& err=ErrorPolicyDefault_{}) const
				{
				if(count(element_name)==n)
					{return true;}
				err(r_owner.tag(),"Element count mismatch");
				return false;
				}

			template<class ErrorPolicy=ErrorPolicyDefault_>
			Element& get(const std::string& element_name,int index,ErrorPolicy&& err=ErrorPolicyDefault_{})
				{
				auto i=m_elements.find(element_name);
				if(i==m_elements.end())
					{err(r_owner.tag(),"Element not found");}
				if(static_cast<size_t>(index) >= i->second.size())
					{err(r_owner.tag(),"Element index out of bounds");}
				return r_owner.node(i->second[index]).template getAs<Element>();
				}


			template<class ErrorPolicy=ErrorPolicyDefault_>
			const Element& get(const std::string& element_name,int index,ErrorPolicy&& err=ErrorPolicyDefault_{}) const
				{
				auto i=m_elements.find(element_name);
				if(i==m_elements.end())
					{err(r_owner.tag(),"Element not found");}
				if(static_cast<size_t>(index) >= i->second.size())
					{err(r_owner.tag(),"Element index out of bounds");}
				return r_owner.node(i->second[index]).template getAs<Element>();
				}

		private:
			Element& r_owner;
			std::map<std::string,std::vector<int>> m_elements;
		};

	ElementIndex::ElementIndex(Element& owner):r_owner(owner)
		{
		owner.visitElements([this](const auto& child,int index)
			{m_elements[child.name()].push_back(index);});
		}
	}

#endif
