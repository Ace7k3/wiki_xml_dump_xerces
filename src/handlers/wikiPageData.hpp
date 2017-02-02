#pragma once

#include <map>
#include <string>

namespace WikiXmlDumpXerces {

	struct WikiPageData {
		WikiPageData()
			:IsRedirect(false)
		{}

		std::map<std::string, std::string> MetaData;
		std::string Content;	

		bool IsRedirect;
		std::string RedirectTarget;
	};

}
