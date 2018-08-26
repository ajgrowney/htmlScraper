#ifndef HTMLDOC_HPP
#define HTMLDOC_HPP
#include <string>
#include <map>
#include "HtmlTag.hpp"
using namespace std;

class HtmlDoc{
    public:
        HtmlDoc(){
            num_div_tags = 0;
            num_lines = 0;
        }
        void addLine(){
            num_lines++;
        }
        int getLines(){
            return num_lines;
        }
        void addDivTag(){
            num_div_tags++;
        }
        int getNumTags(){
            return num_div_tags;
        }
        void insertDocumentTag(string tag_name, HtmlTag* tag){
            documentTags.insert({tag_name, tag});
        }
        multimap<string, HtmlTag*> getDocTags(){
            return documentTags;
        }
    private:
        int num_div_tags;
        int num_lines;
        multimap<string, HtmlTag*> documentTags;
};

#endif