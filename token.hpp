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
	}

#endif
