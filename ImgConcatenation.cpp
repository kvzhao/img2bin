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

// cpp standard lib
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;
using namespace cv;

/** Global variables */
vector<int> compression_parameters;

enum dataset {CAS, HK, FD, MORPH_AGE};
void detectFace(const Mat &frame, Mat& face);
void saveImage(const Mat& img, string name);
int saveMat (ofstream& out, const Mat& M);
int readMat( const string& filename, Mat& M);
int getdir (string dir, vector<string> &files);
int label_filter(dataset ds, string filename);

int rSiz_w =0;
int rSiz_h =0;
bool with_hair = false;

int main (int argc, char** argv) {
    if ( argc != 3 ) {
        cout << "Usage: [DIR_Path] [Name List]\n";
        return -1;
     }

    srand(time(NULL));

    string dir_path = string(argv[1]);
    string out_dir = "fused_set/";
    string filename = string(argv[2]);

    ifstream namelist(filename.c_str(), ios::in);
    ofstream out("fusedList", ios::out);

    vector<string> image_list;
    vector<int>    label_list;

    // Read each line and divide into img and label
    if (namelist.is_open()) {
        string line;
        while (getline(namelist, line)){ 
            static unsigned int i =0;
            unsigned int pos =  line.find(' ', 0);
            image_list.push_back(line.substr(0, pos));
            label_list.push_back(stoi(line.substr(pos, line.size()) ));
        }
    }
    
    const int klist_size = image_list.size();
    const int base_num = 5;
    
    for (size_t j= 0; j < base_num; ++j) {
        int base_index = rand() % klist_size;
        cout << "Randomly pick : " << image_list[base_index] << " as our baseline\n";
        int base_label = label_list[base_index];

        for (int i =0; i < klist_size; ++i) {
            Mat image = imread(dir_path+image_list[i], IMREAD_GRAYSCALE);   // Read the file
            if (image.empty()) { cout << "the image in " << dir_path + image_list[i] << " is empty\n"; }
            Mat base = imread(dir_path+image_list[base_index], IMREAD_GRAYSCALE);   // Read the file
            if (base.empty()) { cout << "the baseline is empty\n"; }
            Mat H;
            hconcat(image, base, H);
            string out_path = out_dir + to_string(j) + image_list[i];
            saveImage(H, out_path);
            int newlabel = (label_list[i] < base_label) ? 1 :0;
            out << to_string(j) + image_list[i] << " " << newlabel << endl;
        }
    }

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
    //face_cascade.detectMultiScale( frame_gray, faces_rect, 1.1, 2, 0, Size(100, 100) );

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

        // including hair
        Rect face_hair(x, y, w, h);
        // no hear, pure face
        Rect face_wo_hair(faces_rect[i].x, faces_rect[i].y, faces_rect[i].width, faces_rect[i].height);

        if (with_hair) {
            frame_gray(face_hair).copyTo(face);
        } else {
            frame_gray(face_wo_hair).copyTo(face);
        }
        resize(face, face, Size(rSiz_w, rSiz_h));
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
    compression_parameters.push_back(IMWRITE_PNG_COMPRESSION);
    compression_parameters.push_back(9);
    if (!img.empty())
        imwrite(name, img, compression_parameters);
}

int label_filter(dataset ds, string filename) {
    int label =-1;
    switch (ds) {
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

        default:
            break;
    }
    return label;
}

