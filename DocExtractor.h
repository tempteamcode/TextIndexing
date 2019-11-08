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
	std::vector<std::string> DATELINE;
	std::vector<std::string> TEXT;
	std::vector<std::string> GRAPHIC;
	std::vector<std::string> TYPE;
	std::vector<std::string> SUBJECT;
	std::vector<std::string> CORRECTION_DATE;
	std::vector<std::string> CORRECTION;
};

typedef void(*DocExtractorCallback)(DocumentData& documentExtracted);

class DocExtractor
{
public:
	DocExtractor(DocExtractorCallback callback) : callback(callback) {};

	void parseToken(std::string& token);

private:
	DocExtractorCallback callback;

	enum contexts { ctxNONE, ctxDOCNO, ctxDOCID, ctxDATE, ctxSECTION, ctxLENGTH, ctxHEADLINE, ctxBYLINE, ctxDATELINE, ctxTEXT, ctxGRAPHIC, ctxTYPE, ctxSUBJECT, ctxCORRECTION_DATE, ctxCORRECTION };

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
		{ "<DATELINE>", ctxDATELINE },
		{ "<TEXT>", ctxTEXT },
		{ "<GRAPHIC>", ctxGRAPHIC },
		{ "<TYPE>", ctxTYPE },
		{ "<SUBJECT>", ctxSUBJECT },
		{ "<CORRECTION-DATE>", ctxCORRECTION_DATE },
		{ "<CORRECTION>", ctxCORRECTION },
	};

	DocumentData documentCurrent;
};

bool starts_with(std::string test, std::string begin)
{
	return test.substr(0, begin.length()) == begin;
}

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
				else if (token == "<DOC>")
				{
					documentCurrent = DocumentData();
				}
				else if (token == "<ROWRULE>" || token == "<TABLEROW>" || token == "<CELLRULE>" || token == "<TABLECELL>");
				else if (starts_with(token, "<TABLE ") || starts_with(token, "<TABLECELL "));
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
		case (ctxDATELINE):
			documentCurrent.DATELINE.push_back(token);
			break;
		case (ctxTEXT):
			documentCurrent.TEXT.push_back(token);
			break;
		case (ctxGRAPHIC):
			documentCurrent.GRAPHIC.push_back(token);
			break;
		case (ctxTYPE):
			documentCurrent.TYPE.push_back(token);
			break;
		case (ctxSUBJECT):
			documentCurrent.SUBJECT.push_back(token);
			break;
		case (ctxCORRECTION_DATE):
			documentCurrent.CORRECTION_DATE.push_back(token);
			break;
		case (ctxCORRECTION):
			documentCurrent.CORRECTION.push_back(token);
			break;
		case (ctxNONE):
			std::cerr << "no context?" << std::endl;
			break;
		default:
			std::cerr << "forgotten context?" << std::endl;
			break;
		}
	}
}
