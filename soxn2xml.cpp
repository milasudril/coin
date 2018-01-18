//@	{"targets":[{"name":"soxn2xml","type":"application"}]}

#include <cstdio>
#include "saxdriver.hpp"
#include "tokenizer.hpp"
#include "xmlwriter.hpp"


int main()
	{
	SoXN::XMLWriter<FILE*> writer(stdout);
	SoXN::tokenize(stdin, SoXN::SAXDriver<SoXN::XMLWriter<FILE*>>{writer});
	return 0;
	}
