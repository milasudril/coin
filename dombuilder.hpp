//@	{
//@	"targets":[{"name":"dombuilder.hpp","type":"include"}]
//@	}

#ifndef SOXN_DOMBUILDER_HPP
#define SOXN_DOMBUILDER_HPP

namespace SoXN
	{
	class DOMBuilder
		{
		public:
			void output(const std::string& str)
				{m_elem_current.append(str);}

			void outputBegin(const SoXN::Tag& tag)
				{
				m_elems_out.push(m_elems_current);
				m_elem_current=*m_elem_current.create(tag);
				}

			void outputEnd(const SoXN::Tag& tag)
				{
				m_elem_current=m_nodes_out.top();
				m_elems_out.pop();
				}

			const Element& currentElement()
				{return m_elem_current;}

		private:
			Element m_elem_current;
			std::stack<Element> m_elems_out;
		};
	}

#endif
