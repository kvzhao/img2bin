// linux
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
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

static int male_num = 0;
static int female_num = 0;

int saveLabel (ofstream& out, const int* l) {
    if ( *l < 0 ) {
        return 0;
    }
    //cout << *l << " = label int\n";
    //snprintf(c, sizeof(c), "%d", l);
    //cout << atoi(c) << " label\n";
    out.write((char*)l, sizeof(int));
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
    double scale = .075;
    Size dSiz(32, 32);
    //Size dSiz(M.cols * scale, M.rows * scale);
    resize(M, M, dSiz);
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

string age_filter(string s) {
   int gender_pos = s.find_first_of("F");
   if (gender_pos == std::string::npos)
           gender_pos = s.find_first_of("M");
   // for most of people, age is 2-digit information
   string sAge = s.substr(gender_pos+1, 2);
   return sAge;
}

string gender_filter (string s) {
    string gender;
    
    // CAS
    if( s[0] == 'M' ) {
        gender = "1";
        male_num++;
    }
    if( s[0] == 'F' ) {
        female_num++;
        gender = "0";
    }
   return gender;
}

const char* image_filename= "image.bin";
const char* label_filename= "label.bin";

int main (int argc, char** argv) {
    if ( argc != 5 ) {
        cout << "Usage: [DIR_Path] [start_id] [end_id] [output-prefix]\n";
        return -1;
     }

    char* dir_path = argv[1];
    string outprefix= string(argv[4]);
    int start_ind = atoi(argv[2]);
    int end_ind = atoi(argv[3]);

    vector<string> files;
    getdir(dir_path, files);

    int ds_size = files.size();

    if (start_ind < 0) return -1;
    if (end_ind <  0) 
        return -1;
    else if(end_ind ==0)
        end_ind = ds_size-1;

    const int data_num = end_ind- start_ind +1;

    cout << "The Dir contains " << ds_size << " visible files\n";
    cout << "  convert item #: " << start_ind << " to " << end_ind << ", "
        << data_num << " items in total\n";

    string image_filename = outprefix + "-image.bin";
    string label_filename = outprefix + "-label.bin";

    ofstream img_file(image_filename.c_str(), ios::out | ios::binary);
    ofstream lab_file(label_filename.c_str(), ios::out | ios::binary);

    for (int i = start_ind; i < end_ind +1; i++) {
        string img_name= files.at(i);

        // Extract age or gender from the image filename
        string str_age = age_filter(img_name);
        string str_gender = gender_filter(img_name);

        int label = std::stoi(str_gender);
        //cout << " -> age: " << age << "\n";
        cout << " -> gender: " << label << "\n";
        
        // CV_LOAD_IMAGE_...
        Mat image = imread(string(dir_path) +img_name, IMREAD_GRAYSCALE);   // Read the file

        if (image.empty()) {
            cout << "something terrible happend when " << img_name << " called\n";
            return -1;
        }
        // I MUST DO SOME IMG PROCESSING HERE!
        MatProcessing(image);

        // Write the data to bin
        saveMat(img_file, image);
        saveLabel(lab_file, &label);

    } // end of for loop

    cout << "Dataset contains " << male_num 
        << " males and " << female_num 
        << " females. \n";

    img_file.close();
    lab_file.close();

    return 0;
}
