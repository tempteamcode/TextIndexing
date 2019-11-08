#pragma once

#include <map>

struct DocumentData
{
	std::vector<std::string> DOCNO;
	std::vector<std::string> DOCID;
	std::vector<std::string> DATE;
	std::vector<std::string> SECTION;
	std::vector<std::string> LENGTH;
	std::vector<std::string> HEADLINE;
	std::vector<std::string> BYLINE;
	std::vector<std::string> TEXT;
};

typedef void(*DocExtractorCallback)(DocumentData& documentExtracted);

class DocExtractor
{
public:
	DocExtractor(DocExtractorCallback callback) : callback(callback) {};

	void parseToken(std::string& token);

private:
	DocExtractorCallback callback;

	enum contexts { ctxNONE, ctxDOCNO, ctxDOCID, ctxDATE, ctxSECTION, ctxLENGTH, ctxHEADLINE, ctxBYLINE, ctxTEXT };

	contexts context = ctxNONE;

	std::map<std::string, contexts> contextsmap = 
	{
		{ "<P>", ctxNONE },
		{ "<DOCNO>", ctxDOCNO },
		{ "<DOCID>", ctxDOCID },
		{ "<DATE>", ctxDATE },
		{ "<SECTION>", ctxSECTION },
		{ "<LENGTH>", ctxLENGTH },
		{ "<HEADLINE>", ctxHEADLINE },
		{ "<BYLINE>", ctxBYLINE },
		{ "<TEXT>", ctxTEXT },
	};

	DocumentData documentCurrent;
};

void DocExtractor::parseToken(std::string& token)
{
	if (token[0] == '<')
	{
		if (token.length() > 1)
		{
			if (token[1] != '/')
			{
				auto it = contextsmap.find(token);
				if (it != contextsmap.end())
				{
					contexts newcontext = (*it).second;
					if (newcontext != ctxNONE) context = newcontext;
				}
				else
				{
					// add missing token
					std::cout << token << std::endl;
				}
			}
			else if (token == "</DOC>")
			{
				callback(documentCurrent);
				documentCurrent = DocumentData();
			}
		}
	}
	else
	{
		switch (context)
		{
		case (ctxDOCNO):
			documentCurrent.DOCNO.push_back(token);
			break;
		case (ctxDOCID):
			documentCurrent.DOCID.push_back(token);
			break;
		case (ctxDATE):
			documentCurrent.DATE.push_back(token);
			break;
		case (ctxSECTION):
			documentCurrent.SECTION.push_back(token);
			break;
		case (ctxLENGTH):
			documentCurrent.LENGTH.push_back(token);
			break;
		case (ctxHEADLINE):
			documentCurrent.HEADLINE.push_back(token);
			break;
		case (ctxBYLINE):
			documentCurrent.BYLINE.push_back(token);
			break;
		case (ctxTEXT):
			documentCurrent.TEXT.push_back(token);
			break;
		case (ctxNONE):
		default:
			std::cerr << "no context?" << std::endl;
			break;
		}
	}
}
