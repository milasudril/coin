//@	{
//@	"targets":[{"name":"xmlwriter.hpp","type":"include"}]
//@	}

#ifndef SOXN_XMLWRITER_HPP
#define SOXN_XMLWRITER_HPP

#include "tag.hpp"

namespace SoXN
	{
	namespace detail
		{
		template<class Stream>
		void writeXMLName(const std::string& str,Stream& stream)
			{
			auto ptr=str.data();
			auto ch_in=*ptr;
			while(ch_in)
				{
			//	TODO: validate ch_in before writing it to stream
				putc(ch_in,stream);
				++ptr;
				ch_in=*ptr;
				}
			}

		template<class Stream>
		void writeXMLAttribValue(const std::string& str,Stream& stream)
			{
			auto ptr=str.data();
			auto ch_in=*ptr;
			while(ch_in)
				{
				switch(ch_in)
					{
					case '&':
						fputs("&amp;",stream);
						break;
					case '"':
						fputs("&quote;",stream);
						break;
					default:
						putc(ch_in,stream);
					}
				++ptr;
				ch_in=*ptr;
				}
			}

		template<class Stream>
		void writeXMLBodyText(const std::string& str,Stream& stream)
			{
			auto ptr=str.data();
			auto ch_in=*ptr;
			while(ch_in)
				{
				switch(ch_in)
					{
					case '<':
						fputs("&lt;",stream);
						break;
					case '>':
						fputs("&gt;",stream);
						break;
					case '&':
						fputs("&amp;",stream);
						break;
					default:
						putc(ch_in,stream);
					}
				++ptr;
				ch_in=*ptr;
				}
			}
		}

	template<class Stream>
	class XMLWriter
		{
		public:
			explicit XMLWriter(Stream& stream):r_stream(stream)
				{fputs("<?xml version=\"1.0\"?>",r_stream);}

			void commentBegin()
				{fputs("<!--",r_stream);}

			void commentEnd(const std::string& str)
				{
				output(str);
				fputs("-->",r_stream);
				}

			void elementBegin(const Tag& tag)
				{
				putc('<',r_stream);
				detail::writeXMLName(tag.name(),r_stream);
				tag.visitAttributes([this](const auto& attribute)
					{
					putc(' ',r_stream);
					detail::writeXMLName(attribute.first,r_stream);
					fputs("=\"",r_stream);
					detail::writeXMLAttribValue(attribute.second,r_stream);
					putc('"',r_stream);
					});
				putc('>',r_stream);
				}

			void elementEnd(const Tag& tag)
				{
				fputs("</",r_stream);
				detail::writeXMLName(tag.name(),r_stream);
				putc('>',r_stream);
				}

			void output(const std::string& string)
				{detail::writeXMLBodyText(string.c_str(),r_stream);}

		private:
			Stream& r_stream;
		};
	}

#endif
