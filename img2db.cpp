// Copyright 2014 BVLC and contributors.
//
// This script converts the MNIST dataset to the leveldb format used
// by caffe to perform classification.
// Usage:
//    convert_mnist_data input_image_file input_label_file output_db_file
// The MNIST dataset could be downloaded at
//    http://yann.lecun.com/exdb/mnist/

#include <google/protobuf/text_format.h>
#include <glog/logging.h>
#include <leveldb/db.h>

#include <stdint.h>
#include <fstream>  // NOLINT(readability/streams)
#include <string>

#include "caffe/proto/caffe.pb.h"

int relabel(int l) {
    int label;
    if (l < 0)
        return -1;
    if( l >=1 && l < 20) {
        label = 0;
    } else if( l >= 20 && l< 30) {
        label =1;
    } else if( l >= 30 && l < 40) {
        label =2;
    } else if( l >= 40 && l < 50) {
        label =3;
    } else if( l >= 50) {
        label =4;
    } 
    return label;
}


void convert_dataset(const char* image_filename, const char* label_filename,
        const char* db_filename, const uint32_t num_items) {
  // Open files
  std::ifstream image_file(image_filename, std::ios::in | std::ios::binary);
  std::ifstream label_file(label_filename, std::ios::in | std::ios::binary);
  CHECK(image_file) << "Unable to open file " << image_filename;
  CHECK(label_file) << "Unable to open file " << label_file;
  //
  // Read the magic and the meta data
  //uint32_t num_items =55134;
  uint32_t num_labels;
  uint32_t rows = 32;
  uint32_t cols = 32;

  // Open leveldb
  leveldb::DB* db;
  leveldb::Options options;
  options.create_if_missing = true;
  options.error_if_exists = true;
  leveldb::Status status = leveldb::DB::Open(
      options, db_filename, &db);
  CHECK(status.ok()) << "Failed to open leveldb " << db_filename
      << ". Is it already existing?";

  int label;
  char* pixels = new char[rows * cols];
  const int kMaxKeyLength = 10;
  char key[kMaxKeyLength];
  std::string value;

  caffe::Datum datum;
  datum.set_channels(1);
  datum.set_height(rows);
  datum.set_width(cols);
  LOG(INFO) << "A total of " << num_items << " items.";
  LOG(INFO) << "Rows: " << rows << " Cols: " << cols;
  for (int itemid = 0; itemid < num_items; ++itemid) {
    image_file.read(pixels, rows * cols);
    label_file.read((char*)&label, sizeof(int));
    datum.set_data(pixels, rows*cols);
    datum.set_label(label);
    // age need to be relabeled
    //datum.set_label(relabel(label));
    
    datum.SerializeToString(&value);
    snprintf(key, kMaxKeyLength, "%08d", itemid);
    db->Put(leveldb::WriteOptions(), std::string(key), value);
  }

  delete db;
  delete pixels;
}

int main(int argc, char** argv) {
  if (argc != 5) {
    printf("This script converts the MNIST dataset to the leveldb format used\n"
           "by caffe to perform classification.\n"
           "Usage:\n"
           "    convert_mnist_data input_image_file input_label_file "
           "output_db_file\n"
           "The MNIST dataset could be downloaded at\n"
           "    http://yann.lecun.com/exdb/mnist/\n"
           "You should gunzip them after downloading.\n");
  } else {
    google::InitGoogleLogging(argv[0]);
    convert_dataset(argv[1], argv[2], argv[3], atoi(argv[4]));
  }
  return 0;
}
