//@	{
//@	"targets":[{"name":"xmlwriter.hpp","type":"include"}]
//@	}

#ifndef SOXN_XMLWRITER_HPP
#define SOXN_XMLWRITER_HPP

#include "tag.hpp"

namespace SoXN
	{
	template<class Stream>
	class XMLWriter
		{
		public:
			explicit XMLWriter(Stream& stream):r_stream(stream)
				{fputs("<?xml version=\"1.0\"?>",r_stream);}

			void writeBeginTag(const Tag& tag)
				{
				putc('<',r_stream);
				fputs(tag.name().c_str(),r_stream);
				tag.visitAttributes([this](const auto& attribute)
					{
					putc(' ',r_stream);
					fputs(attribute.first.c_str(),r_stream);
					fputs("=\"",r_stream);
					fputs(attribute.second.c_str(),r_stream);
					putc('"',r_stream);
					});
				putc('>',r_stream);
				}

			void writeEndTag(const Tag& tag)
				{
				fputs("</",r_stream);
				fputs(tag.name().c_str(),r_stream);
				putc('>',r_stream);
				}

			void writeBodyText(const std::string& string)
				{fputs(string.c_str(),r_stream);}

		private:
			Stream& r_stream;
		};
	}

#endif
