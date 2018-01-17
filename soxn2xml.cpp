//@	{"targets":[{"name":"soxn2xml","type":"application"}]}

#include <cstdio>
#include "saxdriver.hpp"
#include "tokenizer.hpp"
#include "xmlwriter.hpp"

template<class Writer>
class DirectOutput
	{
	public:
		explicit DirectOutput(Writer& writer):r_writer(writer)
			{}

		void commentBegin()
			{printf("<!--");}

		void commentEnd(const std::string& str)
			{
			output(str);
			printf("-->");
			}

		void output(const std::string& str)
			{r_writer.writeBodyText(str);}

		void elementBegin(const SoXN::Tag& tag)
			{r_writer.writeBeginTag(tag);}

		void elementEnd(const SoXN::Tag& tag)
			{r_writer.writeEndTag(tag);}

	private:
		Writer& r_writer;
	};

int main()
	{
	SoXN::XMLWriter<FILE*> writer(stdout);
	DirectOutput<decltype(writer)> direct_out(writer);
	SoXN::tokenize(stdin,SoXN::SAXDriver<DirectOutput<decltype(writer)>>{direct_out});
	return 0;
	}
