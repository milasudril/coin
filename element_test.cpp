//@	{
//@	 "targets":[{"name":"element_test","type":"application"}]
//@	}

#include "element.hpp"
#include <cassert>

static void show(const std::string& str)
	{printf("%s",str.c_str());}

static void show(const CoIN::Element& element)
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
		{
		CoIN::Element test_a("html");
		test_a.append(CoIN::Element("head"));

		CoIN::Element test_b("body");
		test_a.append(test_b);
		show(test_a);
		putchar('\n');
		}


	CoIN::Element test_b("html");

	auto head=test_b.create(CoIN::Element("head"));
	head->create(CoIN::Element("meta"))
		->attributeAdd({"charset","UTF-8"});

	head->create(CoIN::Element("title"))
		->append("Hello, World").append(" More stuff");

	show(test_b);

	return 0;
	}
