

#ifndef MAINMENU_HPP
#define MAINMENU_HPP
#include <string>
#include <map>
using namespace std;

class MainMenu{
    public:
        MainMenu(){
        };

        bool insertOption(int num, string op)
        {
            menuOptions.insert({num, op});
            return true;
        };
        void promptUser()
        {
            cout << "Select your choice: ";
        };
        void display()
        {
            map<int, string>::iterator it;
            for ( it = menuOptions.begin(); it != menuOptions.end(); it++ )
            {
                std::cout << it->first << ": " << it->second  << std::endl ;
            }
        };

    private:
        multimap<int, string> menuOptions;

};
#endif