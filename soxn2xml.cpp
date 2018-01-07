//@	{"targets":[{"name":"soxn2xml","type":"application"}]}

#include <cstdio>
#include "tokenizer.hpp"

namespace
	{
	class XMLOutput
		{
		public:
			void flush(){}
			void write(char ch_in)
				{putchar(ch_in);}
		};
	}

int main()
	{
	SoXN::tokenize(stdin,XMLOutput{});
	return 0;
	}
