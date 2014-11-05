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
  vector<string>::iterator it = lines.begin();

  out.open(out_name.c_str(), ios::out);
  for (;it != lines.end(); ++it) {
     out << *it << "\n";
  }
  out.close();
  return(0);
}
