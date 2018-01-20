//@	{
//@	"targets":[{"name":"tokenizer.hpp","type":"include"}]
//@	}

#ifndef COIN_TOKENIZER_HPP
#define COIN_TOKENIZER_HPP

#include "token.hpp"
#include "errorpolicy.hpp"
#include "parser.hpp"
#include <cassert>

namespace CoIN
	{
	enum class ParseResult:int{NoError,MoreData,Error};

	template<class Stream,class Lexer,class OutputFunction,class ErrorHandler=LogAndAbort>
	ParseResult tokenize(Stream& stream,Lexer&& lexer,OutputFunction&& parser_output
		,ErrorHandler&& err=LogAndAbort{})
		{
		auto status=ProcessStatus::NoError;
		typedef decltype(status) ProcessStatus;
		Parser<OutputFunction> output(std::forward<OutputFunction>(parser_output));

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
