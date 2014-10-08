#include "labelParser.h"

labelParser::labelParser(vector<string> &list) {
    nMale_ = 0; nFemale_ =0;

    vector<string>::const_iterator itr = list.begin();

    if (itr != list.end()){ 
        // if the list is not null, allocate the namelist
        nameList.reserve(list.size());

        // find format
        int dot = itr->find_first_of(".");
        format_ = (*itr).substr(dot);

        // exclude format in name
        while(itr != list.end()) {
            nameList.push_back(itr->substr(0, itr->find_first_of(format_)));
            ++itr;
        }
    }


    for (int i =0; i < ngroup; i++) {
        numOfGroup_[i] = 0;
    }
}

/*
 * MORPH FORMAT: ID_d + Gender + Age
 * 023674_1M63.JPG
 * 019066_08M70.JPG
 * 019066_7M70.JPG
 * 019066_5M69.JPG
 */

unsigned int labelParser::groupConverter(unsigned int age) {
    unsigned int label;
    if (age >= 0 && age < 20) {
        label = 0;
    } else
    if (age >= 20 && age < 30) {
        label = 1;
    } else
    if (age >= 30 && age < 40) {
        label = 2;
    } else
    if (age >= 40 && age < 50) {
        label = 3;
    } else
    if (age >= 50 && age < 90) {
        label = 4;
    } 

    // prior distribution
    numOfGroup_[label]++;
    return label;
}

bool labelParser::parse(DS_NAME name) {
    switch (name) {
        case MORPH: {
            vector<string>::const_iterator itr = nameList.begin();
            // get format (including dot . )
            for (; itr != nameList.end(); ++itr) {
                int gender_pos = 0;
                // exclude format
                string cur_name = *itr;
                //DEBUG cout << cur_name << endl;
                if ((gender_pos = cur_name.find_first_of("M")) != string::npos) {
                    // Male
                    nMale_++;
                    vGender_.push_back(1);
                } else 
                if ((gender_pos = cur_name.find_first_of("F")) != string::npos) {
                    // Female
                    nFemale_++;
                    vGender_.push_back(0);
                } else {
                    cout << " format error! \n";
                }
                // age length must be 2
                stringstream sAge(cur_name.substr(gender_pos +1, 2));
                unsigned int age; 
                sAge >> age;
                vAgeValue_.push_back(age);
                vAgeGroup_.push_back(groupConverter(age));
            }
         } // end of morph
            break;
        default:
            cout << "Not support this dataset\n";
    }
}

void labelParser::displayInfo() {
}
