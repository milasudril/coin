#ifndef SoXN_TOKENIZER_HPP
#define SoXN_TOKENIZER_HPP

namespace SoXN
	{
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
				output.flush();
				return;
				}

			switch(state_current)
				{
				case State::BodyText:
					switch(ch_in)
						{
						case '{':
							state_current=State::TagName;
							break;
						case '}':
							break;
						case '\\':
							state_old=state_current;
							state_current=State::Escape;
							break;
						default:
							output.write(ch_in);
						}
					break;

				case State::Escape:
					output.write(ch_in);
					state_current=state_old;
					break;

				case State::TagName:
					switch(ch_in)
						{
						case ':':
							state_current=State::BodyText;
							break;
						case '\\':
							state_old=state_current;
							state_current=State::Escape;
							break;
						case '@':
							state_current=State::AttributeList;
							break;
						default:
							output.write(ch_in);
						}
					break;

				case State::AttributeList:
					switch(ch_in)
						{
						case ':':
							state_current=State::BodyText;
							break;
						case '\\':
							state_old=state_current;
							state_current=State::Escape;
							break;
						default:
							output.write(ch_in);
							state_current=State::AttributeName;
						}
					break;

				case State::AttributeName:
					switch(ch_in)
						{
						case '=':
							state_current=State::AttributeValue;
							break;
						case '\\':
							state_old=state_current;
							state_current=State::Escape;
							break;
						default:
							output.write(ch_in);
						}
					break;

				case State::AttributeValue:
					switch(ch_in)
						{
						case ':':
							state_current=State::BodyText;
							break;
						case ';':
							state_current=State::AttributeName;
							break;
						case '\\':
							state_old=state_current;
							state_current=State::Escape;
							break;
						default:
							output.write(ch_in);
						}
					break;
				}
			}
		}
	}

#endif
