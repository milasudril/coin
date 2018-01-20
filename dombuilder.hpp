//@	{
//@	"targets":[{"name":"dombuilder.hpp","type":"include"}]
//@	}

#ifndef COIN_DOMBUILDER_HPP
#define COIN_DOMBUILDER_HPP

#include "tokenizer.hpp"
#include "element.hpp"

namespace CoIN
	{
	class DOMBuilder
		{
		public:
			DOMBuilder(Element& element_out):m_elem_current("root"),r_element_out(element_out)
				{}

			void commentBegin(){}

			void commentEnd(const std::string& str){}

			void output(const std::string& str)
				{
				if(r_elem_current)
					{r_elem_current->append(str);}
				}

			void elementBegin(const CoIN::Tag& tag)
				{
				m_elems_out.push(r_elem_current);
				if(r_elem_current)
					{r_elem_current=r_elem_current->create(Element(tag));}
				else
					{r_elem_current=m_elem_current.create(Element(tag));}
				}

			void elementEnd(const CoIN::Tag& tag)
				{
				auto temp=m_elems_out.top();
				m_elems_out.pop();
				if(temp)
					{r_elem_current=temp;}
				else
					{
					// This is the last thing that happens, so steal whatever r_elem_current reffers to.
					r_element_out=std::move(*r_elem_current);
					}
				}

		private:
			Element::NodePtr<Element> r_elem_current;
			Element m_elem_current;
			std::stack<Element::NodePtr<Element>> m_elems_out;
			Element& r_element_out;
		};

	template<class InputStream,class Lexer,class ErrorPolicy=LogAndAbort>
	inline ParseResult load(InputStream&& stream,Lexer&& lexer,Element& element,ErrorPolicy&& err=LogAndAbort{})
		{
		return tokenize(std::forward<InputStream>(stream),std::forward<Lexer>(lexer),DOMBuilder(element));
		}
	}

#endif
