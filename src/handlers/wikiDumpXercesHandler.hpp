#pragma once

#include <stack>
#include <vector>
#include <functional>
#include <sstream>

#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/Attributes.hpp>

#include "abstractWikiPageHandler.hpp"
#include "wikiDumpHandlerProperties.hpp"

namespace WikiXmlDumpXerces {

	class WikiDumpXercesHandler : public xercesc::DefaultHandler {
		public:

			inline WikiDumpXercesHandler(AbstractWikiPageHandler& articleHandler, const WikiDumpHandlerProperties& properties)
			:_articleHandler(articleHandler),
			_handlerProperties(properties)	
			{}

			inline void startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const xercesc::Attributes& attrs)
			{
				char* tmp = xercesc::XMLString::transcode(localname);
				std::string elementName = tmp;
				xercesc::XMLString::release(&tmp);

				if(elementName == "page")
					_insidePage = true;
				else
					if(_insidePage)
					{
						if(elementName == "redirect"){
							_currentArticleData.IsRedirect = true;
							const char* bla = "title"; 
							XMLCh* titleTmp= xercesc::XMLString::transcode(bla);
							auto tmpAttr = attrs.getValue(titleTmp);
							char* tmpAttr2 = xercesc::XMLString::transcode(tmpAttr);
							_currentArticleData.RedirectTarget = tmpAttr2;
							xercesc::XMLString::release(&tmpAttr2);
							xercesc::XMLString::release(&titleTmp);
						}

						_elementStack.push_back(elementName);
					}
			}

			inline void endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname)
			{
				char* tmp = xercesc::XMLString::transcode(localname);
				std::string elementName = tmp;
				xercesc::XMLString::release(&tmp);

				if(elementName == "page")
				{
					if(_handlerProperties.TitleFilter){
						if(_handlerProperties.TitleFilter(_currentArticleData.MetaData.at("title")))
							_articleHandler.HandleArticle(_currentArticleData);
					}
					else
						_articleHandler.HandleArticle(_currentArticleData);

					_currentArticleData.MetaData.clear();
					_currentArticleData.IsRedirect = false;
					_currentArticleData.RedirectTarget = "";
					_currentArticleData.Content.clear();
					_insidePage = false;
					_elementStack.clear();
					_articleCount++;
					if(_articleCount % _handlerProperties.ProgressReportInterval == 0 && _handlerProperties.ProgressCallback)
					{
						_handlerProperties.ProgressCallback(_articleCount);
					}
				}
				else
					if(_insidePage)
						_elementStack.pop_back();
			}


			inline void endDocument()
			{
				if(_handlerProperties.ProgressCallback)
					_handlerProperties.ProgressCallback(_articleCount);
			}

			inline void characters(const XMLCh* const chars, const XMLSize_t length)
			{
				if(_elementStack.size() > 0 && (_elementStack.back() != "text" || _extractText))
				{
					char* tmp = xercesc::XMLString::transcode(chars);
					std::string str = tmp;
					xercesc::XMLString::release(&tmp);

					if(_elementStack.back() == "text")
						_currentArticleData.Content += str;
					else 
					{
						if(_insidePage && !str.empty())
						{
							std::string key = _elementStack[0];
							for(std::size_t i = 1; i < _elementStack.size(); i++)
								key += "." + _elementStack[i];

							while(_currentArticleData.MetaData.find(key) != _currentArticleData.MetaData.end())
								key += ".";

							_currentArticleData.MetaData.insert({ key, str });
						}
					}
				}
			}

			inline void fatalError(const xercesc::SAXParseException& exception)
			{
				char* message = xercesc::XMLString::transcode(exception.getMessage());

				std::stringstream ss;
				ss << "Fatal Error: " << message << " at line: " << exception.getLineNumber() << std::endl;
				throw std::runtime_error(ss.str());

				xercesc::XMLString::release(&message);
			}

		private:
			AbstractWikiPageHandler& _articleHandler;
			const WikiDumpHandlerProperties& _handlerProperties;
			WikiPageData _currentArticleData;
			std::vector<std::string> _elementStack;
			bool _insidePage;
			bool _extractText;
			std::size_t _articleCount;
	};

}
