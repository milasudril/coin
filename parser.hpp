//@	{
//@	"targets":[{"name":"saxdriver.hpp","type":"include"}]
//@	}

#ifndef COIN_SAXDRIVER_HPP
#define COIN_SAXDRIVER_HPP

#include "token.hpp"
#include "tag.hpp"
#include "processstatus.hpp"
#include <stack>

namespace CoIN
	{
	template<class OutputFunction>
	class Parser
		{
		public:
			explicit Parser(OutputFunction&& output):r_output(output)
				{}

			explicit Parser(OutputFunction& output):r_output(output)
				{}

			template<class ErrorPolicy>
			__attribute__((warn_unused_result)) ProcessStatus operator()(const CoIN::Token& token,ErrorPolicy&& err)
				{
				switch(token.type)
					{
					case CoIN::TokenType::TagNameNoAttributes:
						m_tag_stack.push(m_tag_current);
						if(token.value=="!")
							{
							m_tag_current=Tag(token.value,token.row,token.col,err);
							r_output.commentBegin();
							return ProcessStatus::NoError;
							}

						if(token.value=="")
							{
							if(!m_tag_prev)
								{
								err(token, "No previous tag in current context.");
								return ProcessStatus::Error;
								}
							m_tag_current=m_tag_prev; //Restore previous tag
							}
						else
							{m_tag_current=Tag(token.value,token.row,token.col,err);}
						m_tag_prev.clear();
						r_output.elementBegin(m_tag_current);
						return ProcessStatus::NoError;

					case CoIN::TokenType::TagName:
						if(token.value=="!")
							{
							err(token, "Comments cannot have attributes.");
							return ProcessStatus::Error;
							}
						m_tag_stack.push(m_tag_current);
						m_tag_current=Tag(token.value,token.row,token.col,err);
						m_tag_prev.clear();
						return ProcessStatus::NoError;

					case CoIN::TokenType::BodyText:
						if(m_tag_stack.size()==0 && token.value.size()!=0)
							{
							err(token, "Body text must be written within an element.");
							return ProcessStatus::Error;
							}
						r_output.output(token.value);
						return ProcessStatus::NoError;

					case CoIN::TokenType::AttributeNameFirst:
						if(token.value.size()==0)
							{
							err(token, "An attribute name cannot be empty.");
							return ProcessStatus::Error;
							}
						m_attrib.first=token.value;
						return ProcessStatus::NoError;

					case CoIN::TokenType::AttributeName:
						if(token.value.size()==0)
							{
							err(token, "An attribute name cannot be empty.");
							return ProcessStatus::Error;
							}
						m_attrib.first=token.value;
						return ProcessStatus::NoError;

					case CoIN::TokenType::AttributeValue:
						m_attrib.second=token.value;
						m_tag_current.attributeAdd(m_attrib,err);
						return ProcessStatus::NoError;

					case CoIN::TokenType::AttributeValueLast:
						m_attrib.second=token.value;
 						m_tag_current.attributeAdd(m_attrib,err);
						r_output.elementBegin(m_tag_current);
						return ProcessStatus::NoError;

					case CoIN::TokenType::BodyTextLast:
						if(m_tag_stack.size()==0)
							{
							err(token, "No element here to end.");
							return ProcessStatus::Error;
							}
						if(m_tag_current.name()=="!")
							{r_output.commentEnd(token.value);}
						else
							{
							r_output.output(token.value);
							r_output.elementEnd(m_tag_current);
							m_tag_prev=m_tag_current;
							if(m_tag_stack.size()==1)
								{return ProcessStatus::DocumentEnd;}
							}
						m_tag_current=m_tag_stack.top();
						m_tag_stack.pop();
						return ProcessStatus::NoError;

					case CoIN::TokenType::EndOfFile:
						if(m_tag_stack.size()!=0)
							{
							err(token, "Some tags were left open.");
							return ProcessStatus::Error;
							}
						return ProcessStatus::DocumentEnd;
					}
				return ProcessStatus::Error;
				}

		private:
			Tag m_tag_prev;
			Tag m_tag_current;
			std::stack<Tag> m_tag_stack;
			Tag::Attribute m_attrib;
			OutputFunction& r_output;
		};
	}
#endif
