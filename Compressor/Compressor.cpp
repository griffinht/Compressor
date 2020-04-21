// Compressor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
