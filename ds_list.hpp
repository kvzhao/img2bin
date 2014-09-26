#ifndef __DS_LIST_H__
#define __DS_LIST_H__

class dsList {
    public:
        dsList();
        void readDir(string path);
        vector<string>& getList();
        void saveList(string path);
    private:
        vector<string> name_list;
        int num;
};
#endif
