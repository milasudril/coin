//@	{"targets":[{"name":"dombuilder_test","type":"application","cxxoptions":{"cxxversion_min":201402}}]}

#include <cstdio>
#include "dombuilder.hpp"

static void show(const std::string& str)
	{printf("%s",str.c_str());}

static void show(const SoXN::Element& element)
	{
	printf("<%s",element.name().c_str());
	element.visitAttributes([](const auto& attrib)
		{printf(" %s=\"%s\"",attrib.first.c_str(),attrib.second.c_str());});
	printf(">");
	element.visitChildren([](const auto& node)
		{show(node);});
	printf("</%s>",element.name().c_str());
	}

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
			{show(element);}
		}
	return 0;
	}
