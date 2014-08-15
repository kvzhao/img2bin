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

int saveLabel (ofstream& out, const int l) {
    if ( l <= 0 ) {
        return 0;
    }
    out.write((char*)&l, sizeof(int));
    return 1;
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

void MatProcessing(Mat &M) {
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
        if (dirp->d_name[0] != '.')
            files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

const double KVALID_RATIO = 0.8;
const char* image_train_filename= "morph-image-train.bin";
const char* label_train_filename= "morph-label-train.bin";
const char* image_test_filename = "morph-image-test.bin";
const char* label_test_filename = "morph-label-test.bin";

int main (int argc, char** argv) {
    if ( argc != 2 ) {
        cout << "Usage: [DIR_Path]\n";
        return -1;
     }

    char* dir_path = argv[1];
    char* outfilename = argv[2];

    vector<string> files;
    getdir(dir_path, files);

    int DS_SIZE = files.size();
    int DS_TRAIN = (int)(DS_SIZE * KVALID_RATIO);
    int DS_TEST  = DS_SIZE - DS_TRAIN;
    cout << "The Dir contains " << DS_SIZE << " visible files\n";
    cout << "Training : " << DS_TRAIN << " Testing: " << DS_TEST << "\n";

    ofstream train_image(image_train_filename, ios::out | ios::binary);
    ofstream train_label(label_train_filename, ios::out | ios::binary);
    ofstream test_image(image_test_filename, ios::out | ios::binary);
    ofstream test_label(label_test_filename, ios::out | ios::binary);

    for (int i = 0; i < DS_SIZE; i++) {
        static int counter = DS_SIZE /100;
        string img_name= files.at(i);
        int gender_pos = img_name.find_first_of("F");
        if (gender_pos == std::string::npos)
           gender_pos = img_name.find_first_of("M");
        // for most of people, age is 2-digit information
        string sAge = img_name.substr(gender_pos+1, 2);
        int age = std::stoi(sAge);
        // CV_LOAD_IMAGE_...
        Mat image = imread(string(dir_path) +img_name, CV_LOAD_IMAGE_GRAYSCALE);   // Read the file

        if (image.empty()) {
            cout << "something terrible happend\n";
            return -1;
        }
        // I MUST DO SOME IMG PROCESSING HERE!
//        MatProcessing(image);

        if (i < DS_TRAIN) {
            saveLabel(train_label, age);
            saveMat(train_image, image);
        } else {
            saveLabel(test_label, age);
            saveMat(test_image, image);
        }
        if ( i /counter ==0) {
            cout << ".";
        }
    }

    train_image.close();
    train_label.close();
    test_image.close();
    test_label.close();

    return 0;
}
