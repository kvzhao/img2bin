#ifndef __DSLIST_H__
#define __DSLIST_H__

#include <string>
#include <vector>
using namespace std;

/*
 * The function of this class is getting 
 * the dataset from path and capturing the file names
 * of the pictures. Only dealing with small amount of effort.
 */
class dsList {
    public:
        dsList(const string& abs_path);
        void readDir();

        // get function
        vector<string>& getList();
        inline int getNum() const {return num_;};
        inline string getDSName() const { return dsname_;};
        inline string getRoot() const { return root_;};
        inline string getPath() const { return path_; };

        // debug
        void displayList();

    private:
        string dsname_;
        string root_;
        string path_;
        vector<string> name_list_;
        unsigned int num_;
};
#endif
