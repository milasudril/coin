//@	{"targets":[{"name":"coin2xml","type":"application"}]}

#include <cstdio>
#include "saxdriver.hpp"
#include "tokenizer.hpp"
#include "xmlwriter.hpp"
#include "lexer_coin.hpp"

int main()
	{
	CoIN::XMLWriter<FILE*> writer(stdout);
	CoIN::tokenize(stdin, CoIN::LexerCoIN{}, CoIN::SAXDriver<CoIN::XMLWriter<FILE*>>{writer});
	return 0;
	}
