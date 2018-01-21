//@	{
//@	"targets":[{"name":"writerplaintext.hpp","type":"include"}]
//@	}

#ifndef COIN_WRITERPLAINTEXT_HPP
#define COIN_WRITERPLAINTEXT_HPP

#include "tag.hpp"

namespace CoIN
	{
	template<class Stream>
	class WriterPlaintext
		{
		public:
			explicit WriterPlaintext(Stream& stream):r_stream(stream)
				{}

			void commentBegin()
				{}

			void commentEnd(const std::string& str)
				{}

			void elementBegin(const Tag& tag)
				{}

			void elementEnd(const Tag& tag)
				{}

			void output(const std::string& string)
				{fputs(string.c_str(),r_stream);}

		private:
			Stream& r_stream;
		};
	}

#endif
