#pragma once

#include <functional>

namespace WikiXmlDumpXerces {

	struct WikiDumpHandlerProperties {
		inline WikiDumpHandlerProperties()
			:TitleFilter([](const std::string&){ return true; }),
			ProgressCallback([](std::size_t, std::string, std::string) {}),
			ProgressReportInterval(10000),
			ExtractPageContent(true)
			{}

		std::function<bool(const std::string&)> TitleFilter;
		std::function<void(std::size_t, std::string, std::string)> ProgressCallback; // [article_number, current_file_path, current_article_title]
		std::size_t ProgressReportInterval;
		bool ExtractPageContent;
	};

}
