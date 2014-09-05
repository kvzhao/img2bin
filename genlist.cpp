/*
 * The program dump the dir files name to a list
 * and the gender label is following
 */
#include <dirent.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

enum dataset {CAS, HK};

int label_filter(dataset ds, string filename) {
    int label =-1;
    switch (ds) {
        case CAS:
            if (filename[0] == 'M') label =1;
            if (filename[0] == 'F') label =0;
      break;
        case HK:
      break;

        default:
            break;
    }
    return label;
}

int main(int argc, char **argv) {
    if ( argc != 3) {
        cout << "[Usage] RootDir list_name.txt\n";
        return -1;
    }
  DIR *d;
  struct dirent *dir;
  static int counter =0;

  string out_name = string(argv[2]);
  ofstream out;

  out.open(out_name.c_str());

  d = opendir(argv[1]);
  if (d || out.is_open()) {
    while ((dir = readdir(d)) != NULL) {
        if ( dir->d_name[0] != '.') { 
            string img_name = dir->d_name;
            counter++;
            out << img_name << " " << label_filter(CAS, img_name) << endl;
            cout <<  img_name << "\n";
        }
    }
    printf("It constains %d items in total\n", counter);
    closedir(d);
  }

  return(0);
}
