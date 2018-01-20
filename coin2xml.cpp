//@	{"targets":[{"name":"coin2xml","type":"application"}]}

#include "input.hpp"
#include "xmlwriter.hpp"
#include "lexercoin.hpp"

int main()
	{
	CoIN::read(stdin, CoIN::LexerCoIN{}, CoIN::XMLWriter<FILE*>(stdout));
	return 0;
	}
