#ifndef SOXN_TREEBUILDER_HPP
#define SOXN_TREEBUILDER_HPP

#include "tokenizer.hpp"
#include "tag.hpp"
#include <stack>

namespace SoXN
	{
	template<class NodeTree>
	class TreeBuilder
		{
		public:
			explicit TreeBuilder(NodeTree& tree):r_tree(tree)
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

					case SoXN::TokenType::EndOfFile:
						if(m_tag_stack.size()!=0)
							{abort();}
						break;
					}
				}

		private:
			NodeTree& r_tree;
			Tag m_tag_prev;
			Tag m_tag_current;
			std::stack<Tag> m_tag_stack;
			Tag::attribute m_attrib;
		};
	}
#endif
