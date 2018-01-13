//@	{
//@	 "targets":[{"name":"element_test","type":"application"}]
//@	}

#include "element.hpp"
#include <cassert>

static void show(const SoXN::Element::NodeModel& node);

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

static void show(const SoXN::Element::NodeModel& node)
	{
	switch(node.type())
		{
		case SoXN::NodeType::String:
			printf("%s", node.getAs<std::string>().c_str());
			break;

		case SoXN::NodeType::Element:
			show(node.getAs<SoXN::Element>());
			break;
		}
	}

int main()
	{
	SoXN::Element html("html");
	SoXN::Element head("head");
	html.append(SoXN::Element::NodeModel(head));
	show(html);
	return 0;
	}