//@	{
//@	"targets":[{"name":"saxdriver.hpp","type":"include"}]
//@	}

#ifndef SOXN_SAXDRIVER_HPP
#define SOXN_SAXDRIVER_HPP

#include "token.hpp"
#include "tag.hpp"
#include "errorpolicy.hpp"
#include <stack>

namespace SoXN
	{
	template<class EventHandler,class ErrorPolicy=LogAndAbort>
	class SAXDriver
		{
		public:
			explicit SAXDriver(EventHandler&& eh,ErrorPolicy err=LogAndAbort{}):r_eh(eh)
				{}

			explicit SAXDriver(EventHandler& eh):r_eh(eh)
				{}

			void operator()(const SoXN::Token& token)
				{
				switch(token.type)
					{
					case SoXN::TokenType::TagNameNoAttributes:
						m_tag_stack.push(m_tag_current);
						if(token.value=="!")
							{
							m_tag_current=Tag(token.value);
							r_eh.commentBegin();
							break;
							}

						if(token.value=="")
							{
							if(!m_tag_prev)
								{
								m_err(token, "No previous tag in current context.");
								return;
								}
							m_tag_current=m_tag_prev; //Restore previous tag
							}
						else
							{m_tag_current=Tag(token.value);}
						m_tag_prev.clear();
						r_eh.elementBegin(m_tag_current);
						break;

					case SoXN::TokenType::TagName:
						if(token.value=="!")
							{
							m_err(token, "Comments cannot have attributes.");
							return;
							}
						m_tag_stack.push(m_tag_current);
						m_tag_current=Tag(token.value);
						m_tag_prev.clear();
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
						r_eh.elementBegin(m_tag_current);
						break;

					case SoXN::TokenType::BodyTextLast:
						if(m_tag_stack.size()==0)
							{
							m_err(token, "No element here to end.");
							return;
							}
						if(m_tag_current.name()=="!")
							{r_eh.commentEnd(token.value);}
						else
							{
							r_eh.output(token.value);
							r_eh.elementEnd(m_tag_current);
							m_tag_prev=m_tag_current;
							}
						m_tag_current=m_tag_stack.top();
						m_tag_stack.pop();
						break;

					case SoXN::TokenType::EndOfFile:
						if(m_tag_stack.size()!=0)
							{m_err(token, "Some tags were left open.");}
						break;
					}
				}

		private:
			Tag m_tag_prev;
			Tag m_tag_current;
			std::stack<Tag> m_tag_stack;
			Tag::Attribute m_attrib;
			EventHandler& r_eh;
			ErrorPolicy m_err;
		};
	}
#endif
