#include <iostream>

#include "../../parsers/singleCoreParser.hpp"
#include "../../handlers/abstractWikiPageHandler.hpp"
#include "../../handlers/wikiDumpHandlerProperties.hpp"

struct PrintHandler : WikiXmlDumpXerces::AbstractWikiPageHandler {
	
	void HandleArticle(const WikiXmlDumpXerces::WikiPageData& data)
	{
		std::cout << data.MetaData.at("title") << std::endl;
	}

};

int main(int /*argc*/, char* argv[])
{
	WikiXmlDumpXerces::WikiDumpHandlerProperties properties;
	PrintHandler handler;

	WikiXmlDumpXerces::SingleCoreParser parser(handler, properties);

	xercesc::XMLPlatformUtils::Initialize();
	parser.Run(argv[1]);
	xercesc::XMLPlatformUtils::Terminate();

	return 0;
}
