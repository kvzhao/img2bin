// linux
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
// opencv
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
// cpp standard lib
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;
using namespace cv;

int saveMat (const string& filename, const Mat& M) {
    if (M.empty()) {
        return 0;
    }
    ofstream out(filename.c_str(), ios::out | ios::binary);
    // get Matrix specs
    int cols = M.cols;
    int rows = M.rows;
    int chan = M.channels();
    int eSiz = (M.dataend-M.datastart) / (cols*rows*chan);
    // write header
    out.write((char*)&cols, sizeof(cols));
    out.write((char*)&rows, sizeof(rows));
    out.write((char*)&chan, sizeof(chan));
    out.write((char*)&eSiz, sizeof(eSiz));
    // write data
    if (M.isContinuous()) {
        out.write((char*)M.data, cols * rows * chan * eSiz);
    } else {
        return 0;
    }
    out.close();
    return 1;
}

int readMat( const string& filename, Mat& M){
    ifstream in(filename.c_str(), ios::in|ios::binary);
    if (!in) {
       //M = NULL_MATRIX;
       return 0;
    }
    int cols;
    int rows;
    int chan;
    int eSiz;

    // Read header
    in.read((char*)&cols,sizeof(cols));
    in.read((char*)&rows,sizeof(rows));
    in.read((char*)&chan,sizeof(chan));
    in.read((char*)&eSiz,sizeof(eSiz));

    // Determine type of the matrix
    int type = 0;
    switch (eSiz){
    case sizeof(char):
         type = CV_8UC(chan);
         break;
    case sizeof(float):
         type = CV_32FC(chan);
         break;
    case sizeof(double):
         type = CV_64FC(chan);
         break;
    }

    // Alocate Matrix.
    M = Mat(rows,cols,type,Scalar(1));

    // Read data.
    if (M.isContinuous()){
       in.read((char *)M.data,cols*rows*chan*eSiz);
    }
    else{
       return 0;
    }
    in.close();
    return 1;
}

int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        if (dirp->d_name != "." && dirp->d_name != "..")
            files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}


 int main (int argc, char** argv) {
    if ( argc != 2 ) {
        cout << "Usage: ImageToLoad\n";
        return -1;
     }

    vector<string> files;
    getdir(".", files);

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(image.empty()) {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    saveMat("kitty.bin", image);

    namedWindow("Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                   // Show our image inside it.

    for (unsigned int i = 0;i < files.size();i++) {
        cout << files[i] << endl;
    }

    waitKey(0);
    return 0;
}
