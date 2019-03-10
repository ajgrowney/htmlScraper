#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include "HtmlTag.hpp"
#include "HtmlDoc.hpp"
#include <iostream>
#include <stack>
#include <map>
#include <string>
using namespace std;

class Functions{

public:

    void processTagRequest(HtmlDoc* doc, string req){
        multimap<string, HtmlTag*> docTags = doc->getDocTags();
        multimap<string, HtmlTag*>::iterator attr_itr;
        multimap<string, string>::iterator val_itr;
        HtmlTag* currentTag = nullptr;

        int delim_loc = req.find_first_of(".~");
        if(delim_loc == -1){
            cout << "Invalid request\n";
            return;
        }
        string tag_name = req.substr(0, delim_loc);
        req.erase(0,delim_loc);
        attr_itr = docTags.find(tag_name);
        if(attr_itr == docTags.end()){
            cout << "Not Found!\n";
            return;
        }else{
            currentTag = attr_itr->second;
        }

        while(req.at(0) == '.'){
            req.erase(0,1);
            delim_loc = req.find_first_of(".~");
            attr_itr = currentTag->getNestedTags().find(req.substr(0, delim_loc));
            if(attr_itr == currentTag->getNestedTags().end()){
                cout << "Not Found!\n";
                return;
            }else{
                currentTag = attr_itr->second;
                req.erase(0, delim_loc);
            }
        }
        if(req.at(0) != '~'){
            cout << "Invalid syntax\n";
        }else{
            req.erase(0,1);
            val_itr = currentTag->getAttributes().find(req);
            if(val_itr == currentTag->getAttributes().end()){
                cout << "Not Found!\n";
            }else{
                cout << val_itr->second<<endl;
            }
        }
        


    }

    void parseLine(HtmlDoc* scrapeDoc, stack<HtmlTag*>* tagStack, string line_in){
        scrapeDoc->addLine();

        int end_whitesapce = line_in.find_first_not_of(" \t");

        if(end_whitesapce != -1){

            line_in.erase(0,end_whitesapce); // Erase whitespace before meaningful content

            while(line_in.length() > 0){
                int tag_loc = line_in.find_first_of('<');

                if(tag_loc == -1 && !tagStack->empty()){
                    // No tags in the line, add the whole line to top of stack content
                    tagStack->top()->addInnerContent(line_in);
                    line_in.erase(0,line_in.length());
                }else if(tag_loc != -1){
                    // A tag exists somewhere in the line
                    if(!tagStack->empty() && tag_loc !=0){
                        tagStack->top()->addInnerContent(line_in.substr(0,tag_loc)); // Add all up to the tag
                    }
                    line_in.erase(0,tag_loc);
                    line_in = parseTag(scrapeDoc, tagStack, line_in);
                }
            }


        }
    };

    string parseTag(HtmlDoc* scrapeDoc, stack<HtmlTag*>* tagStack, string line_in){
        line_in.erase(0,1); // Get rid of '<'

        // Determine if Opening or Closing tag
        bool isOpeningTag = (line_in.at(0) == '/') ? false : true;
        bool end_of_tag = false;

        //-------------Case: Opening Tag-------------------
        if(isOpeningTag){
            
            // Find first delimiter
            int delim_loc = line_in.find_first_of(" >");

            // Create an object for the opening tag for mapping
            string tagName = line_in.substr(0,delim_loc);
            HtmlTag* newHtmlTag = new HtmlTag(tagName);

            while(delim_loc != -1 && !end_of_tag){
                line_in.erase(0,delim_loc); // Erase tag name up to delimiter
                if(line_in.at(0) == '>'){
                    // End of Opening Tag: '>'
                    line_in.erase(0,1); //Erase the delimiter
                    end_of_tag = true;
                }else if(line_in.at(0) == ' '){
                    // Found Space: contains attribute
                    string attrName, attrVal;
                    int beginAttrName = line_in.find_first_not_of(' ');
                    line_in.erase(0,beginAttrName); // Erase the space(s)

                    int endAttrName = line_in.find_first_of(" =");

                    if(endAttrName != -1){
                        attrName = line_in.substr(0,endAttrName);
                        line_in.erase(0,endAttrName);
                        int beginAttrVal = line_in.find_first_of('"');
                        if(line_in.substr(0, beginAttrVal).find('=') == string::npos){
                            cout << "Invalid attribute: No assignment\n";
                        }else{
                            line_in.erase(0, beginAttrVal+1);
                            int endAttrVal = line_in.find_first_of('"');
                            attrVal = line_in.substr(0,endAttrVal);
                            newHtmlTag->insertAttribute(attrName, attrVal);
                            line_in.erase(0,endAttrVal);
                        }

                    }else{
                        cout << "Invalid attribute error\n";
                    }

                }
                delim_loc = line_in.find_first_of(" >");
            }
            if(!tagStack->empty()){
                tagStack->top()->insertNestedTag(tagName, newHtmlTag);
            }
            tagStack->push(newHtmlTag);
            return line_in;
        }else{
            //---------Case: Closing Tag---------------------
            line_in.erase(0,1); // Erase '/'
            string closeTagName = line_in.substr(0, line_in.find('>'));

            //---------Matches the correct tag----------------
            if(tagStack->top()->getTagName() == closeTagName){
                //cout << "Valid closing tag: "<< closeTagName <<endl;
                scrapeDoc->insertDocumentTag(closeTagName, tagStack->top());
                tagStack->pop();
                // Return remaining line after the '>' character for further parsing
                line_in.erase(0, line_in.find('>')+1); // Erase the closing tag and return remaining string
                return line_in;
            }else{
                cout << "Invalid  closing tag!";
                cout << "\nFound: "<< closeTagName;
                cout << "\nExpected: "<< tagStack->top()->getTagName()<<endl;
                exit(1);
            }
        }
        return "";
        }





};

#endif