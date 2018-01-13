//@	{
//@	"targets":[{"name":"dombuilder.hpp","type":"include"}]
//@	}

#ifndef SOXN_DOMBUILDER_HPP
#define SOXN_DOMBUILDER_HPP

#include "saxdriver.hpp"
#include "tokenizer.hpp"
#include "element.hpp"

namespace SoXN
	{
	class DOMBuilder
		{
		public:
			DOMBuilder():m_elem_current("root"){}

			void output(const std::string& str)
				{m_elem_current.append(str);}

			void outputBegin(const SoXN::Tag& tag)
				{
				m_elems_out.push(m_elem_current);
				m_elem_current=*m_elem_current.create(Element(tag));
				}

			void outputEnd(const SoXN::Tag& tag)
				{
				m_elem_current=m_elems_out.top();
				m_elems_out.pop();
				}

			const Element& currentElement()
				{return m_elem_current;}

		private:
			Element m_elem_current;
			std::stack<Element> m_elems_out;
		};

	template<class InputStream>
	inline Element load(InputStream stream)
		{
		DOMBuilder builder;
		tokenize(stream,SAXDriver<DOMBuilder>(builder));
		return builder.currentElement();
		}
	}

#endif
