//@	{
//@	"targets":[{"name":"tag.hpp","type":"include"}]
//@	}

#ifndef COIN_TAG_HPP
#define COIN_TAG_HPP

#include "errorpolicy.hpp"
#include <string>
#include <map>
#include <algorithm>

namespace CoIN
	{
	class Tag
		{
		public:
			typedef std::pair<std::string,std::string> Attribute;

			Tag()=default;

			template<class ErrorPolicy=ErrorPolicyDefault_>
			explicit Tag(const std::string& name, int row=0, int col=0,ErrorPolicy&& err=ErrorPolicyDefault_{}):
				m_name(name),m_row(row),m_col(col)
				{
				if(m_name=="")
					{err(*this, "Tag names must not be empty");}
				}

			template<class ErrorPolicy=ErrorPolicyDefault_>
			explicit Tag(std::string&& name, int row=0, int col=0,ErrorPolicy&& err=ErrorPolicyDefault_{})
				{
				m_name=std::move(name);
				if(m_name=="")
					{err(*this, "Tag names must not be empty");}
				}

			const std::string& name() const
				{return m_name;}

			template<class Function>
			const Tag& visitAttributes(Function&& f) const
				{
				std::for_each(m_attribs.begin(),m_attribs.end(),f);
				return *this;
				}

			template<class Function>
			Tag& visitAttributes(Function&& f)
				{
				std::for_each(m_attribs.begin(),m_attribs.end(),f);
				return *this;
				}


			template<class ErrorPolicy=ErrorPolicyDefault_>
			const std::string& attribute(const std::string& name,ErrorPolicy&& err=ErrorPolicyDefault_{}) const
				{
				auto i=m_attribs.find(name);
				if(i==m_attribs.end())
					{err(*this,"Attribute does not exisist");}
				return i->second;
				}

			template<class ErrorPolicy=ErrorPolicyDefault_>
			std::string& attribute(const std::string& name,ErrorPolicy&& err=ErrorPolicyDefault_{})
				{
				auto i=m_attribs.find(name);
				if(i==m_attribs.end())
					{err(*this,"Attribute does not exisist");}
				return i->second;
				}

			template<class ErrorPolicy=ErrorPolicyDefault_>
			std::string& attributeCreate(const std::string& name,ErrorPolicy&& err=ErrorPolicyDefault_{})
				{
				auto ip=m_attribs.insert({name,""});
				if(!ip.second)
					{err(*this,"Attribute has already been set");}
				return ip.first->second;
				}

			template<class ErrorPolicy=ErrorPolicyDefault_>
			Tag& attributeAdd(const Attribute& attrib,ErrorPolicy&& err=ErrorPolicyDefault_{})
				{
				auto ip=m_attribs.insert(attrib);
				if(!ip.second)
					{err(*this,"Attribute has already been set");}
				return *this;
				}

			operator bool() const noexcept
				{return m_name.size()!=0;}

			void clear()
				{
				m_name.clear();
				m_attribs.clear();
				}

			int row() const noexcept
				{return m_row;}

			int col() const noexcept
				{return m_col;}

		private:
			std::string m_name;
			std::map<std::string,std::string> m_attribs;
			int m_row;
			int m_col;
		};

	auto row(const Tag& tag) noexcept
		{return tag.row();}

	auto col(const Tag& tag) noexcept
		{return tag.col();}

	}
#endif
