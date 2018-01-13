#ifndef SoXN_TOKENIZER_HPP
#define SoXN_TOKENIZER_HPP

#include <string>

namespace SoXN
	{
	enum class TokenType:int
		{
		 BodyText
		,BodyTextLast
		,TagName
		,TagNameNoAttributes
		,AttributeNameFirst
		,AttributeName
		,AttributeValue
		,AttributeValueLast
		,EndOfFile
		,Error
		};

	struct Token
		{
		std::string value;
		TokenType type;
		int row;
		int col;
		};

	template<class Stream,class OutputPolicy>
	void tokenize(Stream& stream,OutputPolicy&& output)
		{
		enum class State:int{BodyText,Escape,TagName,AttributeList,AttributeName,AttributeValue};

		auto state_current=State::BodyText;
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
					//TODO: Currently, literal '}' is allowed in tag names. It should be interpreted as the sequence  ":}"
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
					//TODO: Currently, literal '}' is allowed in tag names. It is probably better to claim it is an error.
						case ':': //TODO: Should empty attribute list be allowed?
							state_current=State::BodyText;
							break;
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
					//TODO: Currently, literal '}' is allowed in attribute names. It is probably better to claim it is an error.
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
					//TODO: Currently, literal '}' is allowed in attribute values. It is probably better to claim it is an error.
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
