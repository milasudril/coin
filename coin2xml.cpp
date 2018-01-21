//@	{"targets":[{"name":"coin2xml","type":"application"}]}

#include "input.hpp"
#include "writerxml.hpp"
#include "lexercoin.hpp"

int main()
	{
	read(stdin, CoIN::LexerCoIN{}, CoIN::WriterXML<FILE*>(stdout));
	return 0;
	}
