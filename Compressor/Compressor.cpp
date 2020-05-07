#include <iostream>
#include <fstream>
#include <ctime> 
#include <map>
#include <unordered_map>
#include "Compressor.h"
#include <set>
#include <algorithm>
#include <unordered_set>


using namespace std;

int main(int argc, char *argv[]) 
{
    const int START_COMPARE = 6;
    const int COMPARES = 2;
    const int SHOW_FIRST = 100;
    const int MIN_HITS = 2;

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
                file.close();

                int totals[COMPARES];
                int totalTotals[COMPARES];
                unordered_map<Key, int> results[COMPARES];
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
                        else 
                        {
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
                    vector<pair<Key, int>> sorted;
                    for (pair<Key, int> entry : pattern)
                    {
                        if (entry.second >= MIN_HITS)
                        {
                            sorted.push_back(entry);
                        }
                    }

                    totals[i - START_COMPARE] = sorted.size();
                    int totalTotal = 0;
                    for (pair<Key, int> entry : sorted)
                    {
                        totalTotal += entry.second;
                    }
                    totalTotals[i - START_COMPARE] = totalTotal;
                    cout << "\r\nfound " << found << ", did not find " << notfound << ", actually found " << sorted.size() << " unique and " << totalTotal << " total " << endl;

                    sort(sorted.begin(), sorted.end(), 
                        [](pair<Key, int> a, pair<Key, int> b)
                        {
                            return a.second > b.second;
                        });
                    
                    if (sorted.size() > SHOW_FIRST)
                    {
                        sorted.resize(SHOW_FIRST);
                    }

                    reverse(sorted.begin(), sorted.end());

                    unordered_map<Key, int> p(sorted.begin(), sorted.end());
                    results[i - START_COMPARE] = p;
                }
                cout << "Finished after " << ((float)(clock() - time) / CLOCKS_PER_SEC) << " seconds" << endl;
                for (int i = 0; i < COMPARES; i++)
                {
                    for (pair<Key, int> pair : results[i])
                    {
                        cout << i + START_COMPARE << "=>" << pair.second << " ";

                        for (int x = 0; x < pair.first.size; x++)
                        {
                            printf("%x", pair.first.array[x] & 0xFF);
                            cout << " ";
                        }

                        for (int x = 0; x < pair.first.size; x++)
                        {
                            printf("%c", (pair.first.array[x] & 0xff));
                        }

                        cout << endl;
                    }
                }
                for (int i = 0; i < COMPARES; i++)
                {
                    cout << i + START_COMPARE << ": " << totals[i] << " unique, " << totalTotals[i] << " total" << endl;
                }
                fstream outfile;
                string name = filen.substr(filen.find_last_of("\\") + 1);
                string nameName = name.substr(0, name.find_last_of(".")) + ".compressed";
                outfile.open(nameName, ios::out);
                if (outfile)
                {
                    cout << "Created " << nameName << endl;
                    vector<pair<Key, int>> things;
                    unordered_set<Key> usedKeys[COMPARES];
                    for (int i = 0; i < size; i++)
                    {
                        Key match;
                        match.array = &f[i];
                        for (int x = 0; x < COMPARES; x++)
                        {
                            match.size = START_COMPARE + x;

                            
                            unordered_map<Key, int>::iterator it = results[x].find(match);
                            if (it != results[x].end())
                            {
                                things.push_back(make_pair(match, i));
                                usedKeys[x].insert(match);
                                break;
                            }
                        }
                        if (i % 1000 == 0)
                        {
                            cout << i << "/" << size << "\r";
                        }
                    }
                    
                    for (int i = 0; i < COMPARES; i++)
                    {
                        cout << i + START_COMPARE << "=>";
                        for (Key key : usedKeys[i])
                        {
                            cout << key.size << ", ";
                        }
                        cout << endl;
                    }
                    
                    cout << "Done" << endl;
                    file.close();
                }
                else
                {
                    cout << "Could not create " << nameName << endl;
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
