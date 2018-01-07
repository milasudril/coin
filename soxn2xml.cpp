//@	{"targets":[{"name":"soxn2xml","type":"application"}]}

#include <cstdio>
#include "tokenizer.hpp"
#include <string>
#include <stack>

namespace
	{
	void test(const SoXN::Token& tok)
		{
		printf("%d, [%s]\n",tok.type,tok.value.c_str());
		}
	}

int main()
	{
	SoXN::tokenize(stdin,&test);
	return 0;
	}
