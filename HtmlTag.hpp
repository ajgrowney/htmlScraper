#ifndef HTMLTAG_HPP
#define HTMLTAG_HPP
#include <string>
#include <map>
using namespace std;

class HtmlTag {
    public:
        HtmlTag(string name){
            tag_name = name;
        };

        void insertAttribute(string attr_name, string attr_value){
            attributes.insert({attr_name, attr_value});
        }

        void insertNestedTag(string tag_name, HtmlTag* nested_tag){
            nestedTags.insert({tag_name, nested_tag});
        }
        multimap<string,string> getAttributes(){
            return attributes;
        }
        multimap<string, HtmlTag*> getNestedTags(){
            return nestedTags;
        }
    private:
        string tag_name;
        multimap<string,string> attributes;
        multimap<string, HtmlTag*> nestedTags;
};
#endif