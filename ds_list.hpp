#ifndef __DS_LIST_H__
#define __DS_LIST_H__

class dsList {
    public:
        dsList(string path);
        void readDir(string path);
        vector<string>& getList();
        void saveList(string path);
    private:
        string path_;
        vector<string> name_list_;
        unsigned int num_;
        unsigned int male_, female_;
        
};
#endif
