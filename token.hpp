//@	{
//@	"targets":[{"name":"token.hpp","type":"include"}]
//@	}

#ifndef SOXN_TOKEN_HPP
#define SOXN_TOKEN_HPP

#include <string>

namespace SoXN
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
