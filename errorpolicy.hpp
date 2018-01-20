//@	{
//@	"targets":[{"name":"errorpolicy.hpp","type":"include"}]
//@	}

#ifndef COIN_ERRORPOLICY_HPP
#define COIN_ERRORPOLICY_HPP

namespace CoIN
	{
	struct LogAndAbort
		{
		void operator()(const Token& tok,const char* message)
			{
			fprintf(stderr,"%d:%d: %s\n", tok.row, tok.col, message);
			abort();
			}
		};
	}

#endif
