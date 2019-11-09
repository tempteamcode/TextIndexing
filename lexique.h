#include <iostream>
#include <map>

namespace Document {

    enum TagLex : unsigned int  {
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
        CORRECTION_TAG
    };

    std::map< TagLex, std::string > tagLexStrMap = 
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
        { CORRECTION_TAG, "CORRECTION" }
    };

    std::map< std::string, TagLex > strTagLexMap;

    inline std::string toStr( const TagLex lex ) { 
        try {
            auto str = tagLexStrMap.at( lex );
            return str;
        } catch(...) {
            return "";
        }
    }

    inline TagLex toTagLex( const std::string & str ) {
        try {
            auto lex = strTagLexMap.at( str );
            return lex;
        } catch(...) {
            return EMPTY_TAG;
        }
    }

    void initializeLexique() {
        for( auto & item : tagLexStrMap ) {
            strTagLexMap[ item.second ] = item.first;
        }
    }

};