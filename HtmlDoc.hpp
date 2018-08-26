#ifndef HTMLDOC_HPP
#define HTMLDOC_HPP
#include <string>
#include <map>
#include "HtmlTag.hpp"
using namespace std;

class HtmlDoc{
    public:
        HtmlDoc(){
            num_lines = 0;
        }
        void addLine(){
            num_lines++;
        }
        int getLines(){
            return num_lines;
        }
        int getNumTags(){
            return documentTags.size();
        }
        void insertDocumentTag(string tag_name, HtmlTag* tag){
            documentTags.insert({tag_name, tag});
        }
        multimap<string, HtmlTag*> getDocTags(){
            return documentTags;
        }
    private:
        int num_lines;
        multimap<string, HtmlTag*> documentTags;
};

#endif