//@	{"targets":[{"name":"coin2xml","type":"application"}]}

#include <cstdio>
#include "tokenizer.hpp"
#include "xmlwriter.hpp"
#include "lexer_coin.hpp"

int main()
	{
	CoIN::tokenize(stdin, CoIN::LexerCoIN{}, CoIN::XMLWriter<FILE*>(stdout));
	return 0;
	}
