#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "dsList.h"
#include "labelParser.h"

using namespace std;

int main (int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: [dataset path] [output filename]\n";
        return -1;
    }

    string dspath = string(argv[1]);

    dsList ds(dspath);
    ds.readDir();

    vector<string> nlist = ds.getList();

    labelParser lp(nlist);
    lp.parse(MORPH);

    return 0;
}
