//@	{"targets":[{"name":"soxn2xml","type":"application"}]}

#include <cstdio>
#include "tokenizer.hpp"
#include <string>
#include <stack>

namespace
	{
	struct XMLOutput
		{
		std::string m_tagname;
		std::string m_tagprev;
		std::stack<std::string> m_tagstack;
		SoXN::TokenType m_state;
		};

	void flush(XMLOutput& ouptut){}

	void write(char ch_in,XMLOutput& output)
		{
		switch(output.m_state)
			{
			case SoXN::TokenType::TagName:
			//	TODO: Check that ch_in is a legal XML tag name.
				output.m_tagname+=ch_in;
				break;
			default:
				break;
			}

		switch(ch_in)
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
				putchar(ch_in);
			}
		}

	void elementClose(XMLOutput& output)
		{
		printf("</%s>",output.m_tagname.c_str());
		output.m_tagname=output.m_tagstack.top();
		output.m_tagstack.pop();
		}

	void setState(XMLOutput& output,SoXN::TokenType type)
		{
		output.m_state=type;
		switch(type)
			{
			case SoXN::TokenType::TagName:
				output.m_tagstack.push(output.m_tagname);
				output.m_tagprev=output.m_tagname;
				output.m_tagname.clear();
				putchar('<');
				break;

			case SoXN::TokenType::AttributeName:
				putchar(' ');
				break;

			case SoXN::TokenType::AttributeValue:
				putchar('=');
				break;

			case SoXN::TokenType::BodyText:
				putchar('>');
				break;

			default:
				break;
			}
		}
	}

int main()
	{
	SoXN::tokenize(stdin,XMLOutput{});
	return 0;
	}
