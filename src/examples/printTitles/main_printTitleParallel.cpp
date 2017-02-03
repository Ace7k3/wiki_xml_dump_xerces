#include <iostream>

#include "../../parsers/parallelParser.hpp"
#include "../../handlers/abstractWikiPageHandler.hpp"
#include "../../handlers/wikiDumpHandlerProperties.hpp"

struct PrintHandler : WikiXmlDumpXerces::AbstractWikiPageHandler {
	
	void HandleArticle(const WikiXmlDumpXerces::WikiPageData& data)
	{
		std::cout << data.MetaData.at("title") << std::endl;
	}

};

int main(int argc, char* argv[])
{
	WikiXmlDumpXerces::WikiDumpHandlerProperties properties;
	PrintHandler handler;

	WikiXmlDumpXerces::ParallelParser<PrintHandler> parser([](){ return PrintHandler(); }, properties);

	xercesc::XMLPlatformUtils::Initialize();
	std::vector<std::string> files;
	for(int i = 1; i < argc; i++)
		files.push_back(argv[i]);

	parser.Run(files.begin(), files.end());
	xercesc::XMLPlatformUtils::Terminate();

	return 0;
}
