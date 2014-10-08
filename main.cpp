#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

#include "dsList.h"
#include "labelParser.h"

using namespace std;

int main (int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: [dataset path] [output filename]\n";
        return -1;
    }

    // Get directory name
    string dspath = string(argv[1]);
    dsList ds(dspath);
    ds.readDir();

    // Get name list from directory
    vector<string> nlist = ds.getList();

    // Parsing
    labelParser lp(nlist);
    lp.parse(MORPH);
    nlist = lp.getNamelist();
    vector<unsigned int> labels = lp.getAge();

    // Strategy here
    for (int i=0; i < 5; i++ ) {
        cout << "num in " << i << " group " << lp.getSizeOfGroup(i) << endl;
    }

    vector<string>::iterator dsitr = nlist.begin();
    map<string, unsigned int> facebook; // yeh, it's a joke
    while(dsitr != nlist.end()) {
        ++dsitr;
    }


    return 0;
}
