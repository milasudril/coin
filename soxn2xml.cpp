//@	{"targets":[{"name":"soxn2xml","type":"application","cxxoptions":{"cxxversion_min":201402}}]}

#include <cstdio>
#include "tokenizer.hpp"
#include <string>
#include <stack>
#include <map>
#include <algorithm>

namespace
	{
	class Tag
		{
		public:
			typedef std::pair<std::string,std::string> attribute;

			Tag()=default;
			explicit Tag(const std::string& name)
				{
				if(name=="")
					{abort();}
				m_name=name;
				}

			const std::string& name() const
				{return m_name;}

			auto attribsBegin() const
				{return m_attributes.begin();}

			auto attribsEnd() const
				{return m_attributes.end();}

			void insert(const attribute& attrib)
				{
				if(attrib.first=="")
					{abort();}

				auto ip=m_attributes.insert(attrib);
				if(!ip.second)
					{abort();}
				}

		private:
			std::string m_name;
			std::map<std::string,std::string> m_attributes;

		};

	void output(const std::string& str)
		{
		std::for_each(str.begin(),str.end(),[](char ch_out)
			{
			switch(ch_out)
				{
				case '<':
					printf("&lt;");
					break;
				case '>':
					printf("&gt;");
					break;
				case '&':
					printf("&amp;");
					break;
				default:
					putchar(ch_out);
				}
			});
		}

	bool nameValid(const std::string& str)
		{
		return std::find_if(str.begin(),str.end(),[](char ch_check)
			{
		//	For now, only check stuff that would lead to syntactical issues
			return ch_check=='<' || ch_check=='>' || ch_check=='"' || ch_check=='=' || ch_check=='/'
				|| (ch_check>='\0' && ch_check<=' ');
			})==str.end();
		}

	void outputBegin(const Tag& tag)
		{
		if(tag.name()=="!")
			{
			printf("<!--"); //This is a comment. Since we just converts the document to XML, keep it.
			return;
			}

		if(!nameValid(tag.name()))
			{abort();}
		printf("<%s",tag.name().c_str());
		std::for_each(tag.attribsBegin(),tag.attribsEnd(),[](const auto& attrib)
			{
			printf(" %s=\"%s\"",attrib.first.c_str(),attrib.second.c_str());
			});
		printf(">");
		}

	void outputEnd(const Tag& tag)
		{
		if(tag.name()=="")
			{abort();}

		if(tag.name()=="!")
			{
			printf("-->");
			return;
			}

		printf("</%s>",tag.name().c_str());
		}

	class Writer
		{
		public:
			void operator()(const SoXN::Token& token)
				{
				switch(token.type)
					{
					case SoXN::TokenType::TagNameNoAttributes:
						m_tag_stack.push(m_tag_current);
						if(token.value=="-")
							{
							m_tag_current=m_tag_prev; //Restore previous tag
							outputBegin(m_tag_prev);
							}
						else
							{
							m_tag_current=Tag(token.value);
							outputBegin(m_tag_current);
							}
						break;

					case SoXN::TokenType::TagName:
						m_tag_stack.push(m_tag_current);
						m_tag_current=Tag(token.value);
						break;

					case SoXN::TokenType::BodyText:
						output(token.value);
						break;

					case SoXN::TokenType::AttributeNameFirst:
						m_attrib.first=token.value;
						break;

					case SoXN::TokenType::AttributeName:
						m_attrib.first=token.value;
						break;

					case SoXN::TokenType::AttributeValue:
						m_attrib.second=token.value;
						m_tag_current.insert(m_attrib);
						break;

					case SoXN::TokenType::AttributeValueLast:
						m_attrib.second=token.value;
						m_tag_current.insert(m_attrib);
						outputBegin(m_tag_current);
						break;

					case SoXN::TokenType::BodyTextLast:
						if(m_tag_stack.size()==0)
							{abort();}
						output(token.value);
						outputEnd(m_tag_current);
						m_tag_prev=m_tag_current;
						m_tag_current=m_tag_stack.top();
						m_tag_stack.pop();
						break;
					}
				}

		private:
			Tag m_tag_prev;
			Tag m_tag_current;
			std::stack<Tag> m_tag_stack;
			Tag::attribute m_attrib;
		};
	}

int main()
	{
	SoXN::tokenize(stdin,Writer{});
	return 0;
	}
