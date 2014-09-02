/*
 * This program displays the names of all files in the current directory.
 */

#include <dirent.h>
#include <stdio.h>

int main(int argc, char **argv) {
    if ( argc != 2) {
        return -1;
    }
  DIR *d;
  struct dirent *dir;
  static int counter =0;
  d = opendir(argv[1]);
  if (d) {
    while ((dir = readdir(d)) != NULL) {
        if ( dir->d_name[0] != '.') { 
            counter++;
            printf("%s\n", dir->d_name);
        }
    }
    printf("It constains %d items in total\n", counter);
    closedir(d);
  }

  return(0);
}
