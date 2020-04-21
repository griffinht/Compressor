#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) 
{
    if (argc > 0) 
    {
        cout << "Opening " << argv[argc - 1] << endl;
        ifstream open(argv[argc - 1]);
        if (open) 
        {
            string str = argv[argc - 1];
            cout << "Opened " << str.substr(str.find_last_of("\\") + 1) << endl;
        }
        else
        {
            cout << "Error opening " << argv[argc - 1] << endl;
        }
    }
    else 
    {
        cout << "Not enough arguments - please specify a file to open" << endl;
    }
}