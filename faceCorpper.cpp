// linux
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
// opencv
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"

// cpp standard lib
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;
using namespace cv;

/** Global variables */
String face_cascade_name = "lbpcascade_frontalface.xml";
CascadeClassifier face_cascade;
String window_name = "Capture - Face detection";
vector<int> compression_parameters;

void detectFace(const Mat &frame, Mat& face);
void saveImage(const Mat& img, string name);
int saveMat (ofstream& out, const Mat& M);
int readMat( const string& filename, Mat& M);
int getdir (string dir, vector<string> &files);


int main (int argc, char** argv) {
    if ( argc != 3 ) {
        cout << "Usage: [DIR_Path] [output-prefix]\n";
        return -1;
     }
    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return -1; };

    string dir_path = string(argv[1]);
    string out_path = string(argv[2]);

    vector<string> files;
    getdir(dir_path, files);

    int ds_size = files.size();

    cout << "The Dir contains " << ds_size << " visible files\n";
    compression_parameters.push_back(IMWRITE_PNG_COMPRESSION);
    compression_parameters.push_back(9);

    for (int i = 0; i < ds_size; i++) {
        string img_name= files.at(i);

        // Extract face from the frame
        Mat image = imread(dir_path +img_name, IMREAD_GRAYSCALE);   // Read the file
        if (image.empty()) {
            cout << "something terrible happend when " << img_name << " called\n";
            return -1;
        }
	
        // Processing
	    Mat face;
        detectFace(image, face);

        // Write the data to bin
        saveImage(face, out_path + img_name);

    } // end of for loop

    return 0;
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

void detectFace (const Mat& frame, Mat& face) {
    std::vector<Rect> faces_rect;
    Mat frame_gray;
    frame.copyTo(frame_gray);

    //equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces_rect, 1.1, 2, 0, Size(100, 100) );

    for( size_t i = 0; i < faces_rect.size(); i++) {
        Mat faceROI = frame_gray( faces_rect[i] );
        cout << "Face rect size: " << faces_rect[i] << "\n";
        //-- Draw the face
        int x = faces_rect[i].x -10;
        if ( x <= 0) x = faces_rect[i].x;
        int y = faces_rect[i].y -50;
        if ( y <= 0) y = faces_rect[i].y;
        int w = faces_rect[i].width + 10;
        if ( (x+w) > frame_gray.cols) w = faces_rect[i].width;
        int h = faces_rect[i].height +60;
        if ( (y+h) > frame_gray.rows) h = faces_rect[i].height;

        Rect face_hair(x, y, w, h);
        Rect face_wo_hair(faces_rect[i].x, faces_rect[i].y, faces_rect[i].width, faces_rect[i].height);

        frame_gray(face_wo_hair).copyTo(face);
        resize(face, face, Size(100, 120));
    }
    //-- Show what you got
}

int saveMat (ofstream& out, const Mat& M) {
    if (M.empty()) {
        return 0;
    }
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
    return 1;
}

int getdir (string dir, vector<string> &files) {
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        if (dirp->d_name[0] != '.')
            files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

void saveImage(const Mat &img, string name) {
    if (!img.empty())
        imwrite(name, img, compression_parameters);
}

