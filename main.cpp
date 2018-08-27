#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <fstream>
#include "HtmlDoc.hpp"
#include "HtmlTag.hpp"
using namespace std;

void printMenu(){
        cout << "\nSelect a menu option\n";
        cout << "1) View number of lines total\n";
        cout << "2) View number of tags in document\n";
        cout << "3) View all tags with information\n";
        cout << "4) View specific element detail\n";
        cout << "5) View specific attribute value\n";
        cout << "6) Exit the program\n";
        cout << "Select: ";
}

void parseTag(HtmlDoc* scrapeDoc, stack<HtmlTag*>* tagStack, string line_in){
    line_in.erase(0,1); // Get rid of '<'

    // Determine if Opening or Closing tag
    bool isOpeningTag = (line_in.at(0) == '/') ? false : true;

    //-------------Case: Opening Tag-------------------
    if(isOpeningTag){
        //cout << "Found Opening Tag: " << line_in.substr(0, line_in.find('>')) << "\n";
        
        // Find first delimiter
        int delim_loc = line_in.find_first_of(" >");

        // Create an object for the opening tag for mapping
        string tagName = line_in.substr(0,delim_loc);
        HtmlTag* newHtmlTag = new HtmlTag(tagName);

        while(delim_loc != -1){
            line_in.erase(0,delim_loc); // Erase tag name up to delimiter
            if(line_in.at(0) == '>'){
                // End of Opening Tag: '>'
                line_in.erase(0,1); //Erase the delimiter
            }else if(line_in.at(0) == ' '){
                // Found Space: contains attribute
                line_in.erase(0,1); // Erase the space

                string attrName, attrVal;
                int endAttrName = line_in.find_first_of(" =");
                attrName = line_in.substr(0,endAttrName);
                line_in.erase(0,endAttrName);
                int beginAttrVal = line_in.find_first_of('"');
                line_in.erase(0,beginAttrVal+1);
                int endAttrVal = line_in.find_first_of('"');
                attrVal = line_in.substr(0,endAttrVal);

                newHtmlTag->insertAttribute(attrName, attrVal);
            }
            delim_loc = line_in.find_first_of(" >");
        }
        if(!tagStack->empty()){
            tagStack->top()->insertNestedTag(tagName, newHtmlTag);
        }
        tagStack->push(newHtmlTag);
    }else{
        //---------Case: Closing Tag---------------------
        line_in.erase(0,1); // Erase '/'
        string closeTagName = line_in.substr(0, line_in.find('>'));

        //---------Matches the correct tag----------------
        if(tagStack->top()->getTagName() == closeTagName){
            //cout << "Valid closing tag: "<< closeTagName <<endl;
            scrapeDoc->insertDocumentTag(closeTagName, tagStack->top());
            tagStack->pop();
        }else{
            cout << "Invalid  closing tag!";
            cout << "\nFound: "<< closeTagName;
            cout << "\nExpected: "<< tagStack->top()->getTagName()<<endl;
            exit(1);
        }
    }
}

// Assuming the line begins with a '<' 
void parseLine(HtmlDoc* scrapeDoc, stack<HtmlTag*>* tagStack, string line_in){
    scrapeDoc->addLine();

    int end_whitesapce = line_in.find_first_not_of(" \t");

    // Found the opening '<' for a tag
    if(end_whitesapce != -1){
        line_in.erase(0,end_whitesapce);
        if(line_in.at(0) == '<'){
            // -------- Found a tag -------------
            // Parse to determine open or closing tag and handle
            parseTag(scrapeDoc, tagStack, line_in);
        }else{
            // ----------Found inner HTML-----------
            if(!tagStack->empty()){
                // Add the inner html to the current tag on the top of the stack
                tagStack->top()->addInnerContent(line_in);
            }
        }
    }
}

int main(int argc, char* argv[]){

    //---------------Main Menu-----------------
    cout << "Welcome to Page Scraper\nA C++ HTML file scraper\n";
    cout << "\nEnter the filename you would like to scrape from this directory: ";
    ifstream myfile;
    string file_in;
    cin >> file_in;
    myfile.open(file_in);

    //---------------
    while(!myfile.is_open()){
        cout << "Must enter a valid filename. Try again: ";
        cin >> file_in;
        if(!file_in.empty()){
            myfile.open(file_in);
        }
    }
    cout << "Scraping: "<< file_in<<endl;

    HtmlDoc* myDocument = new HtmlDoc();
    stack<HtmlTag*>* tagStack = new stack<HtmlTag*>();
    string input;

    while(getline(myfile, input)){
        parseLine(myDocument, tagStack, input);
    }


    /*
    ---------Interactive Menu Choices-----------------
    TODO: Select a specific tag and view details of it
    */
    int menuChoice = 0;
    while(menuChoice != 6){
        printMenu();
        cin >> menuChoice;
        if(menuChoice == 1){
            cout << "Num of Lines: "<< myDocument->getLines()<<endl;
        }else if(menuChoice == 2){
            cout << "Num of Tags: "<< myDocument->getDocTags().size()<<endl;
        }else if(menuChoice == 3){
            multimap<string, HtmlTag*> allTags = myDocument->getDocTags();
            for(auto itr = allTags.begin(); itr != allTags.end(); itr++){
                cout << "\nTag Name: "<< itr->second->getTagName()<<endl;

                // Print out the attributes
                multimap<string, string> attr_pairs = itr->second->getAttributes();
                if(attr_pairs.size() > 0){
                    cout << "List of attribute pairs: \n";
                    for(auto attr_itr = attr_pairs.begin(); attr_itr != attr_pairs.end(); attr_itr++){
                        cout << "--> Attr: "<<attr_itr->first << ", Val: "<< attr_itr->second<<endl;
                    }
                }else{
                    cout << "Contains no attributes\n";
                }

                // Print out nested tag names
                multimap<string, HtmlTag*> nested_tags = itr->second->getNestedTags();
                if(nested_tags.size() > 0){
                    cout << "List of nested tags: \n";
                    for(auto nest_itr = nested_tags.begin(); nest_itr != nested_tags.end(); nest_itr++){
                        cout << "--> Nested Tag Name: "<< nest_itr->second->getTagName()<<endl;
                    }
                }else{
                    cout << "Contains no nested tags\n";
                }

                // Print out the content
                cout << "Inner content: \n-->"<< itr->second->getInnerContent()<<endl;
            }
        }else if(menuChoice == 4){

        }else if(menuChoice == 5){
            cout << "Structure your request in the following format 'tag1.tag2~attr'\n";
            cout << "Enter your request: ";
            HtmlTag* request_tag = nullptr;
            string find_attr;
            cin >> find_attr;
            while(find_attr.empty()){
                cout << "Enter a non-empty request: ";
                cin >> find_attr;
            }
            int delim_loc = find_attr.find_first_of(".~");
            

        }else if(menuChoice == 6){
            cout << "Goodbye\n";
        }

    }

    delete myDocument;
    delete tagStack;
    return 0;
}