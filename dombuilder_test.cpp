//@	{"targets":[{"name":"dombuilder_test","type":"application","cxxoptions":{"cxxversion_min":201402}}]}

#include <cstdio>
#include "dombuilder.hpp"
#include "xmlwriter.hpp"

static void show(const std::string& str, SoXN::XMLWriter<FILE*>& writer)
	{writer.writeBodyText(str);}

static void show(const SoXN::Element& element, SoXN::XMLWriter<FILE*>& writer)
	{
	writer.writeBeginTag(element.tag());
	element.visitChildren([&writer](const auto& node)
		{show(node,writer);});
	writer.writeEndTag(element.tag());

/*	printf("<%s",element.name().c_str());
	element.visitAttributes([](const auto& attrib)
		{printf(" %s=\"%s\"",attrib.first.c_str(),attrib.second.c_str());});
	printf(">");

	printf("</%s>",element.name().c_str());*/
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
			{
			SoXN::XMLWriter<FILE*> writer(stdout);
			show(element,writer);
			}
		}
	return 0;
	}
