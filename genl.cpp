/*
 * The program dump the dir files name to a list
 * and the gender label is following
 */
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "dsList.h"

using namespace std;

enum dataset {CAS, HK, FD, MORPH_AGE, MORPH_AGE_GROUP};
static int male_counter = 0, female_counter = 0;

int groupConverter(int age) {
     int label;
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
     return label;
}
                                  
int label_filter(dataset ds, string filename) {
    int label =-1;
    switch (ds) {
        case CAS:
            if (filename[0] == 'M') { 
                label =1; male_counter++;
            }
            if (filename[0] == 'F') {
                label =0; female_counter++;
            }
      break;
        case HK:
            if (filename[0] == 'M') { 
                label =1; male_counter++;
            }
            if (filename[0] == 'F') {
                label =0; female_counter++;
            }
      break;
        case FD:
            if (filename[0] == 'm') { 
                label =1; male_counter++;
            }
            if (filename[0] == 'w') {
                label =0; female_counter++;
            }
      break;

        case MORPH_AGE: {
            int gender_pos = filename.find_first_of("M");
            if (gender_pos == string::npos) 
                gender_pos = filename.find_first_of("F");
            if (gender_pos == string::npos) 
                cout << "error occurs\n";
            string sAge = filename.substr(gender_pos +1, 2);
           stringstream(sAge) >> label; 
        }
        break;

        case MORPH_AGE_GROUP: {
            int gender_pos = filename.find_first_of("M");
            if (gender_pos == string::npos) 
                gender_pos = filename.find_first_of("F");
            if (gender_pos == string::npos) 
                cout << "error occurs\n";
            string sAge = filename.substr(gender_pos +1, 2);
           stringstream(sAge) >> label; 
           label = groupConverter(label);
        }
        break;

        default:
            break;
    }
    return label;
}

int main(int argc, char **argv) {
    if ( argc != 3) {
        cout << "[Usage] RootDir list_name\n";
        return -1;
    }
  DIR *d;
  struct dirent *dir;
  static int counter =0;
  
  string ds_path  = string(argv[1]);
  string out_name = string(argv[2]);
  ofstream out;

  dsList ds(ds_path);
  ds.readDir();
  vector<string> lines = ds.getList();

  out.open(out_name.c_str(), ios::out);
  out.close();
  return(0);
}
