//@	{"targets":[{"name":"dombuilder_test","type":"application"}]}

#include "dombuilder.hpp"
#include "writerxml.hpp"
#include "lexercoin.hpp"

int main()
	{
	auto status=CoIN::ParseResult::MoreData;
	while(status==CoIN::ParseResult::MoreData)
		{
		CoIN::Element element;
		status=CoIN::load(stdin,CoIN::LexerCoIN{},element);
		if(status==CoIN::ParseResult::Error)
			{return -1;}
		if(element)
			{write(element,CoIN::WriterXML<FILE*>(stdout));}
		}
	return 0;
	}
