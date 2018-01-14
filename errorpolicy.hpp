//@	{
//@	"targets":[{"name":"errorpolicy.hpp","type":"include"}]
//@	}

#ifndef SOXN_ERRORPOLICY_HPP
#define SOXN_ERRORPOLICY_HPP

namespace SoXN
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
