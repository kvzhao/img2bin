#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>

#include <time.h>
#include <stdlib.h>

#include "dsList.h"
#include "labelParser.h"

using namespace std;

const static int group_num = 5;

int main (int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: [ds_list] [#of base] [output_prefix]\n";
        return -1;
    }

    string list_name = string(argv[1]);
    int base_num = atoi(argv[2]); // assign
    srand(time(NULL));

    // Get directory name
    string outname = string(argv[3]);

    ifstream in(list_name);

    // Get name list from directory
    vector<string> nlist;

    // Parsing
    labelParser lp(nlist);
    lp.parse(MORPH);
    nlist = lp.getNamelist();
    vector<unsigned int> labels = lp.getAge();

    // Strategy here
    for (int i=0; i < group_num; i++ ) {
        cout << "num in " << i << " group " << lp.getSizeOfGroup(i) << endl;
    }

    vector<string>::iterator dsitr = nlist.begin();
    vector<unsigned int>::iterator lbitr = labels.begin();
    
    // Create the global map
    map<string, unsigned int> facebook; // yeh, it's a joke
    while(dsitr != nlist.end()) {
       facebook.insert(pair<string, unsigned>(*dsitr, *lbitr));
       ++dsitr; ++lbitr;
    }

    // Create 5 groups
    vector<vector<string> > groups;
    groups.reserve(group_num);

    dsitr = nlist.begin();
    vector<unsigned int> glabel = lp.getAgeGroup();
    lbitr = glabel.begin();

    while (dsitr != nlist.end() ) {
//       cout << *lbitr << " " << *dsitr << endl; 
       groups[*lbitr].push_back(*dsitr);
       ++dsitr; ++lbitr;
    }

    // Select baseline 
    vector<string> baseline;
    vector<string>::const_iterator it;
    for (size_t i=0; i != group_num; ++i) {
       it = groups[i].begin();
       int gsize;

       if (it == groups[i].end()) {
          cout << "Group " << i << " is an empty group\n";
          continue;
       } else {
          gsize = groups[i].size();
       }

       for (size_t j =0; j < base_num; ++j) {
          size_t baseIdx = rand() % gsize +1;
          cout << "Select #" << baseIdx  << " person who is " 
            << it[baseIdx] << endl; 
          baseline.push_back(it[baseIdx]);
       }
    }

    // Generate the list
    ofstream out(outname.c_str());

    // Same ID versus Different baseliner
    it = nlist.begin();
    while (it != nlist.end()) { 
         for (size_t i =0; i != baseline.size(); ++i) {
            if (*it != baseline[i]) {
                out << *it +".JPG" << "\t" << facebook[*it] << "\t"
                   << baseline[i] +".JPG" << "\t" << facebook[baseline[i]] << endl;
            }
         }
         ++it;
    }

    return 0;
}
