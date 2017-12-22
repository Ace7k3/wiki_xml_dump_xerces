// stdlib
#include <iostream>
#include <iomanip>
#include <map>
#include <future>
#include <thread>
#include <fstream>

// xerces
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>

#include <boost/algorithm/string/trim.hpp>

#include "../../../libs/cxxopts/include/cxxopts.hpp"

// local files
#include "countPagesXercesHandler.hpp"

namespace {
	class SingleFileCounter {
		public:
			SingleFileCounter(std::string path)
				:_path(path)
			{}

			std::size_t operator()(void)
			{
				xercesc::SAX2XMLReader* parser = xercesc::XMLReaderFactory::createXMLReader();
				parser->setFeature(xercesc::XMLUni::fgSAX2CoreValidation, true);
				parser->setFeature(xercesc::XMLUni::fgSAX2CoreNameSpaces, true);   // optional
				parser->setFeature(xercesc::XMLUni::fgXercesSchema , false);   // optional

				CountPagesXercesHandler handler;
				parser->setContentHandler(&handler);
				parser->setErrorHandler(&handler);

				parser->parse(_path.c_str());
				delete parser;

				return handler.count();
			}

		private:
			std::string _path;
	};


	std::vector<std::string> read_lines_from_file(std::istream& input)
	{
		std::vector<std::string> lines;
		std::string line;
		while(std::getline(input,line))
		{
			boost::trim(line);
			if(!line.empty() && line.front() != '#')
				lines.push_back(line);
		}

		return lines;
	}
}



int main(int argc, char* argv[])
{
	cxxopts::Options options("count_pages", "Counts the pages in a Wikipedia xml dump. Can be used for displaying progress in other parsers.");
	options.add_options()
		("help", "Produce this help message.")
		("i,input-paths-file", "File of which each line is absolute path to an xml file that is part of a Wikipedia dump.", cxxopts::value<std::string>())
		;
	auto parsed_options = options.parse(argc, argv);

	// display help if --help was specified
	if (parsed_options.count("help")){
		std::cout << options.help() << std::endl;
		return 0;
	}

	if(!parsed_options.count("input-paths-file"))
		throw std::logic_error("Please specify the parameter --input-paths-file.");

	const std::string input_paths_filepath = parsed_options["input-paths-file"].as<std::string>();
	std::ifstream input_paths_file(input_paths_filepath);
	const auto paths = read_lines_from_file(input_paths_file);

	if(paths.size() == 0)
		throw std::logic_error("Paths file does not contain any uncommented lines.");

	// init xerces
	xercesc::XMLPlatformUtils::Initialize();

	std::vector<std::future<std::size_t>> futures;
	futures.reserve(paths.size());
	for (auto path : paths)
		futures.emplace_back(std::async(std::launch::async, SingleFileCounter(path)));

	std::map<std::string, std::size_t> page_counts;
	for (std::size_t i = 0; i < futures.size(); i++)
		page_counts[paths[i]] = futures[i].get();

	xercesc::XMLPlatformUtils::Terminate();

	for (auto file : page_counts) 
		std::cout << file.first << " " << file.second << std::endl;

	return 0;
}
