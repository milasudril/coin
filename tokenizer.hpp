#ifndef SoXN_TOKENIZER_HPP
#define SoXN_TOKENIZER_HPP

namespace SoXN
	{
	enum class TokenType:int{BodyText,TagName,AttributeList,AttributeName,AttributeValue};

	template<class Stream,class OutputPolicy>
	void tokenize(Stream& stream,OutputPolicy&& output)
		{
		enum class State:int{BodyText,Escape,TagName,AttributeList,AttributeName,AttributeValue};

		auto state_current=State::BodyText;
		auto state_old=state_current;

		while(true)
			{
			auto ch_in=getc(stream);
			if(feof(stream))
				{
				flush(output);
				return;
				}

			switch(state_current)
				{
				case State::BodyText:
					switch(ch_in)
						{
						case '{':
							setState(output,TokenType::TagName);
							state_current=State::TagName;
							break;
						case '}':
							elementClose(output);
							break;
						case '\\':
							state_old=state_current;
							state_current=State::Escape;
							break;
						default:
							write(ch_in,output);
						}
					break;

				case State::Escape:
					write(ch_in,output);
					state_current=state_old;
					break;

				case State::TagName:
					switch(ch_in)
						{
						case ':':
							setState(output,TokenType::BodyText);
							state_current=State::BodyText;
							break;
						case '\\':
							state_old=state_current;
							state_current=State::Escape;
							break;
						case '@':
							setState(output,TokenType::AttributeList);
							state_current=State::AttributeList;
							break;
						default:
							write(ch_in,output);
						}
					break;

				case State::AttributeList:
					switch(ch_in)
						{
						case ':':
							setState(output,TokenType::BodyText);
							state_current=State::BodyText;
							break;
						case '\\':
							state_old=state_current;
							state_current=State::Escape;
							break;
						default:
							setState(output,TokenType::AttributeName);
							write(ch_in,output);
							state_current=State::AttributeName;
						}
					break;

				case State::AttributeName:
					switch(ch_in)
						{
						case '=':
							setState(output,TokenType::AttributeValue);
							state_current=State::AttributeValue;
							break;
						case '\\':
							state_old=state_current;
							state_current=State::Escape;
							break;
						default:
							write(ch_in,output);
						}
					break;

				case State::AttributeValue:
					switch(ch_in)
						{
						case ':':
							setState(output,TokenType::BodyText);
							state_current=State::BodyText;
							break;
						case ';':
							state_current=State::AttributeName;
							setState(output,TokenType::AttributeName);
							break;
						case '\\':
							state_old=state_current;
							state_current=State::Escape;
							break;
						default:
							write(ch_in,output);
						}
					break;
				}
			}
		}
	}

#endif
