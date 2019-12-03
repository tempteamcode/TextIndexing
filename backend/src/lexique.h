/**
 * @file lexique.h
 * @author Quentin Guye, Nathan Mesnard, Paul-Emmanuel Sotir, Tianjian Ye
 * @brief 
 * @version 0.1
 * @date 2019-12-02
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <iostream>
#include <map>

namespace Document {

    /**
     * @brief Lexical enumeration of Tags (unsigned int as underlying type)
     */
    enum TagLex : unsigned int {
        EMPTY_TAG,
        P_TAG,
        DOCNO_TAG,
        DOCID_TAG,
        DATE_TAG,
        SECTION_TAG,
        LENGTH_TAG,
        HEADLINE_TAG,
        BYLINE_TAG,
        DATELINE_TAG,
        TEXT_TAG,
        GRAPHIC_TAG,
        TYPE_TAG,
        SUBJECT_TAG,
        CORRECTION_DATE_TAG,
        CORRECTION_TAG,
    };

    /**
     * @brief Maps TagLex enumeration tags to their respective text string
     */
    std::map<TagLex, std::string> tagLexStrMap = 
    {
        { EMPTY_TAG, "" },
        { P_TAG, "P" },
        { DOCNO_TAG, "DOCNO" },
        { DOCID_TAG, "DOCID" },
        { DATE_TAG, "DATE" },
        { SECTION_TAG, "SECTION" },
        { LENGTH_TAG, "LENGTH" },
        { HEADLINE_TAG, "HEADLINE" },
        { BYLINE_TAG, "BYLINE" },
        { DATELINE_TAG, "DATELINE" },
        { TEXT_TAG, "TEXT" },
        { GRAPHIC_TAG, "GRAPHIC" },
        { TYPE_TAG, "TYPE" },
        { SUBJECT_TAG, "SUBJECT" },
        { CORRECTION_DATE_TAG, "CORRECTION-DATE" },
        { CORRECTION_TAG, "CORRECTION" },
    };

    /**
     * @brief Maps Tag text strings to their respective TagLex enumeration value (inverted tagLexStrMap)
     */
    std::map<std::string, TagLex> strTagLexMap;

    inline std::string toStr(const TagLex lex) { 
    	auto pos = tagLexStrMap.find(lex);
		if (pos != tagLexStrMap.end()) return (*pos).second;
		else return std::string();
    }

    inline TagLex toTagLex(const std::string & str) {
		auto pos = strTagLexMap.find(str);
		if (pos != strTagLexMap.end()) return (*pos).second;
		else return EMPTY_TAG;
    }

    void initializeLexique() {
        for (auto& item : tagLexStrMap) {
            strTagLexMap[item.second] = item.first;
        }
    }
};
