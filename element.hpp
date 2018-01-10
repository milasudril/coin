//@	{
//@	"targets":[{"name":"element.hpp","type":"include"}]
//@	,"dependencies_extra":{"ref":"element.o","rel":"implementation"}
//@	}

#ifndef SOXN_ELEMENT_HPP
#define SOXN_ELEMENT_HPP

#include <string>
#include <map>
#include <vector>

namespace SoXN
	{
	class Node;

	class Element
		{
		public:

		private:
			std::string m_name;
			std::map<std::string,std::string> m_attribs;
			std::vector<Node> m_children;
		};
	}

#endif
