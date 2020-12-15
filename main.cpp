#include <iostream>
#include <exception>
#include <string>
#include <map>
#include <stack>
#include <list>
#include <fstream>
#include "HtmlDoc.hpp"
#include "HtmlTag.hpp"
#include "mainMenu.hpp"
#include "functions.hpp"
using namespace std;



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

    Functions myFunctions;
    HtmlDoc* myDocument = new HtmlDoc();
    stack<HtmlTag*>* tagStack = new stack<HtmlTag*>();
    MainMenu m;
    m.insertOption(1,"hello");
    m.insertOption(1, "View number of lines total");
    m.insertOption(2, "View number of tags in document");
    m.insertOption(3, "View all tags with information");
    m.insertOption(4, "View specific element detail");
    m.insertOption(5, "View specific attribute value");
    m.insertOption(6, "Exit the program");
    string input;

    while(getline(myfile, input)){
        myFunctions.parseLine(myDocument, tagStack, input);
    }
    myfile.close();


    /*
    ---------Interactive Menu Choices-----------------
    TODO: Select a specific tag and view details of it
    */
    int menuChoice = 0;
    while(menuChoice != 6){
        m.display();
        m.promptUser();
        cin.clear();
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
                list<string> inner_content = itr->second->getInnerContent();
                cout << "Inner content: \n";
                for(auto v: inner_content) cout << "-->"<< v << endl;
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
            try{
                myFunctions.processTagRequest(myDocument, find_attr);
            } catch(std::exception& e){
                std::cout << "Exception: " << e.what() << std::endl; 
            }
            cin.clear();

        }else if(menuChoice == 6){
            cout << "Goodbye\n";
        }
        cin.ignore();
    }

    delete myDocument;
    delete tagStack;
    return 0;
}