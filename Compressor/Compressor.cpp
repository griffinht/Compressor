#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) 
{
    int START_COMPARES = 2;
    int COMPARES = 2;

    if (argc > 0) 
    {
        cout << "Opening " << argv[argc - 1] << endl;
        ifstream file(argv[argc - 1], ios::binary | ios::ate);
        if (file) 
        {
            string filen = argv[argc - 1];
            cout << "Opened " << filen.substr(filen.find_last_of("\\") + 1) << endl;
            clock_t time = clock();
            streamsize size = file.tellg();
            file.seekg(0, ios::beg);

            char* f = new char[size];
            if (file.read(f, size))
            {
                cout << "Read in " << ((float)(clock() - time) / CLOCKS_PER_SEC * 1000) << "ms" << endl;

            }
            else
            {
                cout << "Error opening " << filen;
                return 1;
            }
        }
        else
        {
            cout << "Error opening " << argv[argc - 1] << endl;
            return 1;
        }
    }
    else 
    {
        cout << "Not enough arguments - please specify a file to open" << endl;
    }

    return 0;
}