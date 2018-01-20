//@	{"targets":[{"name":"dombuilder_test","type":"application"}]}

#include "dombuilder.hpp"
#include "xmlwriter.hpp"
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
			{
			CoIN::XMLWriter<FILE*> writer(stdout);
			write(element,writer);
			}
		}
	return 0;
	}
