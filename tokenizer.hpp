//@	{
//@	"targets":[{"name":"tokenizer.hpp","type":"include"}]
//@	}

#ifndef SOXN_TOKENIZER_HPP
#define SOXN_TOKENIZER_HPP

#include "token.hpp"
#include "errorpolicy.hpp"
#include "saxdriver.hpp"
#include "lexer_soxn.hpp"
#include <cassert>

namespace SoXN
	{
	enum class ParseResult:int{NoError,MoreData,Error};

	template<class Stream,class Lexer,class OutputFunction,class ErrorHandler=LogAndAbort>
	ParseResult tokenize(Stream& stream,Lexer&& lexer,SAXDriver<OutputFunction>&& output
		,ErrorHandler&& err=LogAndAbort{})
		{
		auto status=ProcessStatus::NoError;
		typedef decltype(status) ProcessStatus;

		auto keepGoing=[](ProcessStatus status)
			{
			typedef decltype(status) ProcessStatus;
			switch(status)
				{
				case ProcessStatus::NoError: return true;
				case ProcessStatus::DocumentEnd: return false;
				case ProcessStatus::Error: return false;
				}
			assert(false);
			};

		Token tok;
		tok.row=1;
		tok.col=0;
		while(keepGoing(status))
			{
			auto ch_in=getc(stream);
			if(feof(stream))
				{
				status=output(Token{"",TokenType::EndOfFile}, err);
				assert(status!=ProcessStatus::NoError);
				return status==ProcessStatus::DocumentEnd? ParseResult::NoError : ParseResult::Error;
				}

			if(ch_in=='\n')
				{
				tok.col=0;
				++tok.row;
				}
 			++tok.col;
			status=lexer(ch_in,tok,output,err);
			}
		assert(status!=ProcessStatus::NoError);
		return status==ProcessStatus::DocumentEnd? ParseResult::MoreData : ParseResult::NoError;
		}
	}

#endif
