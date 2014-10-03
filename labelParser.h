#ifndef __LABELPARSER_H__
#define __LABELPARSER_H__

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "dsList.h"

using namespace std;

enum DS_NAME {MORPH, CAS};
enum LB_TYPE {AGE, GENDER};

/* 
 * The parser class is defined extracting labels from filename
 * and calculate the total number of datum, saving statistical information.
 */

class labelParser {
    public: 
        labelParser(vector<string> &list) ;
        bool parse(DS_NAME);

        void displayInfo();
        void saveInfo(LB_TYPE, string path);

        vector<unsigned int> getAge() const {return vAgeValue_;};
        vector<unsigned int> getGender() const {return vGender_;};
        int getSizeOfGroup(int idx) const { if( idx >=0 && idx < ngroup) return numOfGroup_[idx];};
    protected:
        unsigned int groupConverter(unsigned int);
    private:
        vector<string> nameList;
        // number information
        unsigned int num_, nMale_, nFemale_;

        static const unsigned int ngroup = 5;

        // Labels
        vector<unsigned int> vAgeValue_;
        vector<unsigned int> vAgeGroup_;
        unsigned int numOfGroup_[ngroup];
        vector<unsigned int> vGender_;

};
#endif
