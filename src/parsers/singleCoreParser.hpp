#pragma once

#include <functional>

// xerces
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
// #include <xercesc/util/Base64.hpp>

#include "../handlers/abstractWikiPageHandler.hpp"
#include "../handlers/wikiDumpXercesHandler.hpp"
#include "../handlers/wikiDumpHandlerProperties.hpp"

namespace WikiXmlDumpXerces {

	class SingleCoreParser {

		public: 
			inline SingleCoreParser(AbstractWikiPageHandler& pageHandler, const WikiDumpHandlerProperties& properties)
				:PageHandler(pageHandler),
				HandlerProperties(properties)
			{}

			inline void Run(const std::string path)
			{
				xercesc::SAX2XMLReader* parser = xercesc::XMLReaderFactory::createXMLReader();
				parser->setFeature(xercesc::XMLUni::fgSAX2CoreValidation, true);
				parser->setFeature(xercesc::XMLUni::fgSAX2CoreNameSpaces, true);   // optional
				parser->setFeature(xercesc::XMLUni::fgXercesSchema , false);   // optional

				// set up call back handlers
				WikiDumpXercesHandler handler(PageHandler, HandlerProperties, path);

				parser->setContentHandler(&handler);
				parser->setErrorHandler(&handler);

				// run parser
				parser->parse(path.c_str());
				delete parser;
			}

			inline void RunOnString(const std::string content)
			{
				xercesc::SAX2XMLReader* parser = xercesc::XMLReaderFactory::createXMLReader();
				parser->setFeature(xercesc::XMLUni::fgSAX2CoreValidation, true);
				parser->setFeature(xercesc::XMLUni::fgSAX2CoreNameSpaces, true);   // optional
				parser->setFeature(xercesc::XMLUni::fgXercesSchema , false);   // optional

				// set up call back handlers
				WikiDumpXercesHandler handler(PageHandler, HandlerProperties, "[in_memory_file]");

				parser->setContentHandler(&handler);
				parser->setErrorHandler(&handler);

				xercesc::MemBufInputSource source(reinterpret_cast<const XMLByte*>(content.c_str()), content.length(), "wiki_xml_dump_xerces");

				// run parser
				parser->parse(source);
				delete parser;
			}

			template<typename TIterator>
			inline void Run(const TIterator begin, const TIterator end)
			{
				for(auto it = begin; it != end; it++)
					Run(*it);
			}

			AbstractWikiPageHandler& PageHandler;
			const WikiDumpHandlerProperties& HandlerProperties;
	};

}
