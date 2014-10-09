#ifndef __LABELPARSER_H__
#define __LABELPARSER_H__

#include <string>
#include <vector>
#include <algorithm>
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
        labelParser();
        labelParser(vector<string> &list) ;
        bool parse(DS_NAME);

        void readFromFile(string name);
        void displayInfo();
        void saveInfo(LB_TYPE, string path);

        // Get functions
        vector<unsigned int> getAge() const {return vAgeValue_;};
        vector<unsigned int> getAgeGroup() const {return vAgeGroup_;};
        vector<unsigned int> getGender() const {return vGender_;};
        vector<string> getNamelist() const {return nameList;};
        inline unsigned int getMinAge() const 
        {vector<unsigned int>::const_iterator it = std::min_element(vAgeValue_.begin(), vAgeValue_.end());};
        inline unsigned int getMaxAge() const 
        {vector<unsigned int>::const_iterator it = std::max_element(vAgeValue_.begin(), vAgeValue_.end());};
        int getSizeOfGroup(int idx) const { if( idx >=0 && idx < ngroup) { return numOfGroup_[idx];} };
    protected:
        unsigned int groupConverter(unsigned int);
    private:
        string format_;
        vector<string> nameList;
        // number information
        unsigned int num_, nMale_, nFemale_;
        static const unsigned int ngroup = 5;

        // Labels
        // Label in age value
        vector<unsigned int> vAgeValue_;
        // Label in group number
        vector<unsigned int> vAgeGroup_;
        // Record num of people in each group
        unsigned int numOfGroup_[ngroup];

        // Binay Label in gender
        vector<unsigned int> vGender_;

};
#endif
