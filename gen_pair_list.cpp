#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>

#include <time.h>
#include <stdlib.h>

using namespace std;

const int num_group = 8;
const int num_baseline = 2;
const int baseline_shuffle_times = 1;
const static double train_test_ratio = .8;

int main (int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: [ds_list] [#of base] [output_prefix]\n";
        return -1;
    }
    srand(time(NULL));

    string list_filename = string(argv[1]);
    int base_num = atoi(argv[2]); // assign
    string out_prefix = string(argv[3]);

    // Get directory name
    ifstream in(list_filename);

    // Get name list from the file
    vector<string> lines;
    string line;
    while (in >> line) {
       lines.push_back(line);
    }

    // Parsing
    vector<int> gender_list;
    vector<string> name_list = lines;
    vector<int> age_list;
	/* statistic information*/
    int num_male = 0, num_female = 0, num_total =0;
    // get format (including dot . )
    num_total = lines.size();
    vector<string>::const_iterator itr = lines.begin();
    for (; itr != lines.end(); ++itr) {
        int gender_pos = 0;
        // exclude format
        string cur_name = *itr;
	// cout << cur_name << endl;
        if ((gender_pos = cur_name.find_first_of("M")) != string::npos) {
           // Male
           num_male++;
           gender_list.push_back(1);
        } else 
        if ((gender_pos = cur_name.find_first_of("F")) != string::npos) {
          // Female
          num_female++;
          gender_list.push_back(0);
        } else {
        //      cout << " format error! \n";
        }
        // age length must be 2
         stringstream sAge(cur_name.substr(gender_pos +1, 2));
         int age; 
         sAge >> age;
         age_list.push_back(age);   
    }
    int max_age = *std::max_element(age_list.begin(), age_list.end());
    int min_age = *std::min_element(age_list.begin(), age_list.end());
    double class_range = (max_age-min_age)/num_group;

    cout << "Total: " << num_total << " with " << num_female << " females and " 
	<< num_male << " males.\nMax age: " << max_age << " min age: " << min_age 
	<< " set class range as: " << class_range
	<< endl;

    // Forming a map
    vector<string>::iterator iname = name_list.begin();
    vector<int>::iterator ilabel = age_list.begin();

    // Create the global map
    map<string, int> dsMap;
    while(iname != name_list.end() && ilabel != age_list.end()) {
       dsMap.insert(pair<string, int>(*(iname++), *(ilabel++)));
    }

    // Create N groups contains M_i data
    vector<vector<string> > name_in_groups;
    vector<vector<string> > baselines;
    vector<int> size_of_each_group(num_group, 0);
    name_in_groups.reserve(num_group);
   baselines.reserve(num_group);

    for(std::vector<string>::iterator it = name_list.begin(); it != name_list.end(); ++it) { 
          int age = dsMap[*it];
       for(int ng =0; ng != num_group; ++ng) {
          int lw_bound = min_age + ng * class_range;
          int up_bound = min_age + (1+ng) * class_range;
          if (age >= lw_bound && age <= up_bound) {
             name_in_groups[ng].push_back(*it);
             size_of_each_group[ng]++;
          }
       }
    }

    // group information
    for (int i=0; i !=num_group; ++i) {
       int lw_bound = min_age + i * class_range;
       int up_bound = min_age + (1+i) * class_range;
       cout << "Group size: " << size_of_each_group[i] << endl;
       cout << "[ " << lw_bound << " ,"  << up_bound << " ]\n" << endl;
    }

    int num_training_data = num_total * train_test_ratio;
    int num_testing_data  = num_total - num_training_data;

    // writh group list
    string class_test_list = out_prefix + "-class_list_test_sequence";
    string class_train_list = out_prefix + "-class_list_train_sequence";
    ofstream class_test(class_test_list.c_str());
    ofstream class_train(class_train_list.c_str());
/*
    map<string, int> class_map;
    for (int i=0; i < num_group; ++i) {
       for(int num=0; num < name_in_groups[i].size(); ++num) {
          if (i > 4)
            class_map.insert(pair<string, int>(name_in_groups[i][num], 5));
          else
            class_map.insert(pair<string, int>(name_in_groups[i][num], i));
       }
    }
    */

    for (int i=0; i < num_group; ++i) {
       for(int num=0; num < name_in_groups[i].size(); ++num) {
          if (num < name_in_groups[i].size() * train_test_ratio) {
             class_train << name_in_groups[i][num] << "\t" << ((i>4)?5:i) << "\n";
          } else {
             class_test << name_in_groups[i][num] << "\t" << ((i>4)?5:i) << "\n";
          }
       }
    }
    
   // close
    class_train.close();
    class_test.close();

    // Select Baselines
    for (int i=0; i < num_group; ++i) {
       for (int b=0; b < num_baseline; ++b) {
          int index = rand() % size_of_each_group[i];
          if (index < size_of_each_group[i]) {
             string base_name = name_in_groups[i][index];
             baselines[i].push_back(base_name);
             cout << "Baseline in " << i << " is " << base_name << endl;
          }
       }
    }

    cout << "traing: " << num_training_data << "\ttesting: " << num_testing_data << endl;

    string test_list = out_prefix + "pair_list_test";
    string train_list = out_prefix + "pair_list_train";
    ofstream test(test_list.c_str());
    ofstream train(train_list.c_str());

    // shuffle namelist
    vector<string> candidates;
/*
    for(int i=0; i < num_total; ++i) {
       if(i < num_training_data) {
          train << 
       } else {
       }
    }
    */

    train.close(); test.close();
    return 0;
}
