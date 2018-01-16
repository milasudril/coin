//@	{
//@	"targets":[{"name":"tokenizer.hpp","type":"include"}]
//@	}

#ifndef SOXN_TOKENIZER_HPP
#define SOXN_TOKENIZER_HPP

#include "token.hpp"
#include "errorpolicy.hpp"
#include "saxdriver.hpp"
#include <cassert>

namespace SoXN
	{
	enum ParseResult:int{NoError,MoreData,Error};

	template<class Stream,class OutputFunction>
	ParseResult tokenize(Stream& stream,OutputFunction&& output)
		{return tokenize(stream,std::forward<OutputFunction>(output),LogAndAbort{});}

	template<class Stream,class OutputFunction,class ErrorHandler>
	ParseResult tokenize(Stream& stream,SAXDriver<OutputFunction>&& output,ErrorHandler&& err)
		{
		enum class State:int{Init,BodyText,Escape,TagName,AttributeList,AttributeName,AttributeValue};
		auto status=SAXDriver<OutputFunction>::ProcessStatus::NoError;
		typedef decltype(status) ProcessStatus;
		auto state_current=State::Init;
		auto state_old=state_current;

		auto keepGoing=[](ProcessStatus status)
			{
			typedef decltype(status) ProcessStatus;
			switch(status)
				{
				case ProcessStatus::NoError: return true;
				case ProcessStatus::DocumentEnd: return false;
				case ProcessStatus::Error: return false;
				}
			assert(false);
			};

		Token tok;
		tok.row=1;
		tok.col=0;
		while(keepGoing(status))
			{
			auto ch_in=getc(stream);
			if(feof(stream))
				{
				status=output(Token{"",TokenType::EndOfFile}, err);
				assert(status!=ProcessStatus::NoError);
				return status==ProcessStatus::DocumentEnd? ParseResult::NoError : ParseResult::Error;
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
							if(!(ch_in>=0 && ch_in<=' '))
								{
								err(tok,"Expected { at begin of file.");
								return ParseResult::Error;
								}
						}
					break;

				case State::BodyText:
					switch(ch_in)
						{
						case '{':
							tok.type=TokenType::BodyText;
							status=output(tok,err);
							tok.value.clear();
							tok.type=TokenType::TagName;
							state_current=State::TagName;
							break;
						case '}':
							tok.type=TokenType::BodyTextLast;
							status=output(tok,err);
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
							return ParseResult::Error;
						case ':':
							tok.type=TokenType::TagNameNoAttributes;
							status=output(tok,err);
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
							status=output(tok,err);
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
							return ParseResult::Error;
						case ':':
							err(tok, "Attribute lists cannot be empty.");
							return ParseResult::Error;
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
						case ':':
							err(tok, "Empty attributes are not allowed.");
							return ParseResult::Error;
						case '}':
						case '{':
							err(tok, "Element begin/end markers must be escaped in attribute names.");
							return ParseResult::Error;
						case '=':
							status=output(tok,err);
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
							return ParseResult::Error;
						case ':':
							tok.type=TokenType::AttributeValueLast;
							status=output(tok,err);
							tok.value.clear();
							tok.type=TokenType::BodyText;
							state_current=State::BodyText;
							break;
						case ';':
							status=output(tok,err);
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
		assert(status!=ProcessStatus::NoError);
		return status==ProcessStatus::DocumentEnd? ParseResult::MoreData : ParseResult::NoError;
		}
	}

#endif
