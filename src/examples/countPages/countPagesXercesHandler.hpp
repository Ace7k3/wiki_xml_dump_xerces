#pragma once

#include <stack>
#include <vector>
#include <functional>

#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/Attributes.hpp>

class CountPagesXercesHandler : public xercesc::DefaultHandler {
	public:

		inline CountPagesXercesHandler()
			:_count(0)
		{}

		inline std::size_t count() const
		{
			return _count;
		}

		inline void startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const xercesc::Attributes& attrs)
		{
			char* tmp = xercesc::XMLString::transcode(localname);
			std::string elementName = tmp;
			xercesc::XMLString::release(&tmp);

			if(elementName == "page")
				_count++;
		}

	private:
		std::size_t _count;
};

