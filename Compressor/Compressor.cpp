#include <iostream>
#include <fstream>
#include <ctime> 
#include <map>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) 
{
    const int START_COMPARE = 2;
    const int COMPARES = 2;

    if (argc > 0) 
    {
        cout << "Opening " << argv[argc - 1] << endl;
        ifstream file(argv[argc - 1], ios::binary | ios::ate | ios::in);
        if (file) 
        {
            string filen = argv[argc - 1];
            cout << "Opened " << filen.substr(filen.find_last_of("\\") + 1) << endl;
            clock_t time = clock();
            streamsize size = file.tellg();
            file.seekg(0, ios::beg);

            char *f = new char[size];//mem leak?
  
            if (file.read(f, size))
            {
                cout << "Read in " << ((float)(clock() - time) / CLOCKS_PER_SEC * 1000) << "ms" << endl;

                int totals[COMPARES];
                char **resultsByte[COMPARES];
                int resultsInt[COMPARES];
                for (int i = START_COMPARE; i < START_COMPARE + COMPARES; i++)
                {
                    int foundAmt = 0;
                    int notfound = 0;
                    vector<pair<char*, int>> pattern;
                    for (int x = 0; x < size - i;) 
                    {
                        /*
                        map<char*, int>::iterator it = pattern.find(b);
                        if (it != pattern.end())
                        {
                            cout << (int)it->first << "==" << (int)b << endl;
                            it->second = it->second + 1;
                            x += i;
                            found++;
                        }
                        else
                        {
                            cout << "inserting" << endl;
                            pattern.insert({ b, 1 });
                            notfound++;
                            x++;
                        }
                        */
                        /**/
                        bool found = false;
                        for (pair<char*, int> p : pattern)
                        {
                            int ret = 1;
                            //cout << (int)*p.first << "," << p.second << " (" << pattern.size() << ")";
                            //cout << "match " << (int)f[x] << " with";
                            for (int y = 0; y < i; y++) {
                                //cout << "comparing " << y << ": " << (int)p.first[y] << " to " << (int)f[x + y] << endl;
                                if (p.first[y] != f[x + y])
                                {
                                    ret = 0;
                                    break;
                                }
                            }
                            //cout << "   ";
                            if (ret)
                            {
                                //cout << "- is good" << endl;
                                found = true;
                                p.second = p.second + 1;
                                x += i;
                                foundAmt++;
                                break;
                            }
                        }
                        if (!found)
                        {
                            //cout << "not found, inserting" << b << "()" << (int)*b << endl;
                            //cout << (int)f[x] << endl;
                            pattern.push_back({ &f[x], 1 });
                            notfound++;
                            x++;
                        }
                        else
                        {
                            //cout << "found" << endl;
                        }
                        if (x % 1000 < 2)
                        {
                            cout << "finding for " << i << " (" << (i - START_COMPARE + 1) << "/" << COMPARES << "): " << ((int)((((double)x / size) * 1000)) / 10.0) << "% - " << x << "/" << size << " - " << foundAmt << " found, " << notfound << "not found" << "\r";
                        }
                    }
                    cout << "\r\nfound " << foundAmt << ", did not find " << notfound << endl;
                }
                cout << "all done" << endl;
            }
            else
            {
                cout << "Error opening " << filen << endl;
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
