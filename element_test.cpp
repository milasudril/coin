//@	{
//@	 "targets":[{"name":"element_test","type":"application"}]
//@	}

#include "element.hpp"
#include <cassert>

static void show(const SoXN::Element::node_type& node)
	{
	switch(node.type())
		{
		case SoXN::NodeType::String:
			printf("%s\n", node.getAs<SoXN::NodeType::String>().c_str());
			break;
		}
	}

int main()
	{
	SoXN::Element element("html");
	return 0;
	}
