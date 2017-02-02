#pragma once

#include "wikiPageData.hpp"

namespace WikiXmlDumpXerces {

	class AbstractWikiPageHandler {
		public:
			virtual void HandleArticle(const WikiPageData&) = 0;
	};

}
