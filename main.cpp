#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <fstream>
#include "HtmlDoc.hpp"
#include "HtmlTag.hpp"
using namespace std;


void parseTag(HtmlDoc* scrapeDoc, string line_in){
    line_in.erase(0,1); // Get rid of '<'

    // Determine if Opening or Closing tag
    bool isOpeningTag = (line_in.at(0) == '/') ? false : true;

    //-------------Case: Opening Tag-------------------
    if(isOpeningTag){
        cout << "Found Opening Tag: " << line_in.substr(0, line_in.find('>')) << "\n";
        
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
            }else{
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

        scrapeDoc->insertDocumentTag(tagName, newHtmlTag);

    }else{
        //---------Case: Closing Tag---------------------
        line_in.erase(0,1); // Erase '/'
        cout << "Found Closing Tag: " << line_in.substr(0, line_in.find('>')) << "\n";
        // Found a closing tag

    }
}

// Assuming the line begins with a '<' 
void parseLine(HtmlDoc* scrapeDoc, stack<HtmlTag*> tagStack, string line_in){
    scrapeDoc->addLine();

    int end_whitesapce = line_in.find_first_not_of(" \t");
    line_in.erase(0,end_whitesapce);

    // Found the opening '<' for a tag
    if(end_whitesapce != -1 && line_in.at(0) == '<'){
        parseTag(scrapeDoc, line_in);
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
    stack<HtmlTag*> tagStack;
    string input;

    while(getline(myfile, input)){
        parseLine(myDocument, tagStack, input);
    }

    cout << "Document statistics of: "<< file_in << endl;
    cout << "Num of Lines: "<< myDocument->getLines()<<endl;
    cout << "Num of Tags: "<< myDocument->getDocTags().size()<<endl;
    delete myDocument;

    return 0;
}