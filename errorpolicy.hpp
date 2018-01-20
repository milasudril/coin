//@	{
//@	"targets":[{"name":"errorpolicy.hpp","type":"include"}]
//@	}

#ifndef COIN_ERRORPOLICY_HPP
#define COIN_ERRORPOLICY_HPP

#include <cstdio>
#include <cstdlib>

namespace CoIN
	{
	struct ErrorPolicyDefault;

	struct LogAndAbort
		{
		template<class Location>
		void operator()(const Location& loc, const char* message)
			{
			fprintf(stderr,"%d:%d: %s\n", row(loc), col(loc), message);
			abort();
			}
		};

	typedef std::conditional<std::is_destructible<ErrorPolicyDefault>::value
		,ErrorPolicyDefault, LogAndAbort>::type ErrorPolicyDefault_;
	}

#endif
