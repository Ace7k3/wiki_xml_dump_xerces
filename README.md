# Description

This small header-only C++ library tries to encapsulate a good base for parsing large XML-dumps of Wikipedia using the Apache Xerces framework. It provides simple wrapper classes which support single core and parallel parsing of one or multiple xml files at once.

We provide an abstract handler class which provides an interface for retrieving pages and their meta data from the xml file. To use the library you have to provide an implementation of the handler processing the page depending on your needs.  

# Compiling

- The library code uses C++11 features (e.g. std::future) for parallelization, so you must compile it using at least the c++11 standard
- To compile the source you need to have the Xerces library installed. If you are using a Debian based distribution (e.g. Ubuntu) this can be done by simply running 
	```
	sudo apt-get install libxerces-c-dev
	```
	in the command line. You then have to link against xerces.
- If you want to run the parallel parser you additionally have to link against your operation systems thread library, e.g. pthread for Linux

# Usage

## The structure of a Wikipedia XML dump

Dumps that can be downloaded from [https://en.wikipedia.org/wiki/Wikipedia:Database_download#Where_do_I_get_it.3F](https://en.wikipedia.org/wiki/Wikipedia:Database_download#Where_do_I_get_it.3F) usually look like the following

```
<mediawiki xmlns="http://www.mediawiki.org/xml/export-0.10/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://www.mediawiki.org/xml/export-0.10/ http://www.mediawiki.org/xml/export-0.10.xsd" version="0.10" xml:lang="en">
	<siteinfo>
		<sitename>Wikipedia</sitename>
		<dbname>enwiki</dbname>
		<base>https://en.wikipedia.org/wiki/Main_Page</base>
		<generator>MediaWiki 1.28.0-wmf.10</generator>
		<case>first-letter</case>

		<namespaces>
			<namespace key="-2" case="first-letter">Media</namespace>
			<namespace key="-1" case="first-letter">Special</namespace>
			[...]
			<namespace key="2600" case="first-letter">Topic</namespace>
		</namespaces>
	</siteinfo>
	
	
	<page>
		<title>Talk:The Amazing Spider-Man</title>
		<ns>1</ns>
		<id>1066</id>
		<revision>
		<id>645898156</id>
		<parentid>645790397</parentid>
		<timestamp>2015-02-06T14:28:14Z</timestamp>
		<contributor>
		<username>Spidey104</username>
		<id>9768394</id>
		</contributor>
		<comment>Undid 5 revisions by [[Special:Contributions/86.43.182.227|86.43.182.227]] ([[User talk:86.43.182.227|talk]]) This is a talk page, and this article isn't the appropriate location for that information either.</comment>
		<model>wikitext</model>
		<format>text/x-wiki</format>
		<text xml:space="preserve">
			{{comicsproj
				&lt;!-- B-Class checklist --&gt;
			 }}
		</text>
		<sha1>nlmlimhq7nzlfe0doek47bjma858rxi</sha1>
		</revision>
	</page>
	
	<page>
		[...]
	</page>
	
	[...]
</mediawiki>

```

Here each ```<page>``` element represents an article page on Wikipedia and its ```<text>``` element contains the syntax of the article. The remaining children of the page element are meta data.

## WikiPageHandlers

**Note:** The library introduces the *WikiXmlDumpXerces* namespace, which has to be used to accesses every class / struct mentioned from here on.

To use the library you have to provide a class inheriting from the abstract class *AbstractWikiPageHandler* which is essentially declared as
```
class AbstractWikiPageHandler {
	public:
		virtual void HandleArticle(const WikiPageData&) = 0;
};
```

So you need to implement the *HandleArticle* method. It receives a WikiPageData object containing all extracted data for one page. In the method you compute whats of interest for you about the article.

## Parsers

After implementing the page handler the only thing left to do is to decide if you want to use the single core or the parallel parser.

To use the single core parser simply create an instance of your handler and pass it to the *SingleCoreParser* constructor, as well as an *WikiDumpHandlerProperties* object which we will explain later.

To create a parallel core parser you have to first pass your handler type as a template parameter. The constructor than takes as arguments a std::function<YourHandlerType> which encapsulates the generation of your handlers, as well as an *WikiDumpHandlerProperties* object.

After initializing Xerces the parsers can be run by calling their ```Run``` methods and providing either one path to an xml file or a container containing multiple paths.
```
void Run(const std::string path)

template<typename TIterator>
void Run(const TIterator begin, const TIterator end)
```

## Example

Example code for a single core parser printing articles titles in the xml file could look like

```
#include <iostream>

#include "parsers/singleCoreParser.hpp"
#include "handlers/abstractWikiPageHandler.hpp"
#include "handlers/wikiDumpHandlerProperties.hpp"

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

	WikiXmlDumpXerces::SingleCoreParser parser(handler, properties);

	xercesc::XMLPlatformUtils::Initialize();
	parser.Run(argv[1]);
	xercesc::XMLPlatformUtils::Terminate();

	return 0;
}
```

More examples can be found in the ```examples``` folder. To learn more about the available properties of the parsers (e.g. filters based on title, progress callbacks) you have to look in the source. It should be self-explanatory :-).



