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
	auto element=SoXN::load(stdin);
	show(element);

	}
