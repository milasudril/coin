//@	{
//@	 "targets":[{"name":"element_test","type":"application"}]
//@	}

#include "element.hpp"
#include <cassert>

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
		{
		SoXN::Element test_a("html");
		test_a.append(SoXN::Element("head"));

		SoXN::Element test_b("body");
		test_a.append(test_b);
		show(test_a);
		putchar('\n');
		}


	SoXN::Element test_b("html");

	auto head=test_b.create(SoXN::Element("head"));
	head->create(SoXN::Element("meta"))
		->attributeAdd({"charset","UTF-8"});

	head->create(SoXN::Element("title"))
		->append("Hello, World").append(" More stuff");

	show(test_b);

	return 0;
	}
