#include <functional>
#include <string>

namespace WikiXmlDumpXerces {
	inline std::function<bool(const std::string&)> only_articles() {
		return [](const std::string& title) {
			return !(
					title.substr(0,5) == "User:" 
					|| title.substr(0,10) == "Wikipedia:" 
					|| title.substr(0,5) == "File:" 
					|| title.substr(0,14) == "Category talk:" 
					|| title.substr(0,9) == "Category:"
					|| title.substr(0,14) == "Template talk:"
					|| title.substr(0,9) == "Template:"
					|| title.substr(0,10) == "User talk:"
					|| title.substr(0,10) == "File talk:"
					|| title.substr(0,15) == "Wikipedia talk:"
					);
		};
	}

	inline std::function<bool(const std::string&)> only_talk_pages() {
		return [](const std::string& title) {
			return title.substr(0,5) == "Talk:";
		};
	}

	inline std::function<bool(const std::string&)> only_categories() {
		return [](const std::string& title) {
			return title.substr(0,5) == "Category:";
		};
	}
}
