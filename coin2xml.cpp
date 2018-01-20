//@	{"targets":[{"name":"coin2xml","type":"application"}]}

#include <cstdio>
#include "input.hpp"
#include "xmlwriter.hpp"
#include "lexer_coin.hpp"

int main()
	{
	CoIN::read(stdin, CoIN::LexerCoIN{}, CoIN::XMLWriter<FILE*>(stdout));
	return 0;
	}
