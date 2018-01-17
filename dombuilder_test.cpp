//@	{"targets":[{"name":"dombuilder_test","type":"application","cxxoptions":{"cxxversion_min":201402}}]}

#include <cstdio>
#include "dombuilder.hpp"
#include "xmlwriter.hpp"

int main()
	{
	auto status=SoXN::ParseResult::MoreData;
	while(status==SoXN::ParseResult::MoreData)
		{
		SoXN::Element element;
		status=SoXN::load(stdin,element);
		if(status==SoXN::ParseResult::Error)
			{return -1;}
		if(element)
			{
			SoXN::XMLWriter<FILE*> writer(stdout);
			write(element,writer);
			}
		}
	return 0;
	}
