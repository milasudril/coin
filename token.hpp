//@	{
//@	"targets":[{"name":"token.hpp","type":"include"}]
//@	}

#ifndef COIN_TOKEN_HPP
#define COIN_TOKEN_HPP

#include <string>

namespace CoIN
	{
	enum class TokenType:int
		{
		 BodyText
		,BodyTextLast
		,TagName
		,TagNameNoAttributes
		,AttributeNameFirst
		,AttributeName
		,AttributeValue
		,AttributeValueLast
		,EndOfFile
		};

	struct Token
		{
		std::string value;
		TokenType type;
		int row;
		int col;
		};

	auto row(const Token& tok) noexcept
		{return tok.row;}

	auto col(const Token& tok) noexcept
		{return tok.col;}

	}

#endif
