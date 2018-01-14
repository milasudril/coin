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
			DOMBuilder():m_elem_current("root")
				{}

			void commentBegin(){}

			void commentEnd(const std::string& str){}

			void output(const std::string& str)
				{
				if(r_elem_current)
					{r_elem_current->append(str);}
				}

			void elementBegin(const SoXN::Tag& tag)
				{
				m_elems_out.push(r_elem_current);
				if(!r_elem_current)
					{r_elem_current=m_elem_current.create(Element(tag));}
				else
					{r_elem_current=r_elem_current->create(Element(tag));}
				}

			void elementEnd(const SoXN::Tag& tag)
				{
				auto temp=m_elems_out.top();
				m_elems_out.pop();
				if(temp)
					{r_elem_current=temp;}
				}

			const Element& currentElement()
				{return *r_elem_current;}

		private:
			Element::NodePtr<Element> r_elem_current;
			Element m_elem_current;
			std::stack<Element::NodePtr<Element>> m_elems_out;
		};

	template<class InputStream,class ErrorPolicy=LogAndAbort>
	inline Element load(InputStream stream,ErrorPolicy&& err=LogAndAbort{})
		{
		DOMBuilder builder;
		tokenize(stream,SAXDriver<DOMBuilder>(builder));
		return builder.currentElement();
		}
	}

#endif
