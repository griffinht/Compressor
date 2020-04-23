#include <iostream>
#include <fstream>
#include <ctime> 
#include <map>
#include <unordered_map>
#include "Compressor.h"
#include <set>
#include <algorithm>


using namespace std;

int main(int argc, char *argv[]) 
{
    const int START_COMPARE = 15;
    const int COMPARES = 2;
    const int SHOW_FIRST = 100;

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

            char* f = new char [size];
            Key fKey;
  
            if (file.read(f, size))
            {
                cout << "Read in " << ((float)(clock() - time) / CLOCKS_PER_SEC * 1000) << "ms" << endl;

                int totals[COMPARES];
                vector<pair<Key, int>> results[COMPARES];
                for (int i = START_COMPARE; i < START_COMPARE + COMPARES; i++)
                {
                    int found = 0;
                    int notfound = 0;
                    unordered_map<Key, int> pattern;
                    fKey.size = i;
                    for (int x = 0; x < size - i;) 
                    {
                        fKey.array = &f[x];
                        unordered_map<Key, int>::iterator it = pattern.find(fKey);
                        if (it != pattern.end())
                        {
                            it->second++;
                            x += i;
                            found++;
                        }
                        else {
                            Key insert;
                            insert.array = &f[x];
                            insert.size = i;
                            pattern.insert({ insert, 1 });
                            notfound++;
                            x++;
                        }

                        if (x % 1000 == 0)
                        {
                            cout << "finding for " << i << " (" << (i - START_COMPARE + 1) << "/" << COMPARES << "): " << ((int)((((double)x / size) * 1000)) / 10.0) << "% - " << x << "/" << size << " - " << found << " found, " << notfound << " not found" << "\r";
                        }
                    }
                    vector<pair<Key, int>> sorted(pattern.begin(), pattern.end());

                    if (sorted.size() > SHOW_FIRST)
                    {
                        sorted.resize(SHOW_FIRST);
                    }

                    sort(sorted.begin(), sorted.end(), 
                        [](pair<Key, int> a, pair<Key, int> b)
                        {
                            return a.second < b.second;
                        });
                    
                    results[i - START_COMPARE] = sorted;
                    //delete &pattern; breaks things a lot
                    totals[i - START_COMPARE] = found;
                    cout << "\r\nfound " << found << ", did not find " << notfound << endl;
                }
                cout << "Finished after " << ((float)(clock() - time) / CLOCKS_PER_SEC) << " seconds" << endl;
                for (int i = 0; i < COMPARES; i++)
                {
                    for (pair<Key, int> pair : results[i])
                    {
                        cout << i + START_COMPARE << "=>" << pair.second << ": [";
                        for (int x = 0; x < pair.first.size; x++)
                        {
                            cout << (int)pair.first.array[x];
                            if (x != pair.first.size - 1)
                            {
                                cout << ",";
                            }
                        }
                        cout << "]" << endl;
                    }
                }
            }
            else
            {
                cout << "Error opening " << filen << endl;
                return 1;
            }

            delete[] f;
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
