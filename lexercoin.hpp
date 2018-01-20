//@{"targets":[{"name":"lexercoin.hpp","type":"include"}]}

#ifndef COIN_LEXERCOIN_HPP
#define COIN_LEXERCOIN_HPP

#include "processstatus.hpp"
#include <cassert>


namespace CoIN
	{
	class LexerCoIN
		{
		public:
			LexerCoIN():state_current(State::Init),state_old(State::Init)
				{}

			template<class Parser,class ErrorPolicy>
			ProcessStatus operator()(char ch_in, Token& tok, Parser&& output, ErrorPolicy&& err);

			private:
				enum class State:int{Init,BodyText,Escape,TagName,AttributeList,AttributeName,AttributeValue};
				State state_current;
				State state_old;
		};

	template<class Parser,class ErrorPolicy>
	ProcessStatus LexerCoIN::operator()(char ch_in, Token& tok, Parser&& output, ErrorPolicy&& err)
		{
		switch(state_current)
			{
			case State::Init:
				switch(ch_in)
					{
					case '{':
						tok.value.clear();
						tok.type=TokenType::TagName;
						state_current=State::TagName;
						return ProcessStatus::NoError;
					default:
						if(!(ch_in>=0 && ch_in<=' '))
							{
							err(tok,"Expected { at begin of file.");
							return ProcessStatus::Error;
							}
						return ProcessStatus::NoError;
					}

			case State::BodyText:
				switch(ch_in)
					{
					case '{':
						{
						tok.type=TokenType::BodyText;
						auto status=output(tok,err);
						tok.value.clear();
						tok.type=TokenType::TagName;
						state_current=State::TagName;
						return status;
						}
					case '}':
						{
						tok.type=TokenType::BodyTextLast;
						auto status=output(tok,err);
						tok.value.clear();
						return status;
						}
					case '\\':
						state_old=state_current;
						state_current=State::Escape;
						return ProcessStatus::NoError;
					default:
						tok.value+=ch_in;
						return ProcessStatus::NoError;
					}

			case State::Escape:
				tok.value+=ch_in;
				state_current=state_old;
				return ProcessStatus::NoError;

			case State::TagName:
				switch(ch_in)
					{
					case '}':
					case '{':
						err(tok, "Element begin/end markers must be escaped in element names.");
						return ProcessStatus::Error;
					case ':':
						{
						tok.type=TokenType::TagNameNoAttributes;
						auto status=output(tok,err);
						tok.value.clear();
						tok.type=TokenType::BodyText;
						state_current=State::BodyText;
						return status;
						}
					case '\\':
						state_old=state_current;
						state_current=State::Escape;
						return ProcessStatus::NoError;
					case '@':
						{
						tok.type=TokenType::TagName;
						auto status=output(tok,err);
						tok.value.clear();
						tok.type=TokenType::AttributeNameFirst;
						state_current=State::AttributeList;
						return status;
						}
					default:
						tok.value+=ch_in;
						return ProcessStatus::NoError;
					}

			case State::AttributeList:
				switch(ch_in)
					{
					case '}':
					case '{':
						err(tok, "Element begin/end markers must be escaped in attribute names.");
						return ProcessStatus::Error;
					case ':':
						err(tok, "Attribute lists cannot be empty.");
						return ProcessStatus::Error;
					case '\\':
						state_old=state_current;
						state_current=State::Escape;
						return ProcessStatus::NoError;
					default:
						tok.value+=ch_in;
						state_current=State::AttributeName;
						return ProcessStatus::NoError;
					}

			case State::AttributeName:
				switch(ch_in)
					{
					case ':':
						err(tok, "Empty attributes are not allowed.");
						return ProcessStatus::Error;
					case '}':
					case '{':
						err(tok, "Element begin/end markers must be escaped in attribute names.");
						return ProcessStatus::Error;
					case '=':
						{
						auto status=output(tok,err);
						tok.value.clear();
						tok.type=TokenType::AttributeValue;
						state_current=State::AttributeValue;
						return status;
						}
					case '\\':
						state_old=state_current;
						state_current=State::Escape;
						return ProcessStatus::NoError;
					default:
						tok.value+=ch_in;
						return ProcessStatus::NoError;
					}

			case State::AttributeValue:
				switch(ch_in)
					{
					case '}':
					case '{':
						err(tok, "Element begin/end marker must be escaped in attribute values.");
						return ProcessStatus::Error;
					case ':':
						{
						tok.type=TokenType::AttributeValueLast;
						auto status=output(tok,err);
						tok.value.clear();
						tok.type=TokenType::BodyText;
						state_current=State::BodyText;
						return status;
						}
					case ';':
						{
						auto status=output(tok,err);
						tok.value.clear();
						tok.type=TokenType::AttributeName;
						state_current=State::AttributeName;
						return status;
						}
					case '\\':
						state_old=state_current;
						state_current=State::Escape;
						return ProcessStatus::NoError;
					default:
						tok.value+=ch_in;
						return ProcessStatus::NoError;
					}
			}
		assert(0);
		}
	}
#endif
