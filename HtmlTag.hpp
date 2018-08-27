#ifndef HTMLTAG_HPP
#define HTMLTAG_HPP
#include <string>
#include <map>
#include <list>
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

        void addInnerContent(string cont){
            inner_content.push_back(cont);
        }

        string getTagName(){
            return tag_name;
        }
        list<string> getInnerContent(){
            return inner_content;
        }
        multimap<string,string> getAttributes(){
            return attributes;
        }
        multimap<string, HtmlTag*> getNestedTags(){
            return nestedTags;
        }
    private:
        string tag_name;
        list<string> inner_content;
        multimap<string,string> attributes;
        multimap<string, HtmlTag*> nestedTags;
};
#endif