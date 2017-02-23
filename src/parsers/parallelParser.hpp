#pragma once

#include <functional>
#include <future>
#include <thread>

// xerces
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>

#include "../handlers/abstractWikiPageHandler.hpp"
#include "../handlers/wikiDumpXercesHandler.hpp"
#include "../handlers/wikiDumpHandlerProperties.hpp"

namespace WikiXmlDumpXerces {

	template<typename TPageHandler>
	class ParallelParser {

		public: 
			inline ParallelParser(std::function<TPageHandler(void)> handlerFactory, const WikiDumpHandlerProperties& properties)
				:HandlerProperties(properties),
				_pageHandlerFactory(handlerFactory)
			{}

			// run only single core because parallelization happens only on individual file level
			inline void Run(const std::string path)
			{
				xercesc::SAX2XMLReader* parser = xercesc::XMLReaderFactory::createXMLReader();
				parser->setFeature(xercesc::XMLUni::fgSAX2CoreValidation, true);
				parser->setFeature(xercesc::XMLUni::fgSAX2CoreNameSpaces, true);   // optional
				parser->setFeature(xercesc::XMLUni::fgXercesSchema , false);   // optional

				// set up call back handlers
				_pageHandlers.push_back(_pageHandlerFactory());
				WikiDumpXercesHandler handler(_pageHandlers.back(), HandlerProperties, path);

				parser->setContentHandler(&handler);
				parser->setErrorHandler(&handler);

				// run parser
				parser->parse(path.c_str());
				delete parser;
			}

			template<typename TIterator>
			inline void Run(const TIterator begin, const TIterator end)
			{
				std::size_t length = std::distance(begin, end);

				std::vector<std::future<void>> futures;
				futures.reserve(length);
				_pageHandlers.reserve(length);
				for(std::size_t i = 0; i < length; i++)
					_pageHandlers.push_back(_pageHandlerFactory());

				std::size_t iPath = 0;
				for (auto it = begin; it != end; it++) 
				{
					std::string path = *it;
					futures.emplace_back(std::async(std::launch::async, [path, this, iPath]() { 
						xercesc::SAX2XMLReader* parser = xercesc::XMLReaderFactory::createXMLReader();
						parser->setFeature(xercesc::XMLUni::fgSAX2CoreValidation, true);
						parser->setFeature(xercesc::XMLUni::fgSAX2CoreNameSpaces, true);   // optional
						parser->setFeature(xercesc::XMLUni::fgXercesSchema , false);   // optional

						// set up call back handlers
						WikiXmlDumpXerces::WikiDumpXercesHandler handler(this->_pageHandlers[iPath], this->HandlerProperties, path);

						parser->setContentHandler(&handler);
						parser->setErrorHandler(&handler);

						// run parser
						parser->parse(path.c_str());
						delete parser;
					}));
				}

				// only return after all files have been completely parsed
				for (auto& future : futures)
					future.get();
			}

			inline const std::vector<TPageHandler>& PageHandlers() const
			{
				return _pageHandlers;
			}

			const WikiDumpHandlerProperties& HandlerProperties;
		private:

			std::function<TPageHandler(void)> _pageHandlerFactory;
			std::vector<TPageHandler> _pageHandlers;
	};

}
