#include "dsList.h"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

dsList::dsList(const string & abs_path) {
    int pos = abs_path.find_last_of("/");
    num_ = 0;
    path_ = abs_path;
    root_ = abs_path.substr(0, pos+1);
    dsname_ = abs_path.substr(pos+1);
}

void dsList::readDir() {
    if (!path_.empty()) {
	    DIR *dp;
	    struct dirent *dirp;
	 if((dp  = opendir(path_.c_str())) == NULL) {
		cout << "Error(" << errno << ") opening " << path_ << endl;
	 }

	    while ((dirp = readdir(dp)) != NULL) {
		if (dirp->d_name[0] != '.') {
            num_ ++;
		    name_list_.push_back(string(dirp->d_name));
        }
	    }
	    closedir(dp);
    }
}

vector<string>& dsList::getList() {
    if (!path_.empty()) {
        if (name_list_.empty()) {
            readDir();
        }
        return name_list_;
    }
}

void dsList::displayList() {
    vector<string>::iterator itr = name_list_.begin();
    for (itr; itr != name_list_.end(); ++itr) {
        cout << *itr << "\n";
    }
}

