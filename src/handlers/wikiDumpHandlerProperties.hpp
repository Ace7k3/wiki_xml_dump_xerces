#pragma once

#include <functional>

namespace WikiXmlDumpXerces {

	struct WikiDumpHandlerProperties {
		inline WikiDumpHandlerProperties()
			:TitleFilter([](const std::string&){ return true; }),
			ProgressCallback([](std::size_t) {}),
			ProgressReportInterval(10000),
			ExtractPageContent(true)
			{}

		std::function<bool(const std::string&)> TitleFilter;
		std::function<void(std::size_t)> ProgressCallback;
		std::size_t ProgressReportInterval;
		bool ExtractPageContent;
	};

}
