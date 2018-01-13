//@	{
//@	"targets":[{"name":"saxdriver.hpp","type":"include"}]
//@	}

#ifndef SOXN_SAXDRIVER_HPP
#define SOXN_SAXDRIVER_HPP

#include "token.hpp"
#include "tag.hpp"
#include <stack>

namespace SoXN
	{
	template<class EventHandler>
	class SAXDriver
		{
		public:
			explicit SAXDriver(EventHandler&& eh):r_eh(eh)
				{}

			explicit SAXDriver(EventHandler& eh):r_eh(eh)
				{}

			void operator()(const SoXN::Token& token)
				{
				switch(token.type)
					{
					case SoXN::TokenType::TagNameNoAttributes:
						m_tag_stack.push(m_tag_current);
						if(token.value=="")
							{
							m_tag_current=m_tag_prev; //Restore previous tag
							r_eh.outputBegin(m_tag_prev);
							}
						else
							{
							m_tag_current=Tag(token.value);
							r_eh.outputBegin(m_tag_current);
							}
						break;

					case SoXN::TokenType::TagName:
						m_tag_stack.push(m_tag_current);
						m_tag_current=Tag(token.value);
						break;

					case SoXN::TokenType::BodyText:
						r_eh.output(token.value);
						break;

					case SoXN::TokenType::AttributeNameFirst:
						m_attrib.first=token.value;
						break;

					case SoXN::TokenType::AttributeName:
						m_attrib.first=token.value;
						break;

					case SoXN::TokenType::AttributeValue:
						m_attrib.second=token.value;
						m_tag_current.attributeAdd(m_attrib);
						break;

					case SoXN::TokenType::AttributeValueLast:
						m_attrib.second=token.value;
						m_tag_current.attributeAdd(m_attrib);
						r_eh.outputBegin(m_tag_current);
						break;

					case SoXN::TokenType::BodyTextLast:
						if(m_tag_stack.size()==0)
							{abort();}
						r_eh.output(token.value);
						r_eh.outputEnd(m_tag_current);
						m_tag_prev=m_tag_current;
						m_tag_current=m_tag_stack.top();
						m_tag_stack.pop();
						break;

					case SoXN::TokenType::EndOfFile:
						if(m_tag_stack.size()!=0)
							{abort();}
						break;
					}
				}

		private:
			EventHandler& r_eh;
			Tag m_tag_prev;
			Tag m_tag_current;
			std::stack<Tag> m_tag_stack;
			Tag::Attribute m_attrib;
		};
	}
#endif
