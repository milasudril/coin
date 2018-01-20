//@	{
//@	"targets":[{"name":"tag.hpp","type":"include"}]
//@	}

#ifndef COIN_TAG_HPP
#define COIN_TAG_HPP

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

			explicit Tag(const std::string& name, int row=0, int col=0):m_name(name),m_row(row),m_col(col)
				{}

			explicit Tag(std::string&& name)
				{
				if(name=="")
					{abort();}
				m_name=std::move(name);
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

			std::string& attributeCreate(const std::string& name)
				{
				auto ip=m_attribs.insert({name,""});
				if(!ip.second)
					{abort();}
				return ip.first->second;
				}

			Tag& attributeAdd(const Attribute& attrib)
				{
				auto ip=m_attribs.insert(attrib);
				if(!ip.second)
					{abort();}
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
	}
#endif
