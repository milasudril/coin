//@	{
//@	"targets":[{"name":"tokenizer.hpp","type":"include"}]
//@	}

#ifndef SOXN_TOKENIZER_HPP
#define SOXN_TOKENIZER_HPP

#include "token.hpp"
#include "errorpolicy.hpp"

namespace SoXN
	{

	template<class Stream,class OutputFunction>
	void tokenize(Stream& stream,OutputFunction&& output)
		{tokenize(stream,std::forward<OutputFunction>(output),LogAndAbort{});}

	template<class Stream,class OutputFunction,class ErrorHandler>
	void tokenize(Stream& stream,OutputFunction&& output,ErrorHandler&& err)
		{
		enum class State:int{Init,BodyText,Escape,TagName,AttributeList,AttributeName,AttributeValue};

		auto state_current=State::Init;
		auto state_old=state_current;
		Token tok;
		tok.row=1;
		tok.col=1;
		while(true)
			{
			auto ch_in=getc(stream);
			if(feof(stream))
				{
				output(Token{"",TokenType::EndOfFile});
				return;
				}

			if(ch_in=='\n')
				{
				tok.col=0;
				++tok.row;
				}
 			++tok.col;

			switch(state_current)
				{
				case State::Init:
					switch(ch_in)
						{
						case '{':
							tok.value.clear();
							tok.type=TokenType::TagName;
							state_current=State::TagName;
							break;
						default:
							err(tok,"Expected { at begin of file");
							return;
						}
				case State::BodyText:
					switch(ch_in)
						{
						case '{':
							tok.type=TokenType::BodyText;
							output(tok);
							tok.value.clear();
							tok.type=TokenType::TagName;
							state_current=State::TagName;
							break;
						case '}':
							tok.type=TokenType::BodyTextLast;
							output(tok);
							tok.value.clear();
							break;
						case '\\':
							state_old=state_current;
							state_current=State::Escape;
							break;
						default:
							tok.value+=ch_in;
						}
					break;

				case State::Escape:
					tok.value+=ch_in;
					state_current=state_old;
					break;

				case State::TagName:
					switch(ch_in)
						{
						case '}':
						case '{':
							err(tok, "Element begin/end markers must be escaped in element names.");
							return;
						case ':':
							tok.type=TokenType::TagNameNoAttributes;
							output(tok);
							tok.value.clear();
							tok.type=TokenType::BodyText;
							state_current=State::BodyText;
							break;
						case '\\':
							state_old=state_current;
							state_current=State::Escape;
							break;
						case '@':
							tok.type=TokenType::TagName;
							output(tok);
							tok.value.clear();
							tok.type=TokenType::AttributeNameFirst;
							state_current=State::AttributeList;
							break;
						default:
							tok.value+=ch_in;
						}
					break;

				case State::AttributeList:
					switch(ch_in)
						{
						case '}':
						case '{':
							err(tok, "Element begin/end markers must be escaped in attribute names.");
							return;
						case ':':
							err(tok, "Attribute lists cannot be empty.");
							return;
						case '\\':
							state_old=state_current;
							state_current=State::Escape;
							break;
						default:
							tok.value+=ch_in;
							state_current=State::AttributeName;
						}
					break;

				case State::AttributeName:
					switch(ch_in)
						{
						case '}':
						case '{':
							err(tok, "Element begin/end markers must be escaped in attribute names.");
							return;
						case '=':
							output(tok);
							tok.value.clear();
							tok.type=TokenType::AttributeValue;
							state_current=State::AttributeValue;
							break;
						case '\\':
							state_old=state_current;
							state_current=State::Escape;
							break;
						default:
							tok.value+=ch_in;
						}
					break;

				case State::AttributeValue:
					switch(ch_in)
						{
						case '}':
						case '{':
							err(tok, "Element begin/end marker must be escaped in attribute values.");
							return;
						case ':':
							tok.type=TokenType::AttributeValueLast;
							output(tok);
							tok.value.clear();
							tok.type=TokenType::BodyText;
							state_current=State::BodyText;
							break;
						case ';':
							output(tok);
							tok.value.clear();
							tok.type=TokenType::AttributeName;
							state_current=State::AttributeName;
							break;
						case '\\':
							state_old=state_current;
							state_current=State::Escape;
							break;
						default:
							tok.value+=ch_in;
						}
					break;
				}
			}
		}
	}

#endif
