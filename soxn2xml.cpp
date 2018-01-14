//@	{"targets":[{"name":"soxn2xml","type":"application","cxxoptions":{"cxxversion_min":201402}}]}

#include <cstdio>
#include "saxdriver.hpp"
#include "tokenizer.hpp"
#include <string>
#include <stack>
#include <map>
#include <algorithm>

class XMLWriter
	{
	public:
		void commentBegin()
			{printf("<!--");}

		void commentEnd(const std::string& str)
			{
			output(str);
			printf("-->");
			}

		void output(const std::string& str)
			{
			std::for_each(str.begin(),str.end(),[](char ch_out)
				{
				switch(ch_out)
					{
					case '<':
						printf("&lt;");
						break;
					case '>':
						printf("&gt;");
						break;
					case '&':
						printf("&amp;");
						break;
					default:
						putchar(ch_out);
					}
				});
			}

		bool nameValid(const std::string& str)
			{
			return std::find_if(str.begin(),str.end(),[](char ch_check)
				{
			//	For now, only check stuff that would lead to syntactical issues
				return ch_check=='<' || ch_check=='>' || ch_check=='"' || ch_check=='=' || ch_check=='/'
					|| (ch_check>='\0' && ch_check<=' ');
				})==str.end();
			}

		void outputBegin(const SoXN::Tag& tag)
			{
			if(tag.name()=="!")
				{
				printf("<!--"); //This is a comment. Since we just converts the document to XML, keep it.
				return;
				}

			if(!nameValid(tag.name()))
				{abort();}
			printf("<%s",tag.name().c_str());
			tag.visitAttributes([](const auto& attrib)
				{printf(" %s=\"%s\"",attrib.first.c_str(),attrib.second.c_str());});
			printf(">");
			}

		void outputEnd(const SoXN::Tag& tag)
			{
			if(tag.name()=="")
				{abort();}

			if(tag.name()=="!")
				{
				printf("-->");
				return;
				}

			printf("</%s>",tag.name().c_str());
			}
	};

int main()
	{
	SoXN::tokenize(stdin,SoXN::SAXDriver<XMLWriter>{XMLWriter()});
	return 0;
	}
