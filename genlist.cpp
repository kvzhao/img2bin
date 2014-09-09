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

using namespace std;

enum dataset {CAS, HK, FD};
static int male_counter = 0, female_counter = 0;

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
        default:
            break;
    }
    return label;
}

int main(int argc, char **argv) {
    if ( argc != 4) {
        cout << "[Usage] RootDir dataset_name list_name.txt\n" 
            << "   1: CAS set\n"
            << "   2: HK  set\n"
            << "   3: FD  set\n";
        return -1;
    }
  DIR *d;
  struct dirent *dir;
  static int counter =0;
  
  int ds_flag = atoi(argv[2]);

  string out_name = string(argv[3]);
  ofstream out;

  out.open(out_name.c_str());

  d = opendir(argv[1]);
  if (d || out.is_open()) {
    while ((dir = readdir(d)) != NULL) {
        if ( dir->d_name[0] != '.') { 
            string img_name = dir->d_name;
            counter++;
            switch(ds_flag) {
                case 1:
                    out << img_name << " " << label_filter(CAS, img_name) << endl;
                break;
                case 2:
                    out << img_name << " " << label_filter(HK, img_name) << endl;
                break;
                case 3:
                    out << img_name << " " << label_filter(FD, img_name) << endl;
                break;
            }

            cout <<  img_name << "\n";
        }
    }
    printf("It constains %d items in total\n  -- males: %d, females: %d\n", counter, male_counter
            , female_counter);
    closedir(d);
  }

  return(0);
}
